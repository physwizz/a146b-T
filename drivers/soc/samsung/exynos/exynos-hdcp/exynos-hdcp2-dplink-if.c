/*
 * drivers/soc/samsung/exynos_hdcp/dp_link/exynos-hdcp2-dplink-if.c
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/module.h>
#include "exynos-hdcp2-log.h"
#include "exynos-hdcp2-dplink-reg.h"
#include "exynos-hdcp2-dplink-if.h"

#if IS_ENABLED(CONFIG_DRM_SAMSUNG_DP)
static void (*pdp_hdcp22_enable)(u32 en);
static int (*pdp_dpcd_read_for_hdcp22)(u32 address, u32 length, u8 *data);
static int (*pdp_dpcd_write_for_hdcp22)(u32 address, u32 length, u8 *data);
void dp_register_func_for_hdcp22(void (*func0)(u32 en), int (*func1)(u32 address, u32 length, u8 *data), int (*func2)(u32 address, u32 length, u8 *data));
#else
void pdp_hdcp22_enable(u32 en);
int pdp_dpcd_read_for_hdcp22(u32 address, u32 length, u8 *data);
int pdp_dpcd_write_for_hdcp22(u32 address, u32 length, u8 *data);
#endif


#if defined(CONFIG_HDCP2_EMULATION_MODE)
#define NETLINK_HDCP 31
#define SOCK_BUF_SIZE (1024 * 512)
#define NETLINK_PORT 1000

typedef enum {
	PREPARE_TO_RECV = 0,
	PREPARE_TO_SEND,
	RECEIVED_DONE,
	CONFIG_REAUTH = 0xFFFF,
} nl_msg_t;

struct sock *nl_sk = NULL;
struct sk_buff sk_buf;
uint8_t dplink_wait;
uint8_t *nl_data;
struct nlmsghdr *nlh;
int hdcp22_enable = 0;

void cb_hdcp_nl_recv_msg(struct sk_buff *skb);

int hdcp_dplink_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.input	= cb_hdcp_nl_recv_msg,
	};

	nl_sk = netlink_kernel_create(&init_net, NETLINK_HDCP, &cfg);
	if (!nl_sk) {
		hdcp_err("Error creating socket.\n");
		return -EINVAL;
	}

	nl_data = (uint8_t *)kzalloc(SOCK_BUF_SIZE, GFP_KERNEL);
	if (!nl_data) {
		hdcp_err("Netlink Socket buffer alloc fail\n");
		return -EINVAL;
	}

	dplink_wait = 1;

	return 0;
}

/* callback for netlink driver */
void cb_hdcp_nl_recv_msg(struct sk_buff *skb)
{
	nlh = (struct nlmsghdr *)skb->data;

	memcpy(nl_data, (uint8_t *)nlmsg_data(nlh), nlmsg_len(nlh));
	dplink_wait = 0;
}

static void nl_recv_msg(nl_msg_t msg)
{
	/* todo: change to not a busy wait */
	while (dplink_wait) {
		hdcp_err("wait dplink_wait\n");
		msleep(1000);
	}

	dplink_wait = 1;
}

static void nl_recv_data(uint8_t *data, uint32_t size)
{
	/* todo: change to not a busy wait */
	while (dplink_wait) {
		hdcp_err("wait dplink_wait\n");
		msleep(1000);
	}

	memcpy(data, nl_data, size);

	dplink_wait = 1;
}

static int nl_send_data(uint8_t *data, uint32_t size)
{
	struct sk_buff *skb_out;
	int ret;

	skb_out = nlmsg_new(size, 0);
	if (!skb_out) {
		hdcp_err("Failed to allocate new skb\n");
		return -1;
	}
	nlh = nlmsg_put(skb_out, 0, 0, 2, size, NLM_F_REQUEST);
	if (!nlh) {
		hdcp_err("fail to nlmsg_put()\n");
		return -1;
	}

	NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
	memcpy(nlmsg_data(nlh), data, size);

	ret = nlmsg_unicast(nl_sk, skb_out, NETLINK_PORT);
	if (ret < 0) {
		hdcp_err("Error while sending to user(%d)\n", ret);
		return -1;
	}

	return 0;
}

static int nl_send_msg(nl_msg_t msg)
{
	return nl_send_data((uint8_t *)&msg, sizeof(nl_msg_t));
}


int hdcp_dplink_recv(uint32_t msg_name, uint8_t *data, uint32_t size)
{
	nl_send_msg(PREPARE_TO_SEND);
	nl_recv_data(data, size);
	nl_send_msg(RECEIVED_DONE);
	return 0;
}



int hdcp_dplink_send(uint32_t msg_name, uint8_t *data, uint32_t size)
{
	nl_send_msg(PREPARE_TO_RECV);
	nl_recv_msg(RECEIVED_DONE);
	nl_send_data(data, size);
	return 0;
}

int hdcp_dplink_is_enabled_hdcp22(void)
{
	return hdcp22_enable;
}

void hdcp_dplink_config(int en)
{
	if (!en) {
		nl_send_msg(CONFIG_REAUTH);
	}

	hdcp22_enable = en;
}
#else

/* Address define for HDCP within DPCD address space */
static uint32_t dpcd_addr[NUM_HDCP22_MSG_NAME] = {
	DPCD_ADDR_HDCP22_Rtx,
	DPCD_ADDR_HDCP22_TxCaps,
	DPCD_ADDR_HDCP22_cert_rx,
	DPCD_ADDR_HDCP22_Rrx,
	DPCD_ADDR_HDCP22_RxCaps,
	DPCD_ADDR_HDCP22_Ekpub_km,
	DPCD_ADDR_HDCP22_Ekh_km_w,
	DPCD_ADDR_HDCP22_m,
	DPCD_ADDR_HDCP22_Hprime,
	DPCD_ADDR_HDCP22_Ekh_km_r,
	DPCD_ADDR_HDCP22_rn,
	DPCD_ADDR_HDCP22_Lprime,
	DPCD_ADDR_HDCP22_Edkey0_ks,
	DPCD_ADDR_HDCP22_Edkey1_ks,
	DPCD_ADDR_HDCP22_riv,
	DPCD_ADDR_HDCP22_RxInfo,
	DPCD_ADDR_HDCP22_seq_num_V,
	DPCD_ADDR_HDCP22_Vprime,
	DPCD_ADDR_HDCP22_Rec_ID_list,
	DPCD_ADDR_HDCP22_V,
	DPCD_ADDR_HDCP22_seq_num_M,
	DPCD_ADDR_HDCP22_k,
	DPCD_ADDR_HDCP22_stream_IDtype,
	DPCD_ADDR_HDCP22_Mprime,
	DPCD_ADDR_HDCP22_RxStatus,
	DPCD_ADDR_HDCP22_Type,
};

int hdcp_dplink_init(void)
{
	/* todo */
	return 0;
}

void hdcp_dplink_config(int en)
{
	pdp_hdcp22_enable(en);
}

int hdcp_dplink_is_enabled_hdcp22(void)
{
	/* todo: check hdcp22 enable */
	return 1;
}

int hdcp_dplink_recv(uint32_t msg_name, uint8_t *data, uint32_t size)
{
	int i;
	int ret;
	int remain;

	if (size > DPCD_PACKET_SIZE) {
		for (i = 0; i < (size / DPCD_PACKET_SIZE); i++) {
			ret = pdp_dpcd_read_for_hdcp22(
				dpcd_addr[msg_name] + i * DPCD_PACKET_SIZE,
				DPCD_PACKET_SIZE,
				&data[i * DPCD_PACKET_SIZE]);
			if (ret) {
				hdcp_err("dpcd read fail. ret(%d)\n", ret);
				return ret;
			}
		}

		remain = size % DPCD_PACKET_SIZE;
		if (remain) {
			ret = pdp_dpcd_read_for_hdcp22(
				dpcd_addr[msg_name] + i * DPCD_PACKET_SIZE,
				remain,
				&data[i * DPCD_PACKET_SIZE]);
			if (ret) {
				hdcp_err("dpcd read fail. ret(%d)\n", ret);
				return ret;
			}
		}
		return 0;
	} else
		return pdp_dpcd_read_for_hdcp22(dpcd_addr[msg_name], size, data);
}

int hdcp_dplink_send(uint32_t msg_name, uint8_t *data, uint32_t size)
{
	int i;
	int ret;

	if (size > DPCD_PACKET_SIZE) {
		for (i = 0; i < (size / DPCD_PACKET_SIZE); i++) {
			ret = pdp_dpcd_write_for_hdcp22(
				dpcd_addr[msg_name] + i * DPCD_PACKET_SIZE,
				DPCD_PACKET_SIZE,
				&data[i * DPCD_PACKET_SIZE]);
			if (ret) {
				hdcp_err("dpcd write fail. ret(%d)\n", ret);
				return ret;
			}
		}
		return 0;
	}
	else
		return pdp_dpcd_write_for_hdcp22(dpcd_addr[msg_name], size, data);
}
#endif

/* todo: get stream info from DP */
#define HDCP_DP_STREAM_NUM	0x01
static uint8_t stream_id[1] = {0x00};
int hdcp_dplink_get_stream_info(uint16_t *num, uint8_t *strm_id)
{
	*num = HDCP_DP_STREAM_NUM;
	memcpy(strm_id, stream_id, sizeof(uint8_t) * (*num));

	return 0;
}

#if IS_ENABLED(CONFIG_DRM_SAMSUNG_DP)
void dp_register_func_for_hdcp22(void (*func0)(u32 en), int (*func1)(u32 address, u32 length, u8 *data), int (*func2)(u32 address, u32 length, u8 *data))
{
	pdp_hdcp22_enable = func0;
	pdp_dpcd_read_for_hdcp22 = func1;
	pdp_dpcd_write_for_hdcp22 = func2;
}
EXPORT_SYMBOL_GPL(dp_register_func_for_hdcp22);
#else
int pdp_dpcd_read_for_hdcp22(u32 address, u32 length, u8 *data)
{
       return 0;
}
int pdp_dpcd_write_for_hdcp22(u32 address, u32 length, u8 *data)
{
       return 0;
}
void pdp_hdcp22_enable(u32 en)
{
       return;
}
#endif

MODULE_LICENSE("GPL");

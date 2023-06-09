/*
 * Copyrights (C) 2016-2020 Samsung Electronics, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/usb_notify.h>
#include <linux/battery/sec_pd.h>
#include <linux/usb/typec/common/pdic_core.h>
#include <linux/usb/typec/common/pdic_notifier.h>
#define DRIVER_DESC   "PDIC Notifier driver"

static struct ccic_notifier_struct ccic_notifier;

static int ccic_notifier_init_done;

char CCIC_NOTI_DEST_Print[CCIC_NOTI_DEST_NUM][10] = {
	[CCIC_NOTIFY_DEV_INITIAL]			= {"INITIAL"},
	[CCIC_NOTIFY_DEV_USB]				= {"USB"},
	[CCIC_NOTIFY_DEV_BATTERY]			= {"BATTERY"},
	[CCIC_NOTIFY_DEV_PDIC]				= {"PDIC"},
	[CCIC_NOTIFY_DEV_MUIC]				= {"MUIC"},
	[CCIC_NOTIFY_DEV_CCIC]				= {"CCIC"},
	[CCIC_NOTIFY_DEV_MANAGER]			= {"MANAGER"},
	[CCIC_NOTIFY_DEV_DP]				= {"DP"},
	[CCIC_NOTIFY_DEV_USB_DP]			= {"USBDP"},
	[CCIC_NOTIFY_DEV_SUB_BATTERY]		= {"BATTERY2"},
	[CCIC_NOTIFY_DEV_SECOND_MUIC]		= {"MUIC2"},
	[CCIC_NOTIFY_DEV_ALL]				= {"ALL"},
};
EXPORT_SYMBOL(CCIC_NOTI_DEST_Print);

char CCIC_NOTI_ID_Print[CCIC_NOTI_ID_NUM][20] = {
	[CCIC_NOTIFY_ID_INITIAL] 		= {"ID_INITIAL"},
	[CCIC_NOTIFY_ID_ATTACH] 		= {"ID_ATTACH"},
	[CCIC_NOTIFY_ID_RID] 			= {"ID_RID"},
	[CCIC_NOTIFY_ID_USB]			= {"ID_USB"},
	[CCIC_NOTIFY_ID_POWER_STATUS] 	= {"ID_POWER_STATUS"},
	[CCIC_NOTIFY_ID_WATER]			= {"ID_WATER"},
	[CCIC_NOTIFY_ID_VCONN]			= {"ID_VCONN"},
	[CCIC_NOTIFY_ID_OTG]			= {"ID_OTG"},
	[CCIC_NOTIFY_ID_TA]				= {"ID_TA"},	
	[CCIC_NOTIFY_ID_DP_CONNECT]		= {"ID_DP_CONNECT"},
	[CCIC_NOTIFY_ID_DP_HPD]			= {"ID_DP_HPD"},
	[CCIC_NOTIFY_ID_DP_LINK_CONF]	= {"ID_DP_LINK_CONF"},
	[CCIC_NOTIFY_ID_USB_DP]			= {"ID_USB_DP"},
	[CCIC_NOTIFY_ID_ROLE_SWAP]		= {"ID_ROLE_SWAP"},
	[CCIC_NOTIFY_ID_FAC]			= {"ID_FAC"},
	[CCIC_NOTIFY_ID_CC_PIN_STATUS]	= {"ID_PIN_STATUS"},
	[CCIC_NOTIFY_ID_WATER_CABLE]	= {"ID_WATER_CABLE"},
};
EXPORT_SYMBOL(CCIC_NOTI_ID_Print);

char CCIC_NOTI_RID_Print[CCIC_NOTI_RID_NUM][15] = {
	[RID_UNDEFINED] = {"RID_UNDEFINED"},
	[RID_000K]		= {"RID_000K"},
	[RID_001K]		= {"RID_001K"},
	[RID_255K]		= {"RID_255K"},
	[RID_301K]		= {"RID_301K"},
	[RID_523K]		= {"RID_523K"},
	[RID_619K]		= {"RID_619K"},
	[RID_OPEN]		= {"RID_OPEN"},
};
EXPORT_SYMBOL(CCIC_NOTI_RID_Print);

char CCIC_NOTI_USB_STATUS_Print[CCIC_NOTI_USB_STATUS_NUM][20] = {
	[USB_STATUS_NOTIFY_DETACH]			= {"USB_DETACH"},
	[USB_STATUS_NOTIFY_ATTACH_DFP]		= {"USB_ATTACH_DFP"},
	[USB_STATUS_NOTIFY_ATTACH_UFP]		= {"USB_ATTACH_UFP"},
	[USB_STATUS_NOTIFY_ATTACH_DRP]		= {"USB_ATTACH_DRP"},
};
EXPORT_SYMBOL(CCIC_NOTI_USB_STATUS_Print);

char CCIC_NOTI_PIN_STATUS_Print[CCIC_NOTI_PIN_STATUS_NUM][20] = {
	[CCIC_NOTIFY_PIN_STATUS_NO_DETERMINATION]	= {"NO_DETERMINATION"},
	[CCIC_NOTIFY_PIN_STATUS_CC1_ACTIVE]			= {"CC1_ACTIVE"},
	[CCIC_NOTIFY_PIN_STATUS_CC2_ACTIVE]			= {"CC2_ACTIVE"},
	[CCIC_NOTIFY_PIN_STATUS_AUDIO_ACCESSORY]	= {"AUDIO_ACCESSORY"},
	[CCIC_NOTIFY_PIN_STATUS_DEBUG_ACCESSORY]	= {"DEBUG_ACCESSORY"},
	[CCIC_NOTIFY_PIN_STATUS_CCIC_ERROR]			= {"CCIC_ERROR"},
	[CCIC_NOTIFY_PIN_STATUS_DISABLED]			= {"DISABLED"},
	[CCIC_NOTIFY_PIN_STATUS_RFU]				= {"RFU"},
};
EXPORT_SYMBOL(CCIC_NOTI_PIN_STATUS_Print);

int ccic_notifier_register(struct notifier_block *nb, notifier_fn_t notifier,
			ccic_notifier_device_t listener)
{
	int ret = 0;
	struct device *ccic_device = get_ccic_device();

	if (!ccic_device) {
		pr_err("%s: ccic_device is null.\n", __func__);
		return -ENODEV;
	}
	pr_info("%s: listener=%d register\n", __func__, listener);

	SET_CCIC_NOTIFIER_BLOCK(nb, notifier, listener);
	ret = blocking_notifier_chain_register(&(ccic_notifier.notifier_call_chain), nb);
	if (ret < 0)
		pr_err("%s: blocking_notifier_chain_register error(%d)\n",
				__func__, ret);

	/* current ccic's attached_device status notify */
	nb->notifier_call(nb, 0,
			&(ccic_notifier.ccic_template));

	return ret;
}
EXPORT_SYMBOL(ccic_notifier_register);

int ccic_notifier_unregister(struct notifier_block *nb)
{
	int ret = 0;

	pr_info("%s: listener=%d unregister\n", __func__, nb->priority);

	ret = blocking_notifier_chain_unregister(&(ccic_notifier.notifier_call_chain), nb);
	if (ret < 0)
		pr_err("%s: blocking_notifier_chain_unregister error(%d)\n",
				__func__, ret);
	DESTROY_CCIC_NOTIFIER_BLOCK(nb);

	return ret;
}
EXPORT_SYMBOL(ccic_notifier_unregister);

static void ccic_uevent_work(int id, int state)
{
	char *water[2] = { "CCIC=WATER", NULL };
	char *dry[2] = { "CCIC=DRY", NULL };
	char *vconn[2] = { "CCIC=VCONN", NULL };
#if defined(CONFIG_SEC_FACTORY)
	char ccicrid[15] = {0,};
	char *rid[2] = {ccicrid, NULL};
	char ccicFacErr[20] = {0,};
	char *facErr[2] = {ccicFacErr, NULL};
	char ccicPinStat[20] = {0,};
	char *pinStat[2] = {ccicPinStat, NULL};
#endif
	struct device *ccic_device = get_ccic_device();

	if (!ccic_device) {
		pr_info("ccic_dev is null\n");
		return;
	}

	pr_info("usb: %s: id=%s state=%d\n", __func__, CCIC_NOTI_ID_Print[id], state);

	switch (id) {
	case CCIC_NOTIFY_ID_WATER:
		if (state)
			kobject_uevent_env(&ccic_device->kobj, KOBJ_CHANGE, water);
		else
			kobject_uevent_env(&ccic_device->kobj, KOBJ_CHANGE, dry);
		break;
	case CCIC_NOTIFY_ID_VCONN:
		kobject_uevent_env(&ccic_device->kobj, KOBJ_CHANGE, vconn);
		break;
#if defined(CONFIG_SEC_FACTORY)
	case CCIC_NOTIFY_ID_RID:
		snprintf(ccicrid, sizeof(ccicrid), "%s",
			(state < CCIC_NOTI_RID_NUM) ? CCIC_NOTI_RID_Print[state] : CCIC_NOTI_RID_Print[0]);
		kobject_uevent_env(&ccic_device->kobj, KOBJ_CHANGE, rid);
		break;
	case CCIC_NOTIFY_ID_FAC:
		snprintf(ccicFacErr, sizeof(ccicFacErr), "%s:%d",
			"ERR_STATE", state);
		kobject_uevent_env(&ccic_device->kobj, KOBJ_CHANGE, facErr);
		break;
	case CCIC_NOTIFY_ID_CC_PIN_STATUS:
		snprintf(ccicPinStat, sizeof(ccicPinStat), "%s",
			(state < CCIC_NOTI_PIN_STATUS_NUM) ?
			CCIC_NOTI_PIN_STATUS_Print[state] : CCIC_NOTI_PIN_STATUS_Print[0]);
		kobject_uevent_env(&ccic_device->kobj, KOBJ_CHANGE, pinStat);
		break;
#endif
	default:
		break;
	}
}

/* ccic's attached_device attach broadcast */
int ccic_notifier_notify(CC_NOTI_TYPEDEF *p_noti, void *pd, int pdic_attach)
{
	int ret = 0;

	ccic_notifier.ccic_template = *p_noti;
	switch (p_noti->id) {
#ifdef CONFIG_USB_TYPEC_MANAGER_NOTIFIER
	case CCIC_NOTIFY_ID_POWER_STATUS:		/* CCIC_NOTIFY_PD_EVENT_SINK */
		pr_info("%s: src:%01x dest:%01x id:%02x "
			"attach:%02x event:%02x data:%01x\n", __func__,
			((CC_NOTI_POWER_STATUS_TYPEDEF *)p_noti)->src,
			((CC_NOTI_POWER_STATUS_TYPEDEF *)p_noti)->dest,
			((CC_NOTI_POWER_STATUS_TYPEDEF *)p_noti)->id,
			((CC_NOTI_POWER_STATUS_TYPEDEF *)p_noti)->attach,
			((CC_NOTI_POWER_STATUS_TYPEDEF *)p_noti)->event,
			((CC_NOTI_POWER_STATUS_TYPEDEF *)p_noti)->data);

		if (pd != NULL) {
			if (!((CC_NOTI_POWER_STATUS_TYPEDEF *)p_noti)->attach &&
				((SEC_PD_SINK_STATUS *)pd)->event != CCIC_NOTIFY_PD_EVENT_CCIC_ATTACH) {
				((SEC_PD_SINK_STATUS *)pd)->event = CCIC_NOTIFY_PD_EVENT_DETACH;
			}
			ccic_notifier.ccic_template.pd = pd;

			pr_info("%s: PD event:%d, num:%d, sel:%d \n", __func__,
				((SEC_PD_SINK_STATUS *)pd)->event,
				((SEC_PD_SINK_STATUS *)pd)->available_pdo_num,
				((SEC_PD_SINK_STATUS *)pd)->selected_pdo_num);
		}
		break;
#endif
	case CCIC_NOTIFY_ID_ATTACH:
		pr_info("%s: src:%01x dest:%01x id:%02x "
			"attach:%02x cable_type:%02x rprd:%01x\n", __func__,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->src,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->dest,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->id,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->attach,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->cable_type,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->rprd);
		break;
	case CCIC_NOTIFY_ID_RID:
		pr_info("%s: src:%01x dest:%01x id:%02x rid:%02x\n", __func__,
			((CC_NOTI_RID_TYPEDEF *)p_noti)->src,
			((CC_NOTI_RID_TYPEDEF *)p_noti)->dest,
			((CC_NOTI_RID_TYPEDEF *)p_noti)->id,
			((CC_NOTI_RID_TYPEDEF *)p_noti)->rid);
#if defined(CONFIG_SEC_FACTORY)
			ccic_uevent_work(CCIC_NOTIFY_ID_RID, ((CC_NOTI_RID_TYPEDEF *)p_noti)->rid);
#endif
		break;
#ifdef CONFIG_SEC_FACTORY
	case CCIC_NOTIFY_ID_FAC:
		pr_info("%s: src:%01x dest:%01x id:%02x ErrState:%02x\n", __func__,
			p_noti->src, p_noti->dest, p_noti->id, p_noti->sub1);
			ccic_uevent_work(CCIC_NOTIFY_ID_FAC, p_noti->sub1);
			return 0;
#endif
	case CCIC_NOTIFY_ID_WATER:
		pr_info("%s: src:%01x dest:%01x id:%02x attach:%02x\n", __func__,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->src,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->dest,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->id,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->attach);
			ccic_uevent_work(CCIC_NOTIFY_ID_WATER, ((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->attach);
		break;
	case CCIC_NOTIFY_ID_VCONN:
		ccic_uevent_work(CCIC_NOTIFY_ID_VCONN, 0);
		break;
	case CCIC_NOTIFY_ID_ROLE_SWAP:
		pr_info("%s: src:%01x dest:%01x id:%02x sub1:%02x\n", __func__,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->src,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->dest,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->id,
			((CC_NOTI_ATTACH_TYPEDEF *)p_noti)->attach);
		break;
#ifdef CONFIG_SEC_FACTORY
	case CCIC_NOTIFY_ID_CC_PIN_STATUS:
		pr_info("%s: src:%01x dest:%01x id:%02x pinStatus:%02x\n", __func__,
			p_noti->src, p_noti->dest, p_noti->id, p_noti->sub1);
			ccic_uevent_work(CCIC_NOTIFY_ID_CC_PIN_STATUS, p_noti->sub1);
			return 0;
#endif
	default:
		pr_info("%s: src:%01x dest:%01x id:%02x "
			"sub1:%d sub2:%02x sub3:%02x\n", __func__,
			((CC_NOTI_TYPEDEF *)p_noti)->src,
			((CC_NOTI_TYPEDEF *)p_noti)->dest,
			((CC_NOTI_TYPEDEF *)p_noti)->id,
			((CC_NOTI_TYPEDEF *)p_noti)->sub1,
			((CC_NOTI_TYPEDEF *)p_noti)->sub2,
			((CC_NOTI_TYPEDEF *)p_noti)->sub3);
		break;
	}
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
	store_usblog_notify(NOTIFY_CCIC_EVENT, (void *)p_noti, NULL);
#endif
	ret = blocking_notifier_call_chain(&(ccic_notifier.notifier_call_chain),
			p_noti->id, &(ccic_notifier.ccic_template));


	switch (ret) {
	case NOTIFY_STOP_MASK:
	case NOTIFY_BAD:
		pr_err("%s: notify error occur(0x%x)\n", __func__, ret);
		break;
	case NOTIFY_DONE:
	case NOTIFY_OK:
		pr_info("%s: notify done(0x%x)\n", __func__, ret);
		break;
	default:
		pr_info("%s: notify status unknown(0x%x)\n", __func__, ret);
		break;
	}

	return ret;

}
EXPORT_SYMBOL(ccic_notifier_notify);

static int __init ccic_notifier_init(void)
{
	int ret = 0;

	pr_info("%s\n", __func__);
	if (ccic_notifier_init_done) {
		pr_err("%s already registered\n", __func__);
		goto out;
	}
	ccic_notifier_init_done = 1;
	ccic_core_init();
	BLOCKING_INIT_NOTIFIER_HEAD(&(ccic_notifier.notifier_call_chain));

out:
	return ret;
}

static void __exit ccic_notifier_exit(void)
{
	pr_info("%s: exit\n", __func__);
}

module_init(ccic_notifier_init);
module_exit(ccic_notifier_exit);

MODULE_AUTHOR("Samsung USB Team");
MODULE_DESCRIPTION("Pdic Notifier");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRIVER_DESC);

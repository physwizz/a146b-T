// SPDX-License-Identifier: GPL-2.0
/*
 * Samsung Exynos SoC series Pablo driver
 *
 * Copyright (c) 2019 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "is-core.h"
#include "is-dvfs.h"

/* Macro to find the facing direction for current sensor. */
#define POS_BIT(pos)	(1 << pos)
#define REAR_SENSOR_POS_MASK	(			\
		POS_BIT(SENSOR_POSITION_REAR)		\
		| POS_BIT(SENSOR_POSITION_REAR2)	\
		| POS_BIT(SENSOR_POSITION_REAR3)	\
		| POS_BIT(SENSOR_POSITION_REAR4)	\
		| POS_BIT(SENSOR_POSITION_REAR_TOF)	\
		)
#define FRONT_SENSOR_POS_MASK	(			\
		POS_BIT(SENSOR_POSITION_FRONT)		\
		| POS_BIT(SENSOR_POSITION_FRONT2)	\
		| POS_BIT(SENSOR_POSITION_FRONT3)	\
		| POS_BIT(SENSOR_POSITION_FRONT4)	\
		| POS_BIT(SENSOR_POSITION_FRONT_TOF)	\
		)
#define REAR_TOF_SENSOR_POS_MASK	(		\
		POS_BIT(SENSOR_POSITION_REAR_TOF)	\
		)
#define FRONT_TOF_SENSOR_POS_MASK	(		\
		POS_BIT(SENSOR_POSITION_FRONT_TOF)	\
		)
#define	IS_REAR_SENSOR(pos)		(POS_BIT(pos) & REAR_SENSOR_POS_MASK)
#define	IS_FRONT_SENSOR(pos)		(POS_BIT(pos) & FRONT_SENSOR_POS_MASK)
#define IS_REAR_DUAL(sensor_map)	(is_get_bit_count(sensor_map & REAR_SENSOR_POS_MASK) > 1)
#define IS_FRONT_DUAL(sensor_map)	(is_get_bit_count(sensor_map & FRONT_SENSOR_POS_MASK) > 1)
#define	IS_REAR_TOF_SENSOR(pos)		(POS_BIT(pos) & REAR_TOF_SENSOR_POS_MASK)
#define	IS_FRONT_TOF_SENSOR(pos)	(POS_BIT(pos) & FRONT_TOF_SENSOR_POS_MASK)

/* dvfs scenario check logic data */
DECLARE_DVFS_CHK_FUNC(IS_SN_SECURE_FRONT);

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT2_PREVIEW);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT2_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT2_CAMCORDING);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT2_CAMCORDING_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW_FULL);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_WHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_60FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_60FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_120FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_WHD_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_60FPS_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_60FPS_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_120FPS_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_PREVIEW);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_VT1);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_VT2);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_VT4);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW_HIGH_SPEED_FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_VIDEO_HIGH_SPEED_120FPS);

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR3_PREVIEW_FHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR3_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR3_CAMCORDING_FHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR3_CAMCORDING_FHD_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_PREVIEW_FHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_FHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_UHD_8K);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_FHD_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_UHD_8K_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_FULL);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_FHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_WHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_UHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_UHD_60FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_FHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_HDR);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_WHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_60FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_8K);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_FHD_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_HDR_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_WHD_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_8K_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_PREVIEW);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_FHD_CAMCORDING);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_FHD_CAMCORDING_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_UHD_CAMCORDING);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_UHD_CAMCORDING_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_PREVIEW);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_PREVIEW_WHD);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_WHD_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_FHD_CAMCORDING);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_FHD_CAMCORDING_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_UHD_CAMCORDING);
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_UHD_CAMCORDING_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_PIP_PREVIEW);
DECLARE_DVFS_CHK_FUNC(IS_SN_PIP_CAPTURE);
DECLARE_DVFS_CHK_FUNC(IS_SN_PIP_CAMCORDING);
DECLARE_DVFS_CHK_FUNC(IS_SN_PIP_CAMCORDING_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_PREVIEW_HIGH_SPEED_FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS_PRO);
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_120FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_120FPS_PRO);
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_240FPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_DUALFPS);
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS_CAPTURE);

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_TOF);

/* external isp's dvfs */
DECLARE_EXT_DVFS_CHK_FUNC(IS_SN_EXT_DUAL);
DECLARE_EXT_DVFS_CHK_FUNC(IS_SN_EXT_REAR);
DECLARE_EXT_DVFS_CHK_FUNC(IS_SN_EXT_FRONT);
DECLARE_EXT_DVFS_CHK_FUNC(IS_SN_EXT_SECURE);

/* Thermal */
DECLARE_DVFS_CHK_FUNC(IS_SN_THERMAL_8K);

#if defined(ENABLE_DVFS)
/*
 * Static Scenario Set
 * You should describe static scenario by priorities of scenario.
 * And you should name array 'static_scenarios'
 */

struct is_dvfs_scenario static_scenarios[] = {
	{
		.scenario_id		= IS_SN_DUAL_TOF,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_TOF),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_TOF),
	}, {
		.scenario_id		= IS_SN_PREVIEW_HIGH_SPEED_FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_PREVIEW_HIGH_SPEED_FPS),
		.keep_frame_tick	= KEEP_FRAME_TICK_FASTAE,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_PREVIEW_HIGH_SPEED_FPS),
	}, {
		.scenario_id		= IS_SN_SECURE_FRONT,
		.scenario_nm		= DVFS_SN_STR(IS_SN_SECURE_FRONT),
		.check_func 		= GET_DVFS_CHK_FUNC(IS_SN_SECURE_FRONT),
	}, {
		.scenario_id		= IS_SN_DUAL_SYNC_FHD_CAMCORDING,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_SYNC_FHD_CAMCORDING),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_FHD_CAMCORDING),
	}, {
		.scenario_id		= IS_SN_DUAL_SYNC_UHD_CAMCORDING,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_SYNC_UHD_CAMCORDING),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_UHD_CAMCORDING),
	}, {
		.scenario_id		= IS_SN_DUAL_SYNC_PREVIEW,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_SYNC_PREVIEW),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_PREVIEW),
	}, {
		.scenario_id		= IS_SN_DUAL_SYNC_PREVIEW_WHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_SYNC_PREVIEW_WHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_PREVIEW_WHD),
	}, {
		.scenario_id		= IS_SN_FRONT_DUAL_SYNC_PREVIEW,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_DUAL_SYNC_PREVIEW),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_PREVIEW),
	}, {
		.scenario_id		= IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING),
	}, {
		.scenario_id            = IS_SN_VIDEO_HIGH_SPEED_60FPS_PRO,
		.scenario_nm            = DVFS_SN_STR(IS_SN_VIDEO_HIGH_SPEED_60FPS_PRO),
		.check_func             = GET_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS_PRO),
	}, {
		.scenario_id		= IS_SN_VIDEO_HIGH_SPEED_60FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_VIDEO_HIGH_SPEED_60FPS),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS),
	}, {
		.scenario_id            = IS_SN_VIDEO_HIGH_SPEED_120FPS_PRO,
		.scenario_nm            = DVFS_SN_STR(IS_SN_VIDEO_HIGH_SPEED_120FPS_PRO),
		.check_func             = GET_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_120FPS_PRO),
	}, {
		.scenario_id		= IS_SN_VIDEO_HIGH_SPEED_120FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_VIDEO_HIGH_SPEED_120FPS),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_120FPS),
	}, {
		.scenario_id		= IS_SN_VIDEO_HIGH_SPEED_240FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_VIDEO_HIGH_SPEED_240FPS),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_240FPS),
	}, {
		.scenario_id		= IS_SN_VIDEO_HIGH_SPEED_DUALFPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_VIDEO_HIGH_SPEED_DUALFPS),
		.keep_frame_tick	= IS_DVFS_SKIP_DYNAMIC,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_DUALFPS),
	}, {
		.scenario_id		= IS_SN_REAR2_CAMCORDING_UHD_8K,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR2_CAMCORDING_UHD_8K),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_UHD_8K),
	}, {
		.scenario_id		= IS_SN_REAR2_CAMCORDING_FHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR2_CAMCORDING_FHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_FHD),
	}, {
		.scenario_id		= IS_SN_REAR2_PREVIEW_FHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR2_PREVIEW_FHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR2_PREVIEW_FHD),
	}, {
		.scenario_id		= IS_SN_REAR3_CAMCORDING_FHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR3_CAMCORDING_FHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR3_CAMCORDING_FHD),
	}, {
		.scenario_id		= IS_SN_REAR3_PREVIEW_FHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR3_PREVIEW_FHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR3_PREVIEW_FHD),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_UHD_60FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_UHD_60FPS),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_60FPS),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_UHD_8K,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_UHD_8K),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_8K),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_UHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_UHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_WHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_WHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_WHD),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_FHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_FHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_FHD),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_HDR,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_HDR),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_HDR),
	}, {
		.scenario_id		= IS_SN_REAR_PREVIEW_FULL,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_PREVIEW_FULL),
		.check_func 		= GET_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_FULL),
	}, {
		.scenario_id		= IS_SN_REAR_PREVIEW_FHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_PREVIEW_FHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_FHD),
	}, {
		.scenario_id		= IS_SN_REAR_PREVIEW_WHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_PREVIEW_WHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_WHD),
	}, {
		.scenario_id		= IS_SN_REAR_PREVIEW_UHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_PREVIEW_UHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_UHD),
	}, {
		.scenario_id		= IS_SN_REAR_PREVIEW_UHD_60FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_PREVIEW_UHD_60FPS),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_UHD_60FPS),
	}, {
		.scenario_id		= IS_SN_DUAL_FHD_CAMCORDING,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_FHD_CAMCORDING),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_FHD_CAMCORDING),
	}, {
		.scenario_id		= IS_SN_DUAL_UHD_CAMCORDING,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_UHD_CAMCORDING),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_UHD_CAMCORDING),
	}, {
		.scenario_id		= IS_SN_DUAL_PREVIEW,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_PREVIEW),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_PREVIEW),
	}, {
		.scenario_id		= IS_SN_PIP_CAMCORDING,
		.scenario_nm		= DVFS_SN_STR(IS_SN_PIP_CAMCORDING),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_PIP_CAMCORDING),
	}, {
		.scenario_id		= IS_SN_PIP_PREVIEW,
		.scenario_nm		= DVFS_SN_STR(IS_SN_PIP_PREVIEW),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_PIP_PREVIEW),
	}, {
		.scenario_id		= IS_SN_FRONT_VT1,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_VT1),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_VT1),
	}, {
		.scenario_id		= IS_SN_FRONT_VT2,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_VT2),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_VT2),
	}, {
		.scenario_id		= IS_SN_FRONT_VT4,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_VT4),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_VT4),
	}, {
		.scenario_id		= IS_SN_FRONT_PREVIEW_HIGH_SPEED_FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_PREVIEW_HIGH_SPEED_FPS),
		.keep_frame_tick	= KEEP_FRAME_TICK_FASTAE,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW_HIGH_SPEED_FPS),
	}, {
		.scenario_id            = IS_SN_FRONT_VIDEO_HIGH_SPEED_120FPS,
		.scenario_nm            = DVFS_SN_STR(IS_SN_FRONT_VIDEO_HIGH_SPEED_120FPS),
		.check_func             = GET_DVFS_CHK_FUNC(IS_SN_FRONT_VIDEO_HIGH_SPEED_120FPS),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_FHD_120FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_FHD_120FPS),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_120FPS),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_FHD_60FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_FHD_60FPS),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_60FPS),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_UHD_60FPS,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_UHD_60FPS),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_60FPS),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_WHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_WHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_WHD),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_UHD,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_UHD),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD),
	}, {
		.scenario_id		= IS_SN_FRONT_PREVIEW_FULL,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_PREVIEW_FULL),
		.check_func 		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW_FULL),
	}, {
		.scenario_id		= IS_SN_FRONT_PREVIEW,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_PREVIEW),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW),
	}, {
		.scenario_id		= IS_SN_FRONT2_CAMCORDING,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT2_CAMCORDING),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT2_CAMCORDING),
	}, {
		.scenario_id		= IS_SN_FRONT2_PREVIEW,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT2_PREVIEW),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT2_PREVIEW),
	}
};

/*
 * Dynamic Scenario Set
 * You should describe dynamic scenario by priorities of scenario.
 * And you should name array 'dynamic_scenarios'
 */
static struct is_dvfs_scenario dynamic_scenarios[] = {
	{
		/* Do not insert above this. Thermal scenario has first high priority. */
		.scenario_id		= IS_SN_THERMAL_8K,
		.scenario_nm		= DVFS_SN_STR(IS_SN_THERMAL_8K),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_THERMAL_8K),
	}, {
		.scenario_id		= IS_SN_DUAL_SYNC_FHD_CAMCORDING_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_SYNC_FHD_CAMCORDING_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_FHD_CAMCORDING_CAPTURE),
	}, {
		.scenario_id		= IS_SN_DUAL_SYNC_UHD_CAMCORDING_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_SYNC_UHD_CAMCORDING_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_UHD_CAMCORDING_CAPTURE),
	}, {
		.scenario_id		= IS_SN_DUAL_SYNC_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_SYNC_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_CAPTURE),
	}, {
		.scenario_id		= IS_SN_DUAL_SYNC_WHD_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_SYNC_WHD_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_WHD_CAPTURE),
	}, {
		.scenario_id		= IS_SN_DUAL_FHD_CAMCORDING_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_FHD_CAMCORDING_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_FHD_CAMCORDING_CAPTURE),
	}, {
		.scenario_id		= IS_SN_DUAL_UHD_CAMCORDING_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_UHD_CAMCORDING_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_UHD_CAMCORDING_CAPTURE),
	}, {
		.scenario_id		= IS_SN_DUAL_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_DUAL_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_DUAL_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR3_CAMCORDING_FHD_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR3_CAMCORDING_FHD_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func 		= GET_DVFS_CHK_FUNC(IS_SN_REAR3_CAMCORDING_FHD_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR3_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR3_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func 		= GET_DVFS_CHK_FUNC(IS_SN_REAR3_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR2_CAMCORDING_UHD_8K_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR2_CAMCORDING_UHD_8K_CAPTURE),
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_UHD_8K_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR2_CAMCORDING_FHD_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR2_CAMCORDING_FHD_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func 		= GET_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_FHD_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR2_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR2_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func 		= GET_DVFS_CHK_FUNC(IS_SN_REAR2_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_HDR_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_HDR_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_HDR_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_FHD_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_FHD_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_FHD_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_WHD_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_WHD_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_WHD_CAPTURE),
	}, {
		.scenario_id		= IS_SN_VIDEO_HIGH_SPEED_60FPS_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_VIDEO_HIGH_SPEED_60FPS_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func 		= GET_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_UHD_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_UHD_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR_CAMCORDING_UHD_8K_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAMCORDING_UHD_8K_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_8K_CAPTURE),
	}, {
		.scenario_id		= IS_SN_REAR_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_REAR_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_REAR_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_FHD_60FPS_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_FHD_60FPS_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_60FPS_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_UHD_60FPS_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_UHD_60FPS_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_60FPS_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_FHD_120FPS_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_FHD_120FPS_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_120FPS_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_WHD_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_WHD_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_WHD_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_CAMCORDING_UHD_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAMCORDING_UHD_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT2_CAMCORDING_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT2_CAMCORDING_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT2_CAMCORDING_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT2_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT2_CAPTURE),
		.keep_frame_tick	= IS_DVFS_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT2_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_DUAL_SYNC_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_DUAL_SYNC_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_CAPTURE),
	}, {
		.scenario_id		= IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING_CAPTURE),
	}, {
		.scenario_id		= IS_SN_PIP_CAMCORDING_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_PIP_CAMCORDING_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_PIP_CAMCORDING_CAPTURE),
	}, {
		.scenario_id		= IS_SN_PIP_CAPTURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_PIP_CAPTURE),
		.keep_frame_tick	= IS_DVFS_DUAL_CAPTURE_TICK,
		.check_func		= GET_DVFS_CHK_FUNC(IS_SN_PIP_CAPTURE),
	}
};

/*
 * External Sensor/Vision Scenario Set
 * You should describe external scenario by priorities of scenario.
 * And you should name array 'external_scenarios'
 */
struct is_dvfs_scenario external_scenarios[] = {
	{
		.scenario_id		= IS_SN_EXT_DUAL,
		.scenario_nm		= DVFS_SN_STR(IS_SN_EXT_DUAL),
		.ext_check_func		= GET_DVFS_CHK_FUNC(IS_SN_EXT_DUAL),
	}, {
		.scenario_id		= IS_SN_EXT_REAR,
		.scenario_nm		= DVFS_SN_STR(IS_SN_EXT_REAR),
		.ext_check_func		= GET_DVFS_CHK_FUNC(IS_SN_EXT_REAR),
	}, {
		.scenario_id		= IS_SN_EXT_FRONT,
		.scenario_nm		= DVFS_SN_STR(IS_SN_EXT_FRONT),
		.ext_check_func		= GET_DVFS_CHK_FUNC(IS_SN_EXT_FRONT),
	}, {
		.scenario_id		= IS_SN_EXT_SECURE,
		.scenario_nm		= DVFS_SN_STR(IS_SN_EXT_SECURE),
		.ext_check_func 	= GET_DVFS_CHK_FUNC(IS_SN_EXT_SECURE),
	},
};
#else
/*
 * Default Scenario can not be seleted, this declaration is for static variable.
 */
static struct is_dvfs_scenario static_scenarios[] = {
	{
		.scenario_id		= IS_SN_DEFAULT,
		.scenario_nm		= NULL,
		.keep_frame_tick	= 0,
		.check_func		= NULL,
	},
};

static struct is_dvfs_scenario dynamic_scenarios[] = {
	{
		.scenario_id		= IS_SN_DEFAULT,
		.scenario_nm		= NULL,
		.keep_frame_tick	= 0,
		.check_func		= NULL,
	},
};

static struct is_dvfs_scenario external_scenarios[] = {
	{
		.scenario_id		= IS_SN_DEFAULT,
		.scenario_nm		= NULL,
		.keep_frame_tick	= 0,
		.ext_check_func		= NULL,
	},
};
#endif

/* ToF with any rear sensor: skip dvfs level changing */
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_TOF)
{
	if (IS_REAR_TOF_SENSOR(position) && (stream_cnt > 1) &&
		(resol < SIZE_FHD))
		return DVFS_SKIP;
	else
		return DVFS_NOT_MATCHED;
}

/* rear fastAE */
DECLARE_DVFS_CHK_FUNC(IS_SN_PREVIEW_HIGH_SPEED_FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = (mask == ISS_SUB_SCENARIO_FHD_60FPS) ||
		(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_ON) ||
		(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_AUTO) ||
		(mask == ISS_SUB_SCENARIO_UHD_60FPS) ||
		(mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_ON) ||
		(mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_AUTO) ||
		(mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED) ||
		(mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED_960FPS);
	if (IS_REAR_SENSOR(position) && (fps > 60) && !setfile_flag)
		return 1;
	else
		return 0;
}

/* front fastAE */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW_HIGH_SPEED_FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = (mask == ISS_SUB_SCENARIO_FHD_60FPS) ||
		(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_ON) ||
		(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_AUTO) ||
		(mask == ISS_SUB_SCENARIO_UHD_60FPS) ||
		(mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_ON) ||
		(mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_AUTO) ||
		(mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED);

	if (IS_FRONT_SENSOR(position) && (fps > 30) && !setfile_flag)
		return 1;
	else
		return 0;
}

/* front 120fps recording */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_VIDEO_HIGH_SPEED_120FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = (mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED);

	if (IS_FRONT_SENSOR(position) &&
			(fps > 60) &&
			(fps <= 120) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* secure front */
DECLARE_DVFS_CHK_FUNC(IS_SN_SECURE_FRONT)
{
	u32 scen = (device->setfile & IS_SCENARIO_MASK) >> IS_SCENARIO_SHIFT;
	bool scenario_flag = (scen == IS_SCENARIO_COLOR_IRIS);

	if (scenario_flag && stream_cnt > 1)
		return 1;
	else
		return 0;
}

/* dual fhd camcording sync */
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_FHD_CAMCORDING)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if (IS_REAR_DUAL(sensor_map) &&
		setfile_flag &&
		(stream_cnt > 1) &&
		(dual_info->mode == IS_DUAL_MODE_SYNC))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_THERMAL_8K)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;
	int cur_scen_id = static_ctrl->cur_scenario_id;
	bool scen_flag = ((cur_scen_id == IS_SN_REAR2_CAMCORDING_UHD_8K) ||
			(cur_scen_id == IS_SN_REAR_CAMCORDING_UHD_8K));
	uint16_t ssrmMasking = (AA_SSRM_HINT_LOW_POWER_MODE_L1
					| AA_SSRM_HINT_LOW_POWER_MODE_L2
					| AA_SSRM_HINT_LOW_POWER_MODE_L3);

	if (!test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		scen_flag &&
		(ssrmMasking & shot->ctl.aa.vendor_ssrmHint))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_FHD_CAMCORDING_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_DUAL_SYNC_FHD_CAMCORDING) &&
		(dual_info->mode == IS_DUAL_MODE_SYNC))
		return 1;
	else
		return 0;
}

/* dual uhd camcording sync */
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_UHD_CAMCORDING)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_UHD_30FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_AUTO));

	if (IS_REAR_DUAL(sensor_map) &&
		setfile_flag &&
		(stream_cnt > 1) && (resol > SIZE_12MP_FHD_BDS) &&
		(dual_info->mode == IS_DUAL_MODE_SYNC))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_UHD_CAMCORDING_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_DUAL_SYNC_UHD_CAMCORDING) &&
		(dual_info->mode == IS_DUAL_MODE_SYNC))
		return 1;
	else
		return 0;
}

/* dual preview sync */
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_PREVIEW)
{
	if (IS_REAR_DUAL(sensor_map) &&
		(stream_cnt > 1) && (resol <= SIZE_12MP_FHD_BDS) &&
		(dual_info->mode == IS_DUAL_MODE_SYNC) &&
		((dual_info->max_fps[SENSOR_POSITION_REAR2] + dual_info->max_fps[SENSOR_POSITION_REAR3]) > 0))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_DUAL_SYNC_PREVIEW) &&
		(dual_info->mode == IS_DUAL_MODE_SYNC))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_PREVIEW_WHD)
{
	if (IS_REAR_DUAL(sensor_map) &&
		(stream_cnt > 1) && (resol > SIZE_12MP_FHD_BDS) &&
		(dual_info->mode == IS_DUAL_MODE_SYNC))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_SYNC_WHD_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_DUAL_SYNC_PREVIEW_WHD) &&
		(dual_info->mode == IS_DUAL_MODE_SYNC))
		return 1;
	else
		return 0;
}

/* dual fhd camcording */
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_FHD_CAMCORDING)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if (IS_REAR_DUAL(sensor_map) &&
		setfile_flag &&
		(stream_cnt > 1))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_FHD_CAMCORDING_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_DUAL_FHD_CAMCORDING))
		return 1;
	else
		return 0;
}

/* dual uhd camcording */
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_UHD_CAMCORDING)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_UHD_30FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_AUTO));

	if (IS_REAR_DUAL(sensor_map) &&
		setfile_flag &&
		(stream_cnt > 1) && (resol > SIZE_12MP_FHD_BDS))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_UHD_CAMCORDING_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_DUAL_UHD_CAMCORDING))
		return 1;
	else
		return 0;
}

/* dual preview */
DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_PREVIEW)
{
	if (IS_REAR_DUAL(sensor_map) && (stream_cnt > 1))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_DUAL_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_DUAL_PREVIEW))
		return 1;
	else
		return 0;
}

/* pip camcording */
DECLARE_DVFS_CHK_FUNC(IS_SN_PIP_CAMCORDING)
{
	return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_PIP_CAMCORDING_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_PIP_CAMCORDING))
		return 1;
	else
		return 0;
}

/* pip preview */
DECLARE_DVFS_CHK_FUNC(IS_SN_PIP_PREVIEW)
{
	if (stream_cnt > 1)
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_PIP_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_PIP_PREVIEW))
		return 1;
	else
		return 0;
}

/* 60fps recording */
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_FHD_60FPS) ||
			(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_AUTO));

	if (IS_REAR_SENSOR(position) &&
			(fps > 30) &&
			(fps <= 60) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* 60fps recording pro */
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS_PRO)
{
	u32 scen = (device->setfile & IS_SCENARIO_MASK) >> IS_SCENARIO_SHIFT;
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool scenario_flag = (scen == IS_SCENARIO_PRO_VIDEO);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_FHD_60FPS) ||
			(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_AUTO));

	if ((scenario_flag) &&
			IS_REAR_SENSOR(position) &&
			(fps > 30) &&
			(fps <= 60) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* 120fps recording */
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_120FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = (mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED);

	if (IS_REAR_SENSOR(position) &&
			(fps > 60) &&
			(fps <= 120) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* 120fps recording pro */
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_120FPS_PRO)
{
	u32 scen = (device->setfile & IS_SCENARIO_MASK) >> IS_SCENARIO_SHIFT;
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = (mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED);
	bool scenario_flag = (scen == IS_SCENARIO_PRO_VIDEO);

	if ((scenario_flag) &&
			IS_REAR_SENSOR(position) &&
			(fps > 60) &&
			(fps <= 120) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* 240fps recording */
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_240FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = (mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED);

	if (IS_REAR_SENSOR(position) && (fps > 120) && setfile_flag)
		return 1;
	else
		return 0;
}

/* 60fps --> 240fps recording */
DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_DUALFPS)
{
	u32 scen = (device->setfile & IS_SCENARIO_MASK) >> IS_SCENARIO_SHIFT;
	bool scenario_flag = (scen == IS_SCENARIO_HIGH_SPEED_DUALFPS);

	if (IS_REAR_SENSOR(position) && (scenario_flag))
		return 1;
	else
		return 0;
}

/* rear3 camcording FHD*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR3_CAMCORDING_FHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if ((position == SENSOR_POSITION_REAR3) &&
			(fps <= 30) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* rear2 camcording FHD*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_FHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if ((position == SENSOR_POSITION_REAR2) &&
			(fps <= 30) &&
			(resol <= SIZE_17MP) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* rear2 camcording 8K UHD*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_UHD_8K)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO) ||
			(mask == ISS_SUB_SCENARIO_8K_WDR_AUTO));

	if ((position == SENSOR_POSITION_REAR2) &&
			(fps <= 30) &&
			(resol > SIZE_17MP) &&
			(resol <= SIZE_8K) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* rear camcording FHD*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_FHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if ((position == SENSOR_POSITION_REAR) &&
			(fps <= 30) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* rear camcording WHD*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_WHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_AUTO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if (IS_REAR_SENSOR(position) &&
			(fps <= 30) &&
			(resol > SIZE_12MP_FHD_BDS) &&
			(resol <= SIZE_12MP_QHD_BDS) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* rear camcording UHD*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_UHD_30FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_AUTO));

	if (IS_REAR_SENSOR(position) &&
			(fps <= 30) &&
			(resol >= SIZE_UHD) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* rear camcording UHD@60fps */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_60FPS)
{
	u32 scen = (device->setfile & IS_SCENARIO_MASK) >> IS_SCENARIO_SHIFT;
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_60FPS ) ||
			(mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_AUTO));
	bool scenario_flag = (scen == IS_SCENARIO_PRO_VIDEO);

	if (IS_REAR_SENSOR(position) &&
			(fps > 30) &&
			(fps <= 60) &&
			setfile_flag) {
		if (scenario_flag)
			return 1;
		else
			if(resol >= SIZE_UHD)
				return 1;
	}
	return 0;
}

/* rear camcording 8K UHD*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_8K)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO) ||
			(mask == ISS_SUB_SCENARIO_8K_WDR_AUTO));

	if ((position == SENSOR_POSITION_REAR) &&
			(fps <= 30) &&
			(resol > SIZE_17MP) &&
			(resol <= SIZE_8K) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* rear camcording FHD*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_HDR)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if ((position == SENSOR_POSITION_REAR) &&
			setfile_flag)
		return 1;
	else
		return 0;
}

/* rear3 preview FHD */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR3_PREVIEW_FHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	/* TODO: There is no size variant preview scenario for rear3 */
	if ((position == SENSOR_POSITION_REAR3) &&
			(streaming_cnt == 1) &&
			(fps <= 30) &&
			(!setfile_flag))

		return 1;
	else
		return 0;
}

/* rear2 preview FHD */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_PREVIEW_FHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	/* TODO: There is no size variant preview scenario for rear3 */
	if ((position == SENSOR_POSITION_REAR2) &&
			(streaming_cnt == 1) &&
			(fps <= 30) &&
			(!setfile_flag))

		return 1;
	else
		return 0;
}

/* rear full resolution preview */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_FULL)
{
	u32 scen = (device->setfile & IS_SCENARIO_MASK) >> IS_SCENARIO_SHIFT;
	bool scenario_flag = (scen == IS_SCENARIO_FULL_SIZE);

	if (IS_REAR_SENSOR(position) && scenario_flag && streaming_cnt == 1)
		return 1;
	else
		return 0;
}

/* rear preview FHD */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_FHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if ((position == SENSOR_POSITION_REAR) &&
			(streaming_cnt == 1) &&
			(fps <= 30) &&
			(resol <= SIZE_12MP_FHD_BDS) &&
			(dual_info->max_fps[SENSOR_POSITION_REAR2] == 0) &&
			(dual_info->max_fps[SENSOR_POSITION_REAR3] == 0) &&
			(!setfile_flag))

		return 1;
	else
		return 0;
}

/* rear preview WHD */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_WHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_AUTO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if (IS_REAR_SENSOR(position) &&
			(streaming_cnt == 1) &&
			(fps <= 30) &&
			(resol > SIZE_12MP_FHD_BDS) &&
			(resol <= SIZE_12MP_QHD_BDS) &&
			(!setfile_flag))
		return 1;
	else
		return 0;
}

/* rear preview UHD */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_UHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_UHD_30FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_AUTO));

	if (IS_REAR_SENSOR(position) &&
			(streaming_cnt == 1) &&
			(fps <= 30) &&
			(resol > SIZE_12MP_QHD_BDS) &&
			(!setfile_flag))
		return 1;
	else
		return 0;
}

/* rear preview UHD@60fps*/
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_PREVIEW_UHD_60FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_60FPS ) ||
			(mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_AUTO));

	if (IS_REAR_SENSOR(position) &&
			(streaming_cnt == 1) &&
			(fps > 30) &&
			(fps <= 60) &&
			(resol > SIZE_12MP_FHD_BDS) &&
			(!setfile_flag))
		return 1;
	else
		return 0;
}

/* front vt1 */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_VT1)
{
	if (IS_FRONT_SENSOR(position) &&
			((device->setfile & IS_SETFILE_MASK) \
			 == ISS_SUB_SCENARIO_VT))
		return 1;
	else
		return 0;
}

/* front vt2 */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_VT2)
{
	if (IS_FRONT_SENSOR(position) &&
			((device->setfile & IS_SETFILE_MASK) \
			 == ISS_SUB_SCENARIO_VT))
		return 1;
	else
		return 0;
}

/* front vt4 */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_VT4)
{
	if (IS_FRONT_SENSOR(position) &&
			((device->setfile & IS_SETFILE_MASK) \
			 == ISS_SUB_SCENARIO_VT))
		return 1;
	else
		return 0;
}

/* front2 camcording FHD */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT2_CAMCORDING)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if ((position == SENSOR_POSITION_FRONT2) &&
		(fps <= 30) &&
		(resol < SIZE_WHD) &&
		setfile_flag)
		return 1;
	else
		return 0;
}

/* front2 preview FHD */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT2_PREVIEW)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if ((position == SENSOR_POSITION_FRONT2) &&
		(streaming_cnt == 1) &&
		(fps <= 30) &&
		(resol < SIZE_WHD) &&
		(!setfile_flag))
		return 1;
	else
		return 0;
}

/* front2 capture */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT2_CAPTURE)
{
	if ((position == SENSOR_POSITION_FRONT2) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT2_CAMCORDING_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_FRONT2) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT2_CAMCORDING))
		return 1;
	else
		return 0;
}

/* front recording */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if ((position == SENSOR_POSITION_FRONT) &&
		setfile_flag)
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_WHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_VIDEO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_AUTO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_WDR_AUTO));

	if (IS_FRONT_SENSOR(position) &&
		setfile_flag &&
		(resol <= SIZE_8MP_QHD_BDS))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_UHD_30FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_30FPS_WDR_AUTO));

	if (IS_FRONT_SENSOR(position) &&
		setfile_flag &&
		(resol > SIZE_8MP_QHD_BDS))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_60FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_FHD_60FPS) ||
			(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_AUTO));

	if (IS_FRONT_SENSOR(position) &&
		(fps > 30) && (fps <= 60) &&
		setfile_flag)
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_60FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_UHD_60FPS) ||
			(mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_UHD_60FPS_WDR_AUTO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED));

	if (IS_FRONT_SENSOR(position) &&
		(fps > 30) && (fps <= 60) &&
		setfile_flag)
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_120FPS)
{
	u32 mask = (device->setfile & IS_SETFILE_MASK);
	bool setfile_flag = ((mask == ISS_SUB_SCENARIO_FHD_60FPS) ||
			(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_ON) ||
			(mask == ISS_SUB_SCENARIO_FHD_60FPS_WDR_AUTO) ||
			(mask == ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED));

	if (IS_FRONT_SENSOR(position) &&
		(fps > 60) && (fps <= 120) &&
		setfile_flag)
		return 1;
	else
		return 0;
}

/* front  full resolution preview */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW_FULL)
{
	u32 scen = (device->setfile & IS_SCENARIO_MASK) >> IS_SCENARIO_SHIFT;
	bool scenario_flag = (scen == IS_SCENARIO_FULL_SIZE);

	if (IS_FRONT_SENSOR(position) && scenario_flag && streaming_cnt == 1)
		return 1;
	else
		return 0;
}

/* front preview */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_PREVIEW)
{
	if (position == SENSOR_POSITION_FRONT && streaming_cnt == 1)
		return 1;
	else
		return 0;
}

/* front capture */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_FRONT) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		!(static_ctrl->cur_scenario_id == IS_SN_PIP_PREVIEW) &&
		!(static_ctrl->cur_scenario_id == IS_SN_DUAL_PREVIEW))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_FRONT) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT_CAMCORDING))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_60FPS_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_FRONT) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT_CAMCORDING_FHD_60FPS))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_60FPS_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_FRONT) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT_CAMCORDING_UHD_60FPS))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_FHD_120FPS_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_FRONT) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT_CAMCORDING_FHD_120FPS))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_WHD_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_FRONT_SENSOR(position) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT_CAMCORDING_WHD))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_CAMCORDING_UHD_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_FRONT_SENSOR(position) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT_CAMCORDING_UHD))
		return 1;
	else
		return 0;
}

/* front dual sync camcording */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING)
{
	if (IS_FRONT_DUAL(sensor_map) &&
		((device->setfile & IS_SETFILE_MASK) == ISS_SUB_SCENARIO_VIDEO) &&
		stream_cnt > 1)
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_FRONT_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT_DUAL_SYNC_FHD_CAMCORDING))
		return 1;
	else
		return 0;
}

/* front dual sync preview */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_PREVIEW)
{
	if (IS_FRONT_DUAL(sensor_map) &&
		stream_cnt > 1)
		return 1;
	else
		return 0;
}

/* front dual sync capture */
DECLARE_DVFS_CHK_FUNC(IS_SN_FRONT_DUAL_SYNC_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_FRONT_DUAL(sensor_map) &&
		(test_bit(IS_ISCHAIN_REPROCESSING, &device->state)) &&
		(static_ctrl->cur_scenario_id == IS_SN_FRONT_DUAL_SYNC_PREVIEW))
		return 1;
	else
		return 0;
}

/* rear3 capture */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR3_CAPTURE)
{
	if ((position == SENSOR_POSITION_REAR3) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR3_CAMCORDING_FHD_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_REAR3) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_REAR3_CAMCORDING_FHD))
		return 1;
	else
		return 0;
}

/* rear2 capture */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAPTURE)
{
	if ((position == SENSOR_POSITION_REAR2) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_FHD_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_REAR2) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_REAR2_CAMCORDING_FHD))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR2_CAMCORDING_UHD_8K_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_REAR2) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_REAR2_CAMCORDING_UHD_8K))
		return 1;
	else
		return 0;
}

/* rear capture */
DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_REAR) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		!(static_ctrl->cur_scenario_id == IS_SN_PIP_PREVIEW) &&
		!(static_ctrl->cur_scenario_id == IS_SN_DUAL_PREVIEW))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_FHD_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_REAR) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_REAR_CAMCORDING_FHD))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_WHD_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_SENSOR(position) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_REAR_CAMCORDING_WHD))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_SENSOR(position) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_REAR_CAMCORDING_UHD))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_UHD_8K_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_REAR) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_REAR_CAMCORDING_UHD_8K))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_REAR_CAMCORDING_HDR_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if ((position == SENSOR_POSITION_REAR) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		(static_ctrl->cur_scenario_id == IS_SN_REAR_CAMCORDING_HDR))
		return 1;
	else
		return 0;
}

DECLARE_DVFS_CHK_FUNC(IS_SN_VIDEO_HIGH_SPEED_60FPS_CAPTURE)
{
	struct is_dvfs_scenario_ctrl *static_ctrl = device->resourcemgr->dvfs_ctrl.static_ctrl;

	if (IS_REAR_SENSOR(position) &&
		test_bit(IS_ISCHAIN_REPROCESSING, &device->state) &&
		((static_ctrl->cur_scenario_id == IS_SN_VIDEO_HIGH_SPEED_60FPS) ||
		(static_ctrl->cur_scenario_id == IS_SN_REAR_CAMCORDING_UHD_60FPS)))
		return 1;
	else
		return 0;
}

DECLARE_EXT_DVFS_CHK_FUNC(IS_SN_EXT_DUAL)
{
	/* Just follow the current DVFS scenario for the additional external sensor. */
	if (stream_cnt > 1)
		return DVFS_SKIP;
	else
		return DVFS_NOT_MATCHED;
}

DECLARE_EXT_DVFS_CHK_FUNC(IS_SN_EXT_REAR)
{
	if (IS_REAR_SENSOR(position))
		return 1;
	else
		return 0;
}

DECLARE_EXT_DVFS_CHK_FUNC(IS_SN_EXT_FRONT)
{
	if (IS_FRONT_SENSOR(position))
		return 1;
	else
		return 0;
}

DECLARE_EXT_DVFS_CHK_FUNC(IS_SN_EXT_SECURE)
{
	if (position == SENSOR_POSITION_SECURE)
		return 1;
	else
		return 0;
}

int is_hw_dvfs_init(void *dvfs_data)
{
	int ret = 0;
	ulong i;
	struct is_dvfs_ctrl *dvfs_ctrl;

	dvfs_ctrl = (struct is_dvfs_ctrl *)dvfs_data;

	FIMC_BUG(!dvfs_ctrl);

	/* set priority by order */
	for (i = 0; i < ARRAY_SIZE(static_scenarios); i++)
		static_scenarios[i].priority = i;
	for (i = 0; i < ARRAY_SIZE(dynamic_scenarios); i++)
		dynamic_scenarios[i].priority = i;
	for (i = 0; i < ARRAY_SIZE(external_scenarios); i++)
		external_scenarios[i].priority = i;

	dvfs_ctrl->static_ctrl->cur_scenario_id	= -1;
	dvfs_ctrl->static_ctrl->cur_scenario_idx	= -1;
	dvfs_ctrl->static_ctrl->scenarios		= static_scenarios;
	if (static_scenarios[0].scenario_id == IS_SN_DEFAULT)
		dvfs_ctrl->static_ctrl->scenario_cnt	= 0;
	else
		dvfs_ctrl->static_ctrl->scenario_cnt	= ARRAY_SIZE(static_scenarios);

	dvfs_ctrl->dynamic_ctrl->cur_scenario_id	= -1;
	dvfs_ctrl->dynamic_ctrl->cur_scenario_idx	= -1;
	dvfs_ctrl->dynamic_ctrl->cur_frame_tick	= -1;
	dvfs_ctrl->dynamic_ctrl->scenarios		= dynamic_scenarios;
	if (static_scenarios[0].scenario_id == IS_SN_DEFAULT) {
		dvfs_ctrl->dynamic_ctrl->scenario_cnt	= 0;
		dvfs_ctrl->dynamic_ctrl->fixed_scenario_cnt = 0;
	} else {
		dvfs_ctrl->dynamic_ctrl->scenario_cnt	= ARRAY_SIZE(dynamic_scenarios);
		dvfs_ctrl->dynamic_ctrl->fixed_scenario_cnt = 1; /* IS_SN_THERMAL_8K */
	}

	dvfs_ctrl->external_ctrl->cur_scenario_id	= -1;
	dvfs_ctrl->external_ctrl->cur_scenario_idx	= -1;
	dvfs_ctrl->external_ctrl->scenarios		= external_scenarios;
	if (external_scenarios[0].scenario_id == IS_SN_DEFAULT)
		dvfs_ctrl->external_ctrl->scenario_cnt	= 0;
	else
		dvfs_ctrl->external_ctrl->scenario_cnt	= ARRAY_SIZE(external_scenarios);

	return ret;
}

void is_hw_dvfs_get_qos_throughput(u32 *qos_thput)
{
	qos_thput[IS_DVFS_INT_CAM] = PM_QOS_INTCAM_THROUGHPUT;
	qos_thput[IS_DVFS_TNR] = PM_QOS_TNR_THROUGHPUT;
	qos_thput[IS_DVFS_CSIS] = PM_QOS_CSIS_THROUGHPUT;
	qos_thput[IS_DVFS_ISP] = PM_QOS_ISP_THROUGHPUT;
	qos_thput[IS_DVFS_INT] = PM_QOS_DEVICE_THROUGHPUT;
	qos_thput[IS_DVFS_MIF] = PM_QOS_BUS_THROUGHPUT;
	qos_thput[IS_DVFS_CAM] = PM_QOS_CAM_THROUGHPUT;
	qos_thput[IS_DVFS_M2M] = 0;
}

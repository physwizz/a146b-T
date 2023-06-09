/*
 * Samsung Exynos5 SoC series Sensor driver
 *
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef IS_DEVICE_MODULE_BASE_H
#define IS_DEVICE_MODULE_BASE_H

#include <media/v4l2-subdev.h>
#include "is-device-sensor-peri.h"
#include "is-dt.h"

#define PERI_SET_MODULE(mod) (((struct is_device_sensor_peri*)mod->private_data)->module = mod)

/* Core ops */
int sensor_module_init(struct v4l2_subdev *subdev, u32 val);
long sensor_module_ioctl(struct v4l2_subdev *subdev, unsigned int cmd, void *arg);
int sensor_module_g_ctrl(struct v4l2_subdev *subdev, struct v4l2_control *ctrl);
int sensor_module_s_ctrl(struct v4l2_subdev *subdev, struct v4l2_control *ctrl);
int sensor_module_g_ext_ctrls(struct v4l2_subdev *subdev, struct v4l2_ext_controls *ctrls);
int sensor_module_s_ext_ctrls(struct v4l2_subdev *subdev, struct v4l2_ext_controls *ctrls);
int sensor_module_log_status(struct v4l2_subdev *subdev);

/* Video ops */
int sensor_module_s_routing(struct v4l2_subdev *sd, u32 input, u32 output, u32 config);
int sensor_module_s_stream(struct v4l2_subdev *sd, int enable);
int sensor_module_s_param(struct v4l2_subdev *subdev, struct v4l2_streamparm *param);
int sensor_module_s_format(struct v4l2_subdev *subdev,
	v4l2_subdev_config_t *cfg,
	struct v4l2_subdev_format *fmt);

int __init sensor_module_base_probe(struct platform_device *pdev,
	int (*module_callback)(struct device *, struct exynos_platform_is_module *),
	struct is_module_enum **ret_module);
#endif

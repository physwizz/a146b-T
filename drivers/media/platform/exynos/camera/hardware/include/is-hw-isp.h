/*
 * Samsung EXYNOS FIMC-IS (Imaging Subsystem) driver
 *
 * Copyright (C) 2014 Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef IS_HW_ISP_H
#define IS_HW_ISP_H

#include "is-hw.h"
#include "is-interface-ddk.h"

struct is_hw_isp {
	struct is_lib_isp		lib[IS_STREAM_COUNT];
	struct isp_param_set		param_set[IS_STREAM_COUNT];
};

#endif

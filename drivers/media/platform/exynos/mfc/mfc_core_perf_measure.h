/*
 * drivers/media/platform/exynos/mfc/mfc_core_perf_measure.h
 *
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __MFC_CORE_PERF_MEASURE_H
#define __MFC_CORE_PERF_MEASURE_H __FILE__

#include <linux/time.h>
#include <linux/seq_file.h>

#include "mfc_core_reg_api.h"

void mfc_perf_register(struct mfc_core *core);
void mfc_perf_print(struct mfc_core *core);

#ifndef PERF_MEASURE

static inline void mfc_perf_init(struct mfc_core *core) {}
static inline void mfc_perf_cancel_drv_margin(struct mfc_core *core) {}
static inline void mfc_perf_measure_on(struct mfc_core *core) {}
static inline void mfc_perf_measure_off(struct mfc_core *core) {}

#else

void mfc_perf_result(struct seq_file *s, int id);
void __mfc_measure_init(struct mfc_core *core) {}
void __mfc_measure_on(struct mfc_core *core) {}
void __mfc_measure_off(struct mfc_core *core) {}
void __mfc_measure_store(struct mfc_core *core, int diff) {}

static inline void mfc_perf_init(struct mfc_core *core)
{
	core->perf.new_start = 0;
	core->perf.count = 0;
	core->perf.drv_margin = 0;

	__mfc_measure_init(core);
}

static inline void mfc_perf_cancel_drv_margin(struct mfc_core *core)
{
	core->perf.drv_margin = 0;
}

static inline void mfc_perf_measure_on(struct mfc_core *core)
{
	int diff;

	if (core->perf.drv_margin) {
		ktime_get_ts64(&core->perf.end);

		diff = (core->perf.end.tv_sec * 1000000 + (core->perf.end.tv_nsec / NSEC_PER_USEC))
			- (core->perf.begin.tv_sec * 1000000 + (core->perf.begin.tv_nsec / NSEC_PER_USEC));

		mfc_core_debug(1, "IRQ -> NAL_START time(ms) = %03d.%03d\n",
				diff / 1000, diff % 1000);

		core->perf.drv_margin = 0;
	}

	ktime_get_ts64(&core->perf.begin);

	__mfc_measure_on(core);

	core->perf.new_start = 1;
	core->perf.count++;
}

static inline void mfc_perf_measure_off(struct mfc_core *core)
{
	unsigned int diff;

	if ((core->perf.new_start) && (core->perf.count > 0)) {
		__mfc_measure_off(core);

		ktime_get_ts64(&core->perf.end);

		diff = (core->perf.end.tv_sec * 1000000 + (core->perf.end.tv_nsec / NSEC_PER_USEC))
			- (core->perf.begin.tv_sec * 1000000 + (core->perf.begin.tv_nsec / NSEC_PER_USEC));

		__mfc_measure_store(core, diff);

		mfc_core_debug(3, "uDECtype :%d, uENCtype :%d, codectype :%d\n",
				mfc_core_get_dec_frame_type(),
				mfc_core_get_enc_slice_type(),
				(MFC_CORE_READL(MFC_REG_CODEC_TYPE) & 0xffff));

		core->perf.drv_margin = 1;

		ktime_get_ts64(&core->perf.begin);

	}

	core->perf.new_start = 0;
}

#endif

#endif /* __MFC_CORE_PERF_MEASURE_H */

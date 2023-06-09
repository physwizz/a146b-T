/****************************************************************************
 *
 * Copyright (c) 2014 - 2022 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef _FM_WAKELOCK_H
#define _FM_WAKELOCK_H

#include <linux/ktime.h>
#include <linux/device.h>

struct fm_wake_lock {
	struct wakeup_source *ws;
};

static inline void wake_lock_init(struct device *dev, struct wakeup_source **ws, const char *name)
{
	*ws = wakeup_source_register(dev, name);
}

static inline void wake_lock_destroy(struct fm_wake_lock *lock)
{
	wakeup_source_unregister(lock->ws);
}

static inline void wake_lock(struct fm_wake_lock *lock)
{
	__pm_stay_awake(lock->ws);
}

static inline void wake_lock_timeout(struct fm_wake_lock *lock, long timeout)
{
	__pm_wakeup_event(lock->ws, jiffies_to_msecs(timeout));
}

static inline void wake_unlock(struct fm_wake_lock *lock)
{
	__pm_relax(lock->ws);
}

static inline int wake_lock_active(struct fm_wake_lock *lock)
{
	return lock->ws->active;
}
#endif

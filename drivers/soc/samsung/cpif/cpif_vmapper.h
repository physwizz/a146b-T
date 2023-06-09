/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2021 Samsung Electronics.
 *
 */

#ifndef __CPIF_VMAPPER_H__
#define __CPIF_VMAPPER_H__

#include <linux/types.h>
#include <linux/slab.h>

struct cpif_vmap_item {
	u64			vaddr_base;	/* cp address */
	u64			paddr_base;	/* ap physical address */
	u64			item_size;
	atomic_t		ref; /* if zero, this item will be unmapped */
};

/* vmapper contains vaddr that is linear, but paddr sparsed with certain size
 * vmapper contains several items, and each item contains several instances
 * instances have fixed size, whereas each item size may defer due to
 * page allocation
 */
struct cpif_va_mapper {
	u64			va_start; /* va = cp address */
	u64			va_size;
	u64			va_end;
	u64			instance_size; /* size of instance in the item */

	/* vmap table guaranteed to be mapped/unmapped sequentially */
	struct cpif_vmap_item	*item_arr;
	u32			item_arr_size;
	u32			out_idx; /* item to be unmapped */
	u32			in_idx;  /* item mapped recently */
};

struct cpif_va_mapper *cpif_vmap_create(u64 va_start, u64 va_size, u64 instance_size,
		u32 max_num_item);
void cpif_vmap_init(struct cpif_va_mapper *vmap);
void cpif_vmap_free(struct cpif_va_mapper *vmap);
u64 cpif_vmap_map_area(struct cpif_va_mapper *vmap, u64 item_paddr, u64 item_size,
				u64 instance_paddr);
u64 cpif_vmap_unmap_area(struct cpif_va_mapper *vmap, u64 vaddr);
#endif /* __CPIF_VMAPPER_H__ */

/*
 * Institute for System Programming of the Russian Academy of Sciences
 * Copyright (C) 2016 ISPRAS
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, Version 3.
 *
 * This program is distributed in the hope # that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License version 3 for more details.
 *
 * This file also incorporates work covered by POK License.
 * Copyright (c) 2007-2009 POK team
 */

/* Definitions which are used in deployment.c */

#ifndef __JET_PPC_DEPLOYMENT_H__
#define __JET_PPC_DEPLOYMENT_H__

/* 
 * Virtual address where partition's memory starts.
 * 
 * This address is the same for user and kernel.
 */
#define POK_PARTITION_MEMORY_BASE 0x80000000ULL
/*
 * Size of memory in the single chunk.
 * 
 * DEV: This size corresponds to E500MC_PGSIZE_16M constant, used
 * for memory mapping.
 */
#define POK_PARTITION_MEMORY_SIZE 0x1000000ULL 

/* 
 * Description of one user space.
 */
struct ja_ppc_space
{
    /* Physical address of memory chunk. */
    uintptr_t   phys_base;
    /* Size of the memory for code and static data. */
    size_t      size_normal;
    /* Size of the memory for heap */
    size_t      size_heap;
    /* Total size of memory block */
    size_t      size_total;
    /* State of the user stack allocator. */
    uint32_t    ustack_state;
};

/*
 * Array of user space descriptions.
 * 
 * Should be defined in deployment.c.
 */
extern struct ja_ppc_space ja_spaces[];
extern int ja_spaces_n;


/*
 * TLB for memory maping
 */

struct tlb_entry {
    uint32_t virt_addr;
    uint64_t phys_addr;
    unsigned size;
    unsigned permissions;
    unsigned cache_policy;
    unsigned pid;
};

extern struct tlb_entry jet_tlb_entries[];
extern size_t jet_tlb_entries_n;

#endif /* __JET_PPC_DEPLOYMENT_H__ */

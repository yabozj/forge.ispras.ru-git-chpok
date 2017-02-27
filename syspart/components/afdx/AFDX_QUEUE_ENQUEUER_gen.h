/*
 * GENERATED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify the one it generated from (syspart/components/afdx/config.yaml).
 */
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
 */

#ifndef __AFDX_QUEUE_ENQUEUER_GEN_H__
#define __AFDX_QUEUE_ENQUEUER_GEN_H__

#include <memblocks.h>
    #include "afdx.h"
    #include <arinc653/time.h>

    #include <interfaces/preallocated_sender_gen.h>

    #include <interfaces/preallocated_sender_gen.h>
    #include <interfaces/preallocated_sender_gen.h>

typedef struct AFDX_QUEUE_ENQUEUER_state {
    size_t head;
    SYSTEM_TIME_TYPE min_next_time;
    SYSTEM_TIME_TYPE BAG;
    size_t cur_queue_size;
    afdx_buffer * buffer;
    size_t tail;
    size_t max_queue_size;
    size_t prepend_overhead;
    size_t append_overhead;
}AFDX_QUEUE_ENQUEUER_state;

typedef struct {
    char instance_name[16];
    AFDX_QUEUE_ENQUEUER_state state;
    struct {
            struct {
                preallocated_sender ops;
            } portB;
    } in;
    struct {
            struct {
                preallocated_sender *ops;
                self_t *owner;
            } portNetA;
            struct {
                preallocated_sender *ops;
                self_t *owner;
            } portNetB;
    } out;
} AFDX_QUEUE_ENQUEUER;



      ret_t afdx_enqueuer_implementation(AFDX_QUEUE_ENQUEUER *, char *, size_t, size_t, size_t);
      ret_t afdx_enqueuer_flush(AFDX_QUEUE_ENQUEUER *);

      ret_t AFDX_QUEUE_ENQUEUER_call_portNetA_send(AFDX_QUEUE_ENQUEUER *, char *, size_t, size_t, size_t);
      ret_t AFDX_QUEUE_ENQUEUER_call_portNetA_flush(AFDX_QUEUE_ENQUEUER *);
      ret_t AFDX_QUEUE_ENQUEUER_call_portNetB_send(AFDX_QUEUE_ENQUEUER *, char *, size_t, size_t, size_t);
      ret_t AFDX_QUEUE_ENQUEUER_call_portNetB_flush(AFDX_QUEUE_ENQUEUER *);



    void afdx_queue_init(AFDX_QUEUE_ENQUEUER *);

    void afdx_queue_enqueuer_activity(AFDX_QUEUE_ENQUEUER *);


#endif
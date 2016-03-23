/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */

#ifndef __POK_LIBPOK_PORTS_H__
#define __POK_LIBPOK_PORTS_H__

#include <config.h>

#include <core/dependencies.h>

#include <types.h>
#include <errno.h>

typedef enum
{
   POK_PORT_DIRECTION_IN   = 1,
   POK_PORT_DIRECTION_OUT  = 2
} pok_port_directions_t;

typedef pok_queuing_discipline_t pok_port_queuing_discipline_t;

typedef enum
{
   POK_PORT_KIND_QUEUEING  = 1,
   POK_PORT_KIND_SAMPLING  = 2,
   POK_PORT_KIND_VIRTUAL   = 2,
   POK_PORT_KIND_INVALID   = 10
} pok_port_kinds_t;

#ifdef POK_NEEDS_PORTS_QUEUEING
/* Queueing port functions */
typedef struct
{
   pok_port_size_t      nb_message;
   pok_port_size_t      max_nb_message;
   pok_port_size_t      max_message_size;
   pok_port_direction_t direction;
   uint8_t              waiting_processes;
} pok_port_queuing_status_t;

typedef struct
{
    const char *name;
    pok_port_size_t message_size;
    pok_port_size_t max_nb_message;
    pok_port_direction_t direction;
    pok_port_queuing_discipline_t discipline;
} pok_port_queuing_create_arg_t;




/*pok_ret_t pok_port_queueing_receive (const pok_port_id_t                      id, 
                                     const int64_t                            timeout, 
                                     const pok_port_size_t                    maxlen, 
                                     void*                                    data, 
                                     pok_port_size_t*                         len);*/

/*pok_ret_t pok_port_queueing_send (const pok_port_id_t                         id, 
                                  const void*                                 data, 
                                  const pok_port_size_t                       len, 
                                  const int64_t                               timeout);*/

/*#define pok_port_queueing_status(id,status) \
        pok_syscall2(POK_SYSCALL_MIDDLEWARE_QUEUEING_STATUS,(uint32_t)id,(uint32_t)status)*/
/*
 * Similar to:
 * pok_ret_t pok_port_queueing_status (const pok_port_id_t                    id,
 *                                     const pok_port_queueing_status_t*      status);
 */


/*#define pok_port_queueing_id(name,id) \
        pok_syscall2(POK_SYSCALL_MIDDLEWARE_QUEUEING_ID,(uint32_t)name,(uint32_t)id)*/
/*
 * Similar to:
 * pok_ret_t pok_port_queueing_id     (char*                                     name,
 *                                     pok_port_id_t*                            id);
 */
#endif

#ifdef POK_NEEDS_PORTS_SAMPLING
/* Sampling port functions */

typedef struct
{
   pok_port_size_t      size;
   pok_port_direction_t direction;
   uint64_t             refresh;
   bool_t               validity;
}pok_port_sampling_status_t;


/*pok_ret_t pok_port_sampling_create (char*                                     name,
                                    const pok_port_size_t                     size,
                                    const pok_port_direction_t                direction, 
                                    const uint64_t                            refresh,
                                    pok_port_id_t*                            id);*/

/*pok_ret_t pok_port_sampling_write (const pok_port_id_t                        id,
                                   const void*                                data,
                                   const pok_port_size_t                      len);*/

/*pok_ret_t pok_port_sampling_read (const pok_port_id_t                      id,
                                  void*                                    message,
                                  pok_port_size_t*                         len,
                                  bool_t*                                  valid);*/

/*#define pok_port_sampling_id(name,id) \
        pok_syscall2(POK_SYSCALL_MIDDLEWARE_SAMPLING_ID,(uint32_t)name,(uint32_t)id)*/
/*
 *    Similar to
 *  pok_ret_t pok_port_sampling_id     (char*                                     name,
 *                                      pok_port_id_t*                            id);
 */

/*#define pok_port_sampling_status(id,status) \
        pok_syscall2(POK_SYSCALL_MIDDLEWARE_SAMPLING_STATUS,(uint32_t)id,(uint32_t)status)*/
/*
 * Similar to:
 * pok_ret_t pok_port_sampling_status (const pok_port_id_t                       id,
 *                                     const pok_port_sampling_status_t*               status);
 */
#endif

#include <core/syscall.h>

#define pok_port_queuing_create(name, _message_size, _max_nb_message, _direction, _discipline, id) \
   ({pok_port_queuing_create_arg_t args = {\
         .message_size = _message_size,\
         .max_nb_message = _max_nb_message,\
         .direction = _direction,\
         .discipline = _discipline}; \
         pok_port_queuing_create_packed(name, &args, id);\
   })

#endif

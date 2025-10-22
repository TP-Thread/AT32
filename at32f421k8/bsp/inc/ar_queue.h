/**
 ******************************************************************************
 * @file    ar_queue.h
 * @author  TP-Thread
 * @brief   ar queue related definitions.
 ******************************************************************************
 */
#ifndef AR_QUEUE_H
#define AR_QUEUE_H

#include "main.h"
#include "cmsis_os.h"

/* Queue ID list */
typedef enum
{
    RR_QUEUE_STATE_MACHINE_ID = 0,
    RR_QUEUE_UART_COMM_ID,
    RR_QUEUE_WIFI_ID,
    RR_QUEUE_DRIVER_ID,
    RR_QUEUE_LOG_ID,
    RR_QUEUE_IMU_ID,
    RR_QUEUE_MAX_ID
} QueueID_e;

typedef struct _RRQueue_
{
    const signed char* const ps8Name;
    uint32_t uint32_tQueueLength;
    xQueueHandle pQueueHandle;
} RRQueue_st;

#define QUEUE_STATE_MACHIN_DEPTH (32)
#define QUEUE_UART_COMM_DEPTH    (16)
#define QUEUE_WIFI_DEPTH         (16)
#define QUEUE_DRIVER_DEPTH       (32)
#define QUEUE_LOG_DEPTH          (4)

void rr_queue_init(void);
int32_t rr_queue_send(uint32_t uint32_tQid, uint16_t uint16_tMsgId, uint16_t uint16_tSubMsgId, uint32_t uint32_tParam, uint32_t uint32_tTicksToWait);
int32_t rr_queue_send_to_front(uint32_t uint32_tQid, uint16_t uint16_tMsgId, uint16_t uint16_tSubMsgId, uint32_t uint32_tParam, uint32_t uint32_tTicksToWait);
int32_t rr_queue_send_isr(uint32_t uint32_tQid, uint16_t uint16_tMsgId, uint16_t uint16_tSubMsgId, uint32_t uint32_tParam);
int32_t rr_queue_send_to_front_isr(uint32_t uint32_tQid, uint16_t uint16_tMsgId, uint16_t uint16_tSubMsgId, uint32_t uint32_tParam);
int32_t rr_queue_receive(uint32_t uint32_tQid, void* pvBuffer, uint32_t uint32_tTicksToWait);
xQueueHandle rr_queue_handle(uint32_t uint32_tQid);
int32_t rr_queue_messages_waiting(uint32_t uint32_tQid, int int32_tInIsr);
int32_t rr_queue_is_full(uint32_t uint32_tQid, int int32_tInIsr);

void trace_os_queue_send_failed(void);
void trace_os_queue_send_isr_failed(void);
void monitor_queue_send_fail(void);

#endif /* AR_QUEUE_H */

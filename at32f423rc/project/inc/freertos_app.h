/* add user code begin Header */
/**
  ******************************************************************************
  * File Name          : freertos_app.h
  * Description        : Code for freertos applications
  */
/* add user code end Header */
  
#ifndef FREERTOS_APP_H
#define FREERTOS_APP_H

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"

/* private includes -------------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* exported types -------------------------------------------------------------*/
/* add user code begin exported types */

/* add user code end exported types */

/* exported constants --------------------------------------------------------*/
/* add user code begin exported constants */

/* add user code end exported constants */

/* exported macro ------------------------------------------------------------*/
/* add user code begin exported macro */

/* add user code end exported macro */

/* task handler */
extern TaskHandle_t TASK_UART_handle;
extern TaskHandle_t TASK_DRIVER_handle;
extern TaskHandle_t TASK_STATE_handle;
/* declaration for task function */
void task_uart(void *pvParameters);
void task_driver(void *pvParameters);
void task_state(void *pvParameters);

/* queue handler */
extern QueueHandle_t QUART_handle;
extern QueueHandle_t QDRIVER_handle;
extern QueueHandle_t QSTATE_handle;

/* add user code begin 0 */

/* add user code end 0 */

void freertos_task_create(void);
void freertos_queue_create(void);
void wk_freertos_init(void);

/* add user code begin 1 */

/* add user code end 1 */

#endif /* FREERTOS_APP_H */

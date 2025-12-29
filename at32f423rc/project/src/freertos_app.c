/* add user code begin Header */
/**
  ******************************************************************************
  * File Name          : freertos_app.c
  * Description        : Code for freertos applications
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "freertos_app.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/* task handler */
TaskHandle_t TASK_UART_handle;
TaskHandle_t TASK_DRIVER_handle;
TaskHandle_t TASK_STATE_handle;

/* queue handler */
QueueHandle_t QUART_handle;
QueueHandle_t QDRIVER_handle;
QueueHandle_t QSTATE_handle;

/* Idle task control block and stack */
static StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];
static StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];

static StaticTask_t idle_task_tcb;
static StaticTask_t timer_task_tcb;

/* External Idle and Timer task static memory allocation functions */
extern void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t ** ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
extern void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer, StackType_t ** ppxTimerTaskStackBuffer, uint32_t * pulTimerTaskStackSize );

/*
  vApplicationGetIdleTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
  equals to 1 and is required for static memory allocation support.
*/
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t ** ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &idle_task_tcb;
  *ppxIdleTaskStackBuffer = &idle_task_stack[0];
  *pulIdleTaskStackSize = (uint32_t)configMINIMAL_STACK_SIZE;
}
/*
  vApplicationGetTimerTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
  equals to 1 and is required for static memory allocation support.
*/
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer, StackType_t ** ppxTimerTaskStackBuffer, uint32_t * pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &timer_task_tcb;
  *ppxTimerTaskStackBuffer = &timer_task_stack[0];
  *pulTimerTaskStackSize = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}

/* add user code begin 1 */

/* add user code end 1 */

/**
  * @brief  initializes all task.
  * @param  none
  * @retval none
  */
void freertos_task_create(void)
{
  /* create TASK_UART task */
  xTaskCreate(task_uart,
              "TASK_UART",
              512,
              NULL,
              3,
              &TASK_UART_handle);

  /* create TASK_DRIVER task */
  xTaskCreate(task_driver,
              "TASK_DRIVER",
              256,
              NULL,
              2,
              &TASK_DRIVER_handle);

  /* create TASK_STATE task */
  xTaskCreate(task_state,
              "TASK_STATE",
              512,
              NULL,
              2,
              &TASK_STATE_handle);
}

/**
  * @brief  initializes all queue.
  * @param  none
  * @retval none
  */
void freertos_queue_create(void)
{
  /* Create the QUART, storing the returned handle in the xQueue variable. */
  QUART_handle = xQueueCreate(16, sizeof(uint16_t));

  /* Create the QDRIVER, storing the returned handle in the xQueue variable. */
  QDRIVER_handle = xQueueCreate(32, sizeof(uint16_t));

  /* Create the QSTATE, storing the returned handle in the xQueue variable. */
  QSTATE_handle = xQueueCreate(32, sizeof(uint16_t));
}

/**
  * @brief  freertos init and begin run.
  * @param  none
  * @retval none
  */
void wk_freertos_init(void)
{
  /* add user code begin freertos_init 0 */

  /* add user code end freertos_init 0 */

  /* enter critical */
  taskENTER_CRITICAL();

  freertos_queue_create();
  freertos_task_create();
	
  /* add user code begin freertos_init 1 */

  /* add user code end freertos_init 1 */

  /* exit critical */
  taskEXIT_CRITICAL();

  /* start scheduler */
  vTaskStartScheduler();
}

/**
  * @brief TASK_UART function.
  * @param  none
  * @retval none
  */
__WEAK void task_uart(void *pvParameters)
{
  /* add user code begin task_uart 0 */

  /* add user code end task_uart 0 */

  /* add user code begin task_uart 2 */

  /* add user code end task_uart 2 */

  /* Infinite loop */
  while(1)
  {
  /* add user code begin task_uart 1 */

     vTaskDelay(1);

  /* add user code end task_uart 1 */
  }
}

/**
  * @brief TASK_DRIVER function.
  * @param  none
  * @retval none
  */
__WEAK void task_driver(void *pvParameters)
{
  /* add user code begin task_driver 0 */

  /* add user code end task_driver 0 */

  /* add user code begin task_driver 2 */

  /* add user code end task_driver 2 */

  /* Infinite loop */
  while(1)
  {
  /* add user code begin task_driver 1 */

     vTaskDelay(1);

  /* add user code end task_driver 1 */
  }
}

/**
  * @brief TASK_STATE function.
  * @param  none
  * @retval none
  */
__WEAK void task_state(void *pvParameters)
{
  /* add user code begin task_state 0 */

  /* add user code end task_state 0 */

  /* add user code begin task_state 2 */

  /* add user code end task_state 2 */

  /* Infinite loop */
  while(1)
  {
  /* add user code begin task_state 1 */

     vTaskDelay(1);

  /* add user code end task_state 1 */
  }
}

/* add user code begin 2 */

/* add user code end 2 */


/**
 ******************************************************************************
 * @file    ar_irq.c
 * @author  TP-Thread
 * @brief   中断回调.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ar_irq.h"
#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
extern osMessageQueueId_t QDRIVERHandle;

extern uint32_t u16ADCRaw[2];

uint8_t g_u8Key1ITFlag = 0;
uint8_t g_u8Key2ITFlag = 0;

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  外部中断回调函数
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case KEY1_Pin:
        g_u8Key1ITFlag = 1;
        break;
    case KEY2_Pin:
        g_u8Key2ITFlag = 1;
        break;
    default:
        break;
    }
}

/**
 * @brief  串口接收完成中断回调函数
 */
void HAL_UART_Callback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        uint8_t ch;

        ch = (uint16_t)READ_REG(huart->Instance->DR);
        WRITE_REG(huart->Instance->DR, ch);
    }

    else if (huart->Instance == USART3)
    {
        /* 处理USART3接收到的数据 */
    }
}

/**
 * @brief  ADC转换完成回调函数
 * @param  hadc: ADC句柄
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    /*定时器DMA启动多通道转换*/
    static uint32_t vslider_raw = 0;

    vslider_raw = u16ADCRaw[0];
    // vrefint_raw = u16ADCRaw[1];

    // printf("vrefint_raw:%d, vslider_raw:%d\r\n", vrefint_raw, vslider_raw);
}

/**
 * @brief  定时器回调函数
 * @param  htim: 定时器句柄
 * @retval None
 */
void HAL_TIM_Callback(TIM_HandleTypeDef *htim)
{
    static arMsg_t tMsg;
    tMsg.u16MsgId = 0;
    tMsg.u16SubMsgId = 0;
    tMsg.u32Param = 0;

    static uint8_t u8TimBase10Ms = 0;
    static uint8_t u8TimBase10MForKey1 = 0;
    static uint8_t u8TimBase10MForKey2 = 0;

    if (htim->Instance == TIM7)
    {
        u8TimBase10Ms++;

        /* 10ms 消息队列发送 */
        tMsg.u16MsgId = AR_MSG_DRIVER_10MS_ID;
        osMessageQueuePut(QDRIVERHandle, &tMsg, 0, 0);

        /* 50ms 消息队列发送 */
        if (5 == u8TimBase10Ms)
        {

            tMsg.u16MsgId = AR_MSG_DRIVER_50MS_ID;
            osMessageQueuePut(QDRIVERHandle, &tMsg, 0, 0);
        }

        /* 100ms 消息队列发送 */
        if (10 == u8TimBase10Ms)
        {
            tMsg.u16MsgId = AR_MSG_DRIVER_100MS_ID;
            osMessageQueuePut(QDRIVERHandle, &tMsg, 0, 0);
            u8TimBase10Ms = 0; // 重置计数器
        }

        /* 10ms 按键事件处理 */
        if (g_u8Key1ITFlag) // 按键1中断标志
        {
            u8TimBase10MForKey1++;
            if (3 == u8TimBase10MForKey1) // 30ms 软件延迟防抖
            {
                u8TimBase10MForKey1 = 0;
                g_u8Key1ITFlag = 0;

                if (arKey1ValGet())
                {
                    arKey1EventSet(AR_KEY_DOWN_EVENT);
                }
                else
                {
                    arKey1EventSet(AR_KEY_UP_EVENT);
                }
            }
        }
        if (g_u8Key2ITFlag) // 按键2中断标志
        {
            u8TimBase10MForKey2++;
            if (3 == u8TimBase10MForKey2) // 30ms 软件延迟防抖
            {
                u8TimBase10MForKey2 = 0;
                g_u8Key2ITFlag = 0;

                if (arKey2ValGet())
                {
                    arKey2EventSet(AR_KEY_DOWN_EVENT);
                }
                else
                {
                    arKey2EventSet(AR_KEY_UP_EVENT);
                }
            }
        }
    }
}

/**
 ******************************************************************************
 * @file    ar_key.c
 * @author  TP-Thread
 * @brief   按键.
 ******************************************************************************
 */

/* Includes -----------------------------------------------------------------*/
#include "ar_key.h"
#include "cmsis_os.h"

/* Private variables --------------------------------------------------------*/
extern osMessageQueueId_t QSTATEHandle;

arKeyEvent_e g_eKey1Event = AR_KEY_NONE_EVENT;
arKeyEvent_e g_eKey2Event = AR_KEY_NONE_EVENT;

/* Private functions --------------------------------------------------------*/

void arKey1EventSet(arKeyEvent_e eKeyEvent)
{
    if (eKeyEvent < AR_KEY_MAX_EVENT)
    {
        g_eKey1Event = eKeyEvent;
    }
}

arKeyEvent_e arKey1EventGet(void)
{
    return g_eKey1Event;
}

void arKey2EventSet(arKeyEvent_e eKeyEvent)
{
    if (eKeyEvent < AR_KEY_MAX_EVENT)
    {
        g_eKey2Event = eKeyEvent;
    }
}

arKeyEvent_e arKey2EventGet(void)
{
    return g_eKey2Event;
}

/**
 * @brief  检查按键状态
 * @retval None
 */
void arCheckKey(void)
{
    static arKeyEvent_e eKey1LastEvent = AR_KEY_NONE_EVENT;
    static arKeyEvent_e eKey2LastEvent = AR_KEY_NONE_EVENT;

    static arMsg_t tMsg;
    tMsg.u16MsgId = AR_MSG_STATE_MACHINE_KEY_ID;
    tMsg.u16SubMsgId = 0;
    tMsg.u32Param = 0;

    // 检查按键1状态，在按键持续按下期间，只在按下的瞬间触发一次事件
    if (AR_KEY_DOWN_EVENT == arKey1EventGet())
    {
        if (AR_KEY_DOWN_EVENT != eKey1LastEvent)
        {
            printf("Key1 Down");

            tMsg.u16SubMsgId = AR_SUBMSG_STATE_KEY_KEY1_DOWN_ID;
            osMessageQueuePut(QSTATEHandle, &tMsg, 0, 0);
        }
    }
    eKey1LastEvent = arKey1EventGet();

    // 检查按键2状态，在按键持续按下期间，只在按下的瞬间触发一次事件
    if (AR_KEY_DOWN_EVENT == arKey2EventGet())
    {
        if (AR_KEY_DOWN_EVENT != eKey2LastEvent)
        {
            LED_G_Toggle();
        }
    }
    eKey2LastEvent = arKey2EventGet();
}

/**
 ******************************************************************************
 * @file    ar_state_ready.c
 * @author  TP-Thread
 * @brief   for statemachine ready.
 ******************************************************************************
 */
#include "ar_state_ready.h"

arStateId_e readyEnter(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore)
{
    arStateId_e NextState = SID_Ready;

    return NextState;
}

void readyLeave(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore)
{
    printf("startup_ready ");
}

arStateId_e readyRun(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore)
{
    arStateId_e NextState = SID_Ready;

    return NextState;
}

arEventId_e readyKeyEvent(SUBMSG_STATEMACHINE_KEY_ID_e eSubMsgKey, uint32_t u32KeyPara)
{
    arEventId_e eEventId = RE_INVALID;
    switch (eSubMsgKey)
    {
    case AR_SUBMSG_STATE_KEY_KEY1_DOWN_ID:
        LED_B_Toggle();
        eEventId = RE_Key_Power_Short;
        break;
    case AR_SUBMSG_STATE_MACHINE_KEY_SWITCH_SHORT_ID:
        eEventId = RE_Key_Switch_Short;
        break;
    default:
        break;
    }
    return eEventId;
}

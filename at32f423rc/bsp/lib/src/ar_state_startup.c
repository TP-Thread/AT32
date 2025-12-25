/**
 ******************************************************************************
 * @file    ar_state_startup.c
 * @author  TP-Thread
 * @brief   for statemachine startup.
 ******************************************************************************
 */
#include "ar_state_startup.h"

arStateId_e startupEnter(arEvent_t *ptEvent, arStateStore_t *ptStateStore)
{
	arStateId_e eStateNext = SID_Run;
    ptStateStore->eCurStateId = SID_Ready;
	
	return eStateNext;
}

// 没起作用
void startupLeave(arEvent_t *ptEvent, arStateStore_t *ptStateStore)
{
    printf("startup_leave ");
}

arStateId_e startupRun(arEvent_t *ptEvent, arStateStore_t *ptStateStore)
{
    arStateId_e NextState = SID_StartUp;
    // arEventId_e eEventId = ptEvent->eEventId;

    // switch (eEventId)
    // {
    // case RE_StartUp_Waitfor_Run: // 不在桩上开机
    //     if (ptStateStore->bMicroSwitchLock == true)
    //     {
    //         if (stNVRAMData.CustomOption.IsAutoRun == DP_INDEX_AUTORUN_STANDLOCK_ALL_ON)
    //         {
    //             NextState = SID_Ready;
    //         }
    //         else
    //         {
    //             NextState = SID_Run;
    //         }
    //     }
    //     else
    //     {
    //         // TODO：chengyi    //BB_Clean_SetStartWay(BB_STARTWAY_UNLOCK);
    //         NextState = SID_Run;
    //         ptStateStore->bMicroSwitchLock = false;
    //     }
    //     break;
    // case RE_StartUp_Waitfor_TimeOut:
    //     NextState = SID_Error;
    //     break;
    // default:
    //     break;
    // }

    return NextState;
}

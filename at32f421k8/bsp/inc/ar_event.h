/**
 ******************************************************************************
 * @file    ar_event.h
 * @author  TP-Thread
 * @brief   Event for StateMachine.
 ******************************************************************************
 */
#ifndef AR_EVENT_H_
#define AR_EVENT_H_

#include "main.h"

typedef enum
{
    RE_INVALID = 0,

    /* For Key events */
    RE_Key_Power_Short = 1,
    RE_Key_Switch_Short,
    RE_Key_AutoClean_Short,
    RE_Key_Power_Long_2s,
    RE_Key_Power_Long_10s,
    RE_Key_AutoClean_Long_10s, // 非桩自清洁
    RE_Key_AutoClean_Long_2s,
    RE_Key_Switch_Long_10s,
    RE_Key_Switch_Long_2s,
    RE_Key_Volume_Long_5s, // roborock add
    RE_Key_Volume_Short,
    RE_CombineKey_AutoClean_Switch_For_Ver,
    RE_CombineKey_AutoClean_Switch_For_AUTO_BIST,
    RE_CombineKey_AutoClean_Switch_For_MANUAL_BIST,
    RE_CombineKey_AutoClean_Switch_For_LanguageShow,
    RE_CombineKey_AutoClean_Switch_For_AudioAfterMarket,
    RE_CombineKey_AutoClean_Switch_For_ResetWifi,
    RE_CombineKey_AutoClean_Power_For_RecoveryDefault,
    RE_CombineKey_Power_Switch_For_IMU_Calibrate,
    RE_CombineKey_Power_Switch_For_BIST_CHECK,
    RE_Key_Slience_Short,
#if (RR_PROJECT_YAOGUANGALL)
    RE_Key_Switch_Down,
    RE_Key_Power_Down,
    RE_Key_Clean_Down,
#endif
    RE_Key_HotWater_Down,
    RE_Key_HotWater_Up,
    RE_Key_HotWater_TimeOut10s,

    RE_Key_Power_Long_5s,

    /* Battery events*/
    RE_Battery_SOC_Change = 1001,
    RE_Battery_Discharge_LowTemp,
    RE_Battery_Discharge_HighTemp,
    RE_Battery_Charge_OverCurrtent,
    RE_Battery_Charge_HighTemp,
    RE_Battery_Charge_LowTemp,
    RE_Battery_FakeCharge_OverTime,
    RE_Battery_Charge_VoltageAbnormal,
    RE_Battery_Undervoltage,
    RE_Battery_SOC_Lower_30,
    RE_Battery_SOC_Lower_25,
    RE_Battery_SOC_Lower_20,
    RE_Battery_SOC_Lower_15,
    RE_Battery_SOC_Lower_10,
    RE_Battery_SOC_Lower_5,
    RE_Battery_SOC_Upto_15,
    RE_Battery_SOC_Upto_5,
    RE_Battery_OTA_Restart_OK,
    RE_Battery_OTA_Restart_Fail,
    RE_Battery_OTA_GetInfo_OK,
    RE_Battery_OTA_GetInfo_Fail,
    RE_Battery_OTA_Init_OK,
    RE_Battery_OTA_Init_Fail,
    RE_Battery_OTA_Program_OK,
    RE_Battery_OTA_Program_Fail,
    RE_Battery_OTA_Verify_OK,
    RE_Battery_OTA_Verify_Fail,
    RE_Battery_OTA_GO_OK,
    RE_Battery_OTA_GO_Fail,
    RE_Battery_OTA_Sync_OK,
    RE_Battery_OTA_Sync_Fail,
    RE_Battery_Discharge_Low_Temper_Recover,
    RE_Battery_Discharge_High_Temper_Recover,
    RE_Battery_Connect,
    RE_Battery_DisConnect,
    RE_Battery_SOC_Enough_SilentDry,

    /* Fan events*/
    RE_Fan_BlockedHole = 2001, //
    RE_Fan_BlockedRotor,
    RE_Fan_OverVoltage,
    RE_Fan_UnderVoltage,
    RE_Fan_HWOverCurrent,
    RE_Fan_SWOverCurrent,
    RE_Fan_HighTemperature,
    RE_Fan_Comm_Fail,
    RE_Fan_Power_0,
    RE_Fan_OTA_Restart_OK,
    RE_Fan_OTA_Restart_Fail,
    RE_Fan_OTA_GetInfo_OK,
    RE_Fan_OTA_GetInfo_Fail,
    RE_Fan_OTA_Init_OK,
    RE_Fan_OTA_Init_Fail,
    RE_Fan_OTA_Program_OK,
    RE_Fan_OTA_Program_Fail,
    RE_Fan_OTA_Verify_OK,
    RE_Fan_OTA_Verify_Fail,
    RE_Fan_OTA_GO_OK,
    RE_Fan_OTA_GO_Fail,
    RE_Fan_OTA_Sync_OK,
    RE_Fan_OTA_Sync_Fail,

    /* Dock events*/
    RE_Dock_Connect = 3001,
    RE_Dock_DisConnect,
    RE_Dock_Comm_OK,
    RE_Dock_Comm_Err,
    RE_Dock_ResistCheck,
    RE_Dock_OTA_Start_ACK,
    RE_Dock_OTA_Program_ACK,
    RE_Dock_OTA_Verify_ACK,
    RE_Dock_DisCharge,
    RE_Dock_Charge_Voltage_Error,

    /* BrushBoard events*/
    RE_Brush_Board_OTA_Start_ACK = 3501,
    RE_Brush_Board_OTA_Program_ACK,
    RE_Brush_Board_OTA_Verify_ACK,
    RE_Brush_Board_Connect,
    RE_Brush_Board_Disconnect,

    /* App events*/
    RE_App_Set_Switch_Mode = 4001,
    RE_App_Set_AutoClean_OFF,
    RE_App_Set_AutoClean_Pause,
    RE_App_Set_AutoClean_Continue,
    RE_App_Set_NormalAutoClean_ON,
    RE_App_Set_DeepAutoClean_ON,
    RE_App_Set_NormalAutoClean_Continue,
    RE_App_Set_DeepAutoClean_Continue,
    RE_App_Set_CleanDry_ON,
    RE_App_Set_CleanDry_OFF,
    RE_App_Set_OTA,
    RE_App_Start_General_OTA,
    RE_App_End_General_OTA,
    RE_App_End_General_OTA_Silent,
    RE_App_Set_Submodule_OTA,
    RE_App_OTA_Package_Ready,
    RE_App_Watchdog_starve,
    RE_App_Audio_Done,
    RE_App_Run_Start,
    RE_App_Run_Stop,
    RE_App_Switch_CleanDry_Type,
    RE_App_Switch_Run_Type,

    /* MicroSwitch events*/
    RE_MicroSwitch_Lock = 5001,
    RE_MicroSwitch_unLock,

    /* wait run events*/
    RE_WaitRun_Start = 5501,
    /* Watertank events*/
    RE_CleanWaterTank_Empty = 6001,

    RE_DirtyWaterTank_Right_Full,

    RE_CleanWaterTank_NoEmpty,

    RE_DirtyWaterTank_Right_NoFull,

    RE_DirtyWaterTank_Least_One_Full,
    RE_Flat_Detect,
    RE_Flat_Recover,
    RE_Flat_STAND,

    RE_CleanWaterTank_Error,
    RE_CleanWaterTank_NotPlace,
    RE_CleanWaterTank_InPlace_And_Empty,
    RE_CleanWaterTank_InPlace_And_Middle,
    RE_CleanWaterTank_InPlace_And_Full,

    RE_DirtyWaterTank_NotPlace,
    RE_DirtyWaterTank_InPlace,

    /* Brush events*/
    RE_Brush_OverCurrent = 7001,
    RE_Brush_OverTemperature,
    RE_Brush_Current_Normal,

    /***自己根据时间计算出来的event***/
    RE_AutoClean_OneStep_TimeOut = 8001,
    RE_CleanWaterTank_Empty_Start,
    RE_CleanWaterTank_Empty_5s,
    RE_CleanWaterTank_Empty_10s,
    RE_CleanWaterTank_Empty_20s,
    RE_CleanWaterTank_Empty_2min,
    RE_CleanWaterTank_Empty_2min30s,
    RE_CleanWaterTank_Empty_5min,

    RE_CleanWaterTank_Empty_RunStart,
    RE_CleanWaterTank_Empty_HoldStep1,
    RE_CleanWaterTank_Empty_HoldStep2,
    RE_CleanWaterTank_Empty_HoldStep3,
    RE_CleanWaterTank_Empty_HoldSelfClean,
    RE_CleanWaterTank_Not_Empty,

    RE_DirtyWaterTank_Full_100ms,
    RE_DirtyWaterTank_Full_3s,

    RE_Ready_TimeOut_5min,
    RE_Run_TimeOut_5s,
    RE_Auto_Submode_Change,

    RE_Charge_TimeOut_5min,
    RE_Charge_TimeOut_1min,
    RE_Charge_TimeOut_8hour,
    RE_Charge_AbnormalCharger,

    RE_CleanDry_TimeOut_5min,
    RE_CleanDry_TimeOut_1min,
    RE_CleanDry_TimeOut_5min_Ptc,
    RE_CleanDry_TimeOut_Finish,
    RE_CleanDry_OneStep_TimeOut,

    RE_AutoCleanPause_TimeOut_5min,
    RE_Error_TimeOut_5min,

    RE_Sleep_TimeOut_1hour,

    RE_AT_No_Operation_20min,

    RE_FAST_CLEANDRY_5MIN_BRUSH_RUN_Begin,
    RE_FAST_CLEANDRY_5MIN_BRUSH_RUN_Stop,
    RE_SILENT_CLEANDRY_10MIN_BRUSH_RUN_Begin,
    RE_SILENT_CLEANDRY_10MIN_BRUSH_RUN_Stop,

    RE_Dock_Fan_Detect_NoCurrent,
    RE_Dock_Fan_Detect_NoCurrent_Retry_PowerOn_After_5s,
    RE_Dock_Fan_Detect_NoCurrent_Retry_PowerOn_After_3_Times,

    RE_Fan_BlockedHole_TimeOut_1s,
    RE_Fan_BlockedRotor_TimeOut_1s,
    RE_Fan_OverVoltage_TimeOut_1s,
    RE_Fan_UnderVoltage_TimeOut_1s,
    RE_Fan_HWOverCurrent_TimeOut_1s,
    RE_Fan_SWOverCurrent_TimeOut_1s,
    RE_Fan_HighTemperature_TimeOut_1s,
    RE_Fan_Comm_Fail_TimeOut_1s,

    /************for startup*********************/
    RE_StartUp_Waitfor_Run = 9001,
    RE_StartUp_Waitfor_Charge,
    RE_StartUp_Waitfor_AT,
    RE_StartUp_Waitfor_OTA,
    RE_StartUp_Waitfor_TimeOut,
    RE_StartUp_Waitfor_Autoclean,

    RE_Ready_Waitfor_AT,
    RE_Charge_Waitfor_AT,
    RE_Run_Waitfor_AT,
    RE_Error_Waitfor_AT,
    RE_Sleep_Waitfor_AT,
    RE_PowerSupplySleep_Waitfor_AT,
    RE_ChargeSleep_Waitfor_AT,
    RE_Recv_AT_CMD,

    /***********for bist***********************/
    RE_Bist_Waitfor_5min = 10001,
    RE_Bist_Waitfor_10min,
    RE_Bist_Waitfor_5s,
    RE_Bist_Wait,
    RE_Bist_Wheel_Success,
    BIST_WIFI_TEST_SUCCESS,
    BIST_WIFI_TEST_FAIL,

    /*********** for language ****************/
    RE_Language_Waitfor_1min = 13001,
    RE_Language_Waitfor_3s,

    /*********** for run**********************/
    RE_DirtyCheck_Run_Into_DeepDirty = 13501,
    RE_DirtyCheck_Run_Outof_DeepDirty,

    RE_Boiler_TemperatureState_Change = 14000,
    RE_Boiler_SlowDown_Update,
    RE_Boiler_TriggerNTCPortect,
    RE_Boiler_ReleaseNTCPortect,
    RE_Boiler_TriggerEmptyPortect,
    RE_Boiler_ReleaseEmptyPortect,

    /*********** for life test****************/
    RE_Life_Test_Run_One_Step_TimeOut = 15001,
    RE_Life_Test_Stop_TimeOut_2s,
    RE_Life_Test_One_Step_TimeOut // 新版寿命测试单步完成
} arEventId_e;

/*************hepeng add for bist********/
typedef struct
{
    arEventId_e eEventId;
    uint32_t Param;
} arEvent_t;

#endif /* AR_EVENT_H */

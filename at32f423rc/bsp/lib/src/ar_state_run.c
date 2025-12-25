/**
 ******************************************************************************
 * @file    ar_state_run.c
 * @author  TP-Thread
 * @brief   for statemachine run.
 ******************************************************************************
 */
#include "ar_state_run.h"

#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
#define HOTER_WATER_STEP1_100MS_NUM PUMP_HOTWATER_STEP1_TIME / (100)
#define HOTER_WATER_STEP2_100MS_NUM PUMP_HOTWATER_STEP2_TIME / (100)
static uint64_t s_u64HotWaterRunCnt100ms = 0;
#endif

static uint64_t s_u64PumpFanSwitchModeTime = INVALID_TIME;
static uint8_t s_u8PumpModeStep = 0;
static uint8_t s_u8LastPumpModeStep = 0;
static uint16_t s_u16CloseFanKeep2sFlag = 0;
#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
static uint8_t s_u8StreamProgress = 0;
static uint8_t s_u8StreamProgressPre = 0;
static uint8_t s_u8StreamStripProgress = 0;
static uint8_t s_u8StreamStripProgressPre = 0;
static uint16_t s_u16SteamPlayMode = 0;
#endif
static u16 s_u16DelayAudioID = 0xff;
static RoboEventId s_eLedErrorIconEvent = RE_INVALID;

typedef enum
{
    STEAM_PLAY_STOP,
    STEAM_PLAY_LOW_TEMP,
    STEAM_PLAY_HIGH_TEMP,
} SteamAudioPlayMode_e;

/******************************************************************************
 * 时间戳
 *****************************************************************************/
static uint64_t s_u64RunStartTick = INVALID_TIME;
static uint64_t s_u64SwicthModeTick = INVALID_TIME;
static uint64_t s_u64CleanBoxEmptyAlarmTick = INVALID_TIME;
static uint64_t s_u64HotWaterModeStartTick = INVALID_TIME;
#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
static uint64_t s_u64SteamModeStartTick = INVALID_TIME;
static uint64_t s_u64SteamModeBlinkTick = INVALID_TIME;
#endif
static uint64_t s_u64RunOutofDeepDirtyTick = INVALID_TIME;
static uint64_t s_u64HotKeyUpValveTick = INVALID_TIME;
static uint64_t s_u64HotKeyUpAirTick = INVALID_TIME;
static uint64_t s_u64HotKeyDownFoamTick = INVALID_TIME;
static uint64_t s_u64HotKeyDownValveTick = INVALID_TIME;

// 开始运行的时间
uint64_t get_run_start_tick(void)
{
    return s_u64RunStartTick;
}
void set_run_start_tick(uint64_t u64Tick)
{
    s_u64RunStartTick = u64Tick;
}
// 运行时切换挡位的时间
uint64_t get_switch_mode_tick(void)
{
    return s_u64SwicthModeTick;
}
void set_switch_mode_tick(uint64_t u64Tick)
{
    s_u64SwicthModeTick = u64Tick;
}
// 缺水报警的时间
uint64_t get_clean_box_empty_alarm_tick(void)
{
    return s_u64CleanBoxEmptyAlarmTick;
}
void set_clean_box_empty_alarm_tick(uint64_t u64Tick)
{
    s_u64CleanBoxEmptyAlarmTick = u64Tick;
}
// 热水模式的时间
uint64_t get_hot_water_mode_start_tick(void)
{
    return s_u64HotWaterModeStartTick;
}
void set_hot_water_mode_start_tick(uint64_t u64Tick)
{
    s_u64HotWaterModeStartTick = u64Tick;
}

#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
void set_steam_mode_start_tick(uint64_t u64Tick) // 设置蒸汽模式进入时间，退出蒸汽模式或播报结束设置INVALID_TIME清除计时
{
    s_u64SteamModeStartTick = u64Tick;
    if (u64Tick == INVALID_TIME)
    {
        s_u16SteamPlayMode = STEAM_PLAY_STOP;
        s_u64SteamModeBlinkTick = INVALID_TIME;
#if (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL)
        general_light_clear_strip_steam_state();
#elif (RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC)
#else
        rr_clear_strip_steam_state();
#endif
    }
}

uint64_t get_steam_mode_start_tick(void)
{
    return s_u64SteamModeStartTick;
}

uint64_t get_steam_mode_work_time(void)
{
    return get_ms_tick() - get_steam_mode_start_tick();
}
#endif

void set_run_outof_deepdirty_tick(uint64_t u64Tick)
{
    s_u64RunOutofDeepDirtyTick = u64Tick;
}

uint64_t get_run_outof_deepdirty_tick()
{
    return s_u64RunOutofDeepDirtyTick;
}

void set_hot_key_up_valve_tick(uint64_t u64Tick)
{
    s_u64HotKeyUpValveTick = u64Tick;
}

uint64_t get_hot_key_up_valve_tick()
{
    return s_u64HotKeyUpValveTick;
}

void set_hot_key_down_valve_tick(uint64_t u64Tick)
{
    s_u64HotKeyDownValveTick = u64Tick;
}

uint64_t get_hot_key_down_valve_tick()
{
    return s_u64HotKeyDownValveTick;
}

void set_hot_key_up_airpump_tick(uint64_t u64Tick)
{
    s_u64HotKeyUpAirTick = u64Tick;
}

uint64_t get_hot_key_up_airpump_tick()
{
    return s_u64HotKeyUpAirTick;
}

void set_hot_key_down_foampump_tick(uint64_t u64Tick)
{
    s_u64HotKeyDownFoamTick = u64Tick;
}

uint64_t get_hot_key_down_foampump_tick()
{
    return s_u64HotKeyDownFoamTick;
}

uint64_t get_run_outof_deepdirty_time(void)
{
    return (get_ms_tick() > get_run_outof_deepdirty_tick()) ? (get_ms_tick() - get_run_outof_deepdirty_tick()) : 0;
}

void set_led_error_icon_event(RoboEventId eLedErrorIconEvent)
{
    s_eLedErrorIconEvent = eLedErrorIconEvent;
}

RoboEventId get_led_error_icon_event()
{
    return s_eLedErrorIconEvent;
}
/******************************************************************************
 * 运行模式
 *****************************************************************************/
uint8_t get_previous_run_mode_index(RoboProject_st *pProject, uint8_t u8CurrentIndex)
{
    return (u8CurrentIndex - 1 + pProject->u8ActualRunModesCount) % pProject->u8ActualRunModesCount;
}

uint8_t get_next_run_mode_index(RoboProject_st *pProject, uint8_t u8CurrentIndex)
{
    return (u8CurrentIndex + 1) % pProject->u8ActualRunModesCount;
}

uint8_t get_current_run_enum_index(RoboProject_st *pProject, Run_Mode_e eCurrentMode)
{
    for (int i = 0; i < pProject->u8ActualRunModesCount; i++)
    {
        if (eCurrentMode == pProject->eRunModes[i])
        {
            return i;
        }
    }

    return 0;
}

Run_Mode_e get_run_mode_enum(RoboProject_st *pProject, uint8_t u8Index)
{
    if (u8Index < pProject->u8ActualRunModesCount)
    {
        return pProject->eRunModes[u8Index];
    }
    return RUN_MODE_AUTO;
}

void set_current_run_mode(RoboProject_st *pProject, uint8_t u8Index)
{
    if (u8Index < pProject->u8ActualRunModesCount)
    {
        Run_Mode_e eCurMode = get_run_mode_enum(pProject, pProject->u8CurrentModeIndex);
        Run_Mode_e eNxtMode = get_run_mode_enum(pProject, u8Index);
        rr_println("mode of %d switch: old %d -> new %d", RR_PROJECT_ID, eCurMode, eNxtMode);
        LOG_INFO("mode of %d switch: old %d -> new %d", RR_PROJECT_ID, eCurMode, eNxtMode);
        pProject->u8CurrentModeIndex = u8Index;
    }
}

RoboEventId run_submsg_key_filter_valid_event(SUBMSG_STATEMACHINE_KEY_ID_e eSubMsgKey, u32 u32KeyPara)
{
    RoboEventId eRoboEventId = RE_INVALID;

    switch (eSubMsgKey)
    {
    case RR_SUBMSG_STATE_MACHINE_KEY_POWER_SHORT_ID:
        eRoboEventId = RE_Key_Power_Short;
        break;
    case RR_SUBMSG_STATE_MACHINE_KEY_SWITCH_SHORT_ID:
        eRoboEventId = RE_Key_Switch_Short;
        break;
    case RR_SUBMSG_STATE_MACHINE_KEY_SWITCH_LONG_2S_ID:
        eRoboEventId = RE_Key_Switch_Long_2s;
        break;
    case RR_SUBMSG_STATE_MACHINE_KEY_POWER_LONG_10S_ID:
        eRoboEventId = RE_Key_Power_Long_10s;
        break;
    case RR_SUBMSG_STATE_MACHINE_KEY_HOTWATER_DOWN_ID:
        eRoboEventId = RE_Key_HotWater_Down;
        break;
    case RR_SUBMSG_STATE_MACHINE_KEY_HOTWATER_UP_ID:
        eRoboEventId = RE_Key_HotWater_Up;
        break;
    case RR_SUBMSG_STATE_MACHINE_KEY_HOTWATER_TIMEOUT_10S_ID:
        eRoboEventId = RE_Key_HotWater_TimeOut10s;
        break;
    default:
        break;
    }
    return eRoboEventId;
}

RoboEventId run_submsg_brush_filter_valid_event(SUBMSG_STATEMACHINE_BRUSH_ID_e eSubMsgBrush, u32 u32BrushPara)
{
    RoboEventId eRoboEventId = RE_INVALID;
    if (RR_SUBMSG_STATE_MACHINE_BRUSH_HARDWARE_OVER_CURRENT_ID == eSubMsgBrush ||
        RR_SUBMSG_STATE_MACHINE_BRUSH_SOFTWARE_OVER_CURRENT_ID == eSubMsgBrush)
    {
        eRoboEventId = RE_Brush_OverCurrent;
    }
    else if (RR_SUBMSG_STATE_MACHINE_BRUSH_OVER_TEMPER_ID == eSubMsgBrush)
    {
        eRoboEventId = RE_Brush_OverTemperature;
    }
    return eRoboEventId;
}

RoboEventId run_submsg_dirtycheck_filter_valid_event(SUBMSG_STATEMACHINE_DIRTYCHECK_ID_e eSubMsgDirtyCheck, u32 u32DirtyCheckPara)
{
    RoboEventId eRoboEventId = RE_INVALID;

    switch (eSubMsgDirtyCheck)
    {
    case RR_SUBMSG_STATE_MACHINE_RUN_INTO_DEEP_DIRTY_ID:
        set_is_run_deep_dirty(true);
        eRoboEventId = RE_DirtyCheck_Run_Into_DeepDirty;
        break;
    case RR_SUBMSG_STATE_MACHINE_RUN_OUTOF_DEEP_DIRTY_ID:
        set_is_run_deep_dirty(false);
        eRoboEventId = RE_DirtyCheck_Run_Outof_DeepDirty;
        break;
    default:
        break;
    }
    return eRoboEventId;
}

RoboEventId run_submsg_boiler_filter_valid_event(SUBMSG_STATEMACHINE_BOILER_ID_e eSubMsgBoiler, u32 u32BoilerPara)
{
    RoboEventId eRoboEventId = RE_INVALID;

    switch (eSubMsgBoiler)
    {
    case RR_SUBMSG_STATE_MACHINE_BOILER_TRIGGER_NTC_PROTECT_ID:
        eRoboEventId = RE_Boiler_TriggerNTCPortect;
        break;
    case RR_SUBMSG_STATE_MACHINE_BOILER_RELEASE_NTC_PROTECT_ID:
        eRoboEventId = RE_Boiler_ReleaseNTCPortect;
        break;
    case RR_SUBMSG_STATE_MACHINE_BOILER_TRIGGER_EMPTY_PROTECT_ID:
        eRoboEventId = RE_Boiler_TriggerEmptyPortect;
        break;
    case RR_SUBMSG_STATE_MACHINE_BOILER_RELEASE_EMPTY_PROTECT_ID:
        eRoboEventId = RE_Boiler_ReleaseEmptyPortect;
        break;
    default:
        break;
    }
    return eRoboEventId;
}

RoboEventId run_submsg_app_filter_valid_event(SUBMSG_STATEMACHINE_APP_ID_e eSubMsgApp, u32 u32AppPara)
{
    RoboEventId eRoboEventId = RE_INVALID;
    RoboStateStore_st *pRoboStateStore = get_robo_state_store_point();
    if (NULL == pRoboStateStore)
    {
        return RE_INVALID;
    }
    switch (eSubMsgApp)
    {
    case RR_SUBMSG_STATE_MACHINE_APP_AT_ID:
        eRoboEventId = RE_Run_Waitfor_AT;
        break;
    case RR_SUBMSG_STATE_MACHINE_APP_SWITCH_RUN_MODE_ID:
        pRoboStateStore->ePreRunMode = pRoboStateStore->eCurRunMode;
        pRoboStateStore->eCurRunMode = (Run_Mode_e)u32AppPara;
        eRoboEventId = RE_App_Set_Switch_Mode;
        break;
    case RR_SUBMSG_STATE_MACHINE_APP_RUN_STOP:
        eRoboEventId = RE_App_Run_Stop;
        break;
    case RR_SUBMSG_STATE_MACHINE_APP_SWITCH_RUN_TYPE_ID:
        eRoboEventId = RE_App_Switch_Run_Type;
        break;
    default:
        break;
    }

    return eRoboEventId;
}

RoboEventId run_submsg_battery_filter_valid_event(SUBMSG_STATEMACHINE_BATTERY_ID_e eSubMsgBattery, u32 u32BatteryPara)
{
    RoboEventId eRoboEventId = RE_INVALID;

    switch (eSubMsgBattery)
    {
    case RR_SUBMSG_STATE_MACHINE_BATTERY_SOC_LOWER_10:
        eRoboEventId = RE_Battery_SOC_Lower_10;
        break;
    case RR_SUBMSG_STATE_MACHINE_BATTERY_SOC_LOWER_5:
        eRoboEventId = RE_Battery_SOC_Lower_5;
        break;
    case RR_SUBMSG_STATE_MACHINE_BATTERY_SOC_LOWER_20:
        eRoboEventId = RE_Battery_SOC_Lower_20;
        break;
    case RR_SUBMSG_STATE_MACHINE_BATTERY_SOC_LOWER_25:
        eRoboEventId = RE_Battery_SOC_Lower_25;
        break;
    case RR_SUBMSG_STATE_MACHINE_BATTERY_SOC_LOWER_30:
        eRoboEventId = RE_Battery_SOC_Lower_30;
        break;
    default:
        break;
    }
    return eRoboEventId;
}

RoboEventId run_submsg_dock_filter_valid_event(SUBMSG_STATEMACHINE_DOCK_ID_e eSubMsgDock, u32 u32DockPara)
{
    RoboEventId eRoboEventId = RE_INVALID;

    RoboStateStore_st *pRoboStateStore = get_robo_state_store_point();
    if (NULL == pRoboStateStore)
    {
        return RE_INVALID;
    }

    if (RR_SUBMSG_STATE_MACHINE_DOCK_CHARGE_CONNECT_ID == eSubMsgDock)
    {
        eRoboEventId = RE_Dock_Connect;
        pRoboStateStore->bConnectDock = true;
    }

    if (RR_SUBMSG_STATE_MACHINE_DOCK_COMM_OK_ID == eSubMsgDock)
    {
        eRoboEventId = RE_Dock_Connect;
        pRoboStateStore->bConnectDockComm = true;
    }

    return eRoboEventId;
}

RoboEventId run_submsg_clean_water_tank_filter_valid_event(SUBMSG_STATEMACHINE_CLEAN_WATER_TANK_ID_e eSubMsgCleanWaterTank, u32 u32CleanWaterTankPara)
{
    return RE_INVALID;
}

arStateId_e run_event_power_key_process(RoboEventId eEventId)
{
    arStateId_e NextState = SID_Run;
    RoboStateStore_st *pRoboStateStore = get_robo_state_store_point();
    switch (eEventId)
    {
    case RE_Key_Power_Short:
    case RE_App_Run_Stop:
        set_close_fan_keep_2s_flag(AUDIO_STREAM_INVALID);
        pRoboStateStore->eRunQuitReason = RUN_QUIT_REASON_POWER_KEY;
        // set_led_dirty(LED_Dirty_Breath_Fast, 1);
        audio_play(AUDIO_STREAM_STOP);
        NextState = SID_Ready;
        break;
    case RE_Key_Power_Long_10s:
        // 强制断电，考虑提前告诉esp32，以便做一些回收操作
        flash_save_data(FLASH_OBJECT_NVRAM);
        flash_save_data(FLASH_OBJECT_BLACKBOX);
        set_wifi_ready_to_shutdown();
        break;
    default:
        break;
    }

    return NextState;
}

arStateId_e run_event_hotwater_key_process(RoboEventId eEventId)
{
    arStateId_e NextState = SID_Run;

    RoboStateStore_st *pRoboStateStore = get_robo_state_store_point();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pDeviceControlStateStore || NULL == pRoboStateStore)
    {
        return NextState;
    }

#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) // ProX 插件策略待确定
    if (RE_Key_HotWater_Down == eEventId)
    {
        rr_println("SparyWaterMode:%d", get_addon_set_spary_water_mode());
        LOG_DEBUG("SparyWaterMode:%d", get_addon_set_spary_water_mode());
    }

    if (DP_INDEX_HOTWATER_KEY_OFF == get_addon_set_spary_water_mode())
    {
        return NextState;
    }
#endif

    switch (eEventId)
    {
    case RE_Key_HotWater_Down:
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
        if (stNVRAMData.CustomOptionV2.IsBrushLightOn)
        {
            set_board_led(true);
        }
        if (pRoboStateStore->eCurRunMode != RUN_MODE_HOTWATER) // 非热水模式只有滋热水才是红灯带
        {
            if (get_addon_set_spary_water_mode() == DP_INDEX_HOTAWTER_KEY_ALLMODE_HOTWATER)
            {
                set_led_dirty(LED_Dirty_All_Red, 0);
            }
        }
        // 热水模式本来就是红灯带暂不处理
        pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyWater = true;
        pDeviceControlStateStore->stBoilerCSS.u16Power = get_current_boiler_power(pRoboStateStore->eCurRunMode);
        pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_ON;
        pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pRoboStateStore->eCurRunMode);
#endif
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
        pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyFoam = true;
        pDeviceControlStateStore->stAirPumpCSS.u16AirPumpVoltage = AIR_PUMP_VOLTAGE;
        pDeviceControlStateStore->stAirPumpCSS.bAirPumpParaChanged = true;
        pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_ON;
        pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
        set_hot_key_up_airpump_tick(INVALID_TIME);
        set_hot_key_down_foampump_tick(get_ms_tick());
        set_hot_key_up_valve_tick(INVALID_TIME);
        set_hot_key_down_valve_tick(get_ms_tick());
        mark_FoamMode_work_start_tick();
        record_foam_mode_times();
#endif
        break;
    case RE_Key_HotWater_Up:
    case RE_Key_HotWater_TimeOut10s:
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
        if (stNVRAMData.CustomOptionV2.IsBrushLightOn != DP_INDEX_BRUSH_LIGHT_ALL_ON)
        {
            set_board_led(false);
        }
        if (pRoboStateStore->eCurRunMode != RUN_MODE_HOTWATER)
        {
            set_led_dirty(LED_Dirty_Gradual, 0);
        }
        pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyWater = false;
        pDeviceControlStateStore->stBoilerCSS.u16Power = get_current_boiler_power(pRoboStateStore->eCurRunMode);
        pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_OFF;
        pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pRoboStateStore->eCurRunMode);
#endif
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
        pDeviceControlStateStore->stFoamPumpCSS.u16FoamPumpVoltage = 0;
        pDeviceControlStateStore->stFoamPumpCSS.bFoamPumpParaChanged = true;
        set_hot_key_up_airpump_tick(get_ms_tick());
        set_hot_key_down_foampump_tick(INVALID_TIME);
        set_hot_key_up_valve_tick(get_ms_tick());
        set_hot_key_down_valve_tick(INVALID_TIME);
        record_FoamMode_work_log();
#endif
        break;
    }
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
    pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
    pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
    pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
#endif
    rr_devices_control();

    return NextState;
}

arStateId_e run_or_ready_event_switch_mode_process(RoboEventId eEventId, arStateId_e eCurState, RoboStateStore_st *pRoboStateStore)
{
    arStateId_e NextState = eCurState;
    RoboProject_st *pRoboProject = get_robo_cur_project_point();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pRoboProject || NULL == pDeviceControlStateStore || NULL == pRoboStateStore)
    {
        return NextState;
    }

    if (RUN_MODE_STEAM == pRoboStateStore->eCurRunMode && STEAM_MODE_LARGE == get_steam_mode())
    {
        switch (eEventId)
        {
        case RE_Key_Switch_Short:
        case RE_Key_Switch_Long_2s:
            pRoboStateStore->ePreRunMode = pRoboStateStore->eCurRunMode;
            pRoboStateStore->eCurRunMode = RUN_MODE_HOTWATER;
            set_current_run_mode(pRoboProject, get_current_run_enum_index(pRoboProject, pRoboStateStore->eCurRunMode));
            switch_steam_mode();
            break;
        default:
            break;
        }
    }
    else
    {
        switch (eEventId)
        {
        case RE_Key_Switch_Short:
            pRoboStateStore->ePreRunMode = pRoboStateStore->eCurRunMode;
            set_current_run_mode(pRoboProject, get_next_run_mode_index(pRoboProject, pRoboProject->u8CurrentModeIndex));
            pRoboStateStore->eCurRunMode = get_run_mode_enum(pRoboProject, pRoboProject->u8CurrentModeIndex);
#ifndef _NOWIFI_VER_
            BB_Clean_SetLatestChangeLevelWay(BB_LATESTCHANGELEVEL_KEY);
            sync_data_points_changed_with_run_mode_to_addon(pRoboStateStore->eCurRunMode);
#ifdef IS_SUPPORT_BRUSH_BOARD
            sync_power_wheel_slope_depends_addon_setting();
#endif
#endif
            break;
        case RE_App_Set_Switch_Mode:
            set_current_run_mode(pRoboProject, get_current_run_enum_index(pRoboProject, pRoboStateStore->eCurRunMode));
#ifndef _NOWIFI_VER_
            BB_Clean_SetLatestChangeLevelWay(BB_LATESTCHANGELEVEL_APP);
#ifdef IS_SUPPORT_BRUSH_BOARD
            sync_power_wheel_slope_depends_addon_setting();
#endif
#endif
            break;
        case RE_Key_Switch_Long_2s:
            pRoboStateStore->ePreRunMode = pRoboStateStore->eCurRunMode;
            pRoboStateStore->eCurRunMode = RUN_MODE_STEAM;
            set_current_run_mode(pRoboProject, get_current_run_enum_index(pRoboProject, pRoboStateStore->eCurRunMode));
            switch_steam_mode();
            break;
        default:
            break;
        }
    }

    set_led_run_mode(pRoboStateStore->eCurRunMode);
    // 在运行状态下切换模式时，统一设置灯带灯效
    if (SID_Run == eCurState)
    {
        switch (pRoboStateStore->eCurRunMode)
        {
        case RUN_MODE_HOTWATER:
            set_led_dirty(LED_Dirty_All_Red, 0); // yaoguang热水模式单独设置灯带灯效
            set_screen_soc();
            set_led_soc(LED_ON, LED_Direct_Run, pRoboStateStore->u8CurSoc); // 蒸汽切热水，可能需要恢复电量显示
            break;
#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
        case RUN_MODE_STEAM:
            // yaoguang蒸汽模式，如果温度高于90度，不显示蒸汽制备灯效，直接显示蒸汽运行灯效
            // 如果需要显示蒸汽制备灯效，灯条先全灭
            if (get_boiler_temperature() > BOILER_TEMP_HOT_WATER)
            {
                // set_led_dirty(LED_Dirty_Steam, 0);
                set_led_dirty(LED_Dirty_All_On, 0);
            }
            else
            {
                set_led_dirty(LED_Dirty_All_OFF, 0);
            }
            break;
#endif
        default:
            set_led_dirty(LED_Dirty_Gradual, 0);
            if (RUN_MODE_STEAM == pRoboStateStore->ePreRunMode)
            {
                set_screen_soc();
                set_led_soc(LED_ON, LED_Direct_Run, pRoboStateStore->u8CurSoc); // 蒸汽切其他模式，可能需要恢复电量显示
            }
            break;
        }
    }
#if (RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC) || (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE) || \
    (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL)
    set_led_icons_at_once();
#endif
    // 只有切换模式，才开启大水量冲锅炉。停止或重新进入不开启大水量
    pDeviceControlStateStore->stPumpCSS.bIsNeedSteamModeMaxWaterOut = true;

    pDeviceControlStateStore->stFanCSS.u8FanPower = get_current_fan_power(pRoboStateStore->eCurRunMode);
    pDeviceControlStateStore->stBrushCSS.u16BrushVoltage = get_current_brush_voltage(pRoboStateStore->eCurRunMode);
    pDeviceControlStateStore->stBrushCSS.eBrushMode = BRUSH_MODE_RUN_VOLTAGE;
    pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pRoboStateStore->eCurRunMode);

#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
    set_steam_mode_start_tick(INVALID_TIME);
#endif
    switch (pRoboStateStore->eCurRunMode)
    {
    case RUN_MODE_AUTO:
        audio_play(AUDIO_STREAM_AUTO_MODE);
        break;
    case RUN_MODE_MAX:
        audio_play(AUDIO_STREAM_MAX_MODE);
        break;
    case RUN_MODE_LIGHT:
        audio_play(AUDIO_STREAM_LIGHTPOWER_MODE);
        break;
    case RUN_MODE_UPTAKE:
        audio_play(AUDIO_STREAM_DRYING_MODE);
        break;
    case RUN_MODE_HOTWATER:
        audio_play(AUDIO_STREAM_HOT_WATER_MODE);
        break;
    case RUN_MODE_STEAM:
        if (pRoboStateStore->eSteamMode == STEAM_MODE_LARGE)
        {
            audio_play(AUDIO_STREAM_TURBO_STEAM);
        }
        else
        {
            audio_play(AUDIO_STREAM_STEAM_MODE);
            if (SID_Run == eCurState)
            {
                audio_play(AUDIO_STREAM_STEAM_MAKING);
            }
        }
#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
        steam_temperature_detect_restart();
#endif
        break;
    default:
        break;
    }
    if (SID_Run == eCurState)
    {
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
        // 运行中切档时对电磁阀的处理
        if (pRoboStateStore->eCurRunMode == RUN_MODE_HOTWATER)
        {
            set_hot_water_mode_start_tick(get_ms_tick());
            // 运行中切档到热水模式，前5s先不开电磁阀
            pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_1_2_OFF;
            // 清除热水执行时间计时
            pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut = false;
        }
        else if (pRoboStateStore->eCurRunMode == RUN_MODE_STEAM)
        {
            set_steam_mode_start_tick(get_ms_tick()); // 蒸汽语音计时
            /**
             * 普通蒸汽模式，以下情况需要需要开启大水量冲锅炉，开5s电磁阀
             * 1、开机第一次运行蒸汽模式
             * 2、在蒸汽模式运行前，热水模式运行超过2s
             * 3、在蒸汽模式运行前，热水自清洁运行超过2s
             */
            if (STEAM_MODE_NORMAL == get_steam_mode())
            {
                if (pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut == true)
                {
                    pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_1_2_ON;
                }
            }
            else if (STEAM_MODE_LARGE == get_steam_mode())
            {
                pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_1_2_OFF;
            }
        }
        else
        {
            pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_ALL_OFF;
        }

        // 运行过程中，切档到非蒸汽模式的其他模式，关闭蒸汽泵
        if (pRoboStateStore->eCurRunMode != RUN_MODE_STEAM)
        {
            steam_pump_update_state_store_flow(pDeviceControlStateStore, STEAM_PUMP_0_ML);
        }
#endif

        if (pRoboStateStore->ePreRunMode == RUN_MODE_UPTAKE)
        {
            if (true == pDeviceControlStateStore->stPumpCSS.bIsNeedOffWhenCWTankEmpty)
            {
                // 运行中吸水模式（满足缺水停机条件）切档到其他模式，重新缺水检测
                clear_clwater_bucket();
                clear_clean_box_det_history();
                clean_box_empty_det_for_boiler_init();
                pDeviceControlStateStore->stPumpCSS.bIsNeedOffWhenCWTankEmpty = false;
            }
            clear_clean_box_step_flag(); // 清除吸水模式已触发的step标志位，防止切换到其他模式后不报缺水
        }

        pDeviceControlStateStore->stBoilerCSS.u16Power = 0; // 运行时切换模式关闭锅炉
        pDeviceControlStateStore->stBoilerCSS.bHaveOpenWhenHotWaterMode = false;
        pDeviceControlStateStore->stFanCSS.bFanParaChanged = true;
        pDeviceControlStateStore->stBrushCSS.bBrushParaChanged = true;
        pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
        pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
        pDeviceControlStateStore->stSteamPumpCSS.bSteamPumpParaChanged = true;
        pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
        rr_devices_control();
    }

    set_switch_mode_tick(get_ms_tick());
    reset_pump_fan_phase_time();

    return NextState;
}

arStateId_e run_event_microswitch_process(RoboEventId eEventId)
{
    arStateId_e NextState = SID_Run;
    RoboStateStore_st *pstRoboState = get_robo_state_store_point();
    if (RE_MicroSwitch_Lock == eEventId)
    {
        if (stNVRAMData.CustomOption.IsAutoRun == DP_INDEX_AUTORUN_STANDLOCK_ALL_ON)
        {
            set_close_fan_keep_2s_flag(AUDIO_STREAM_INVALID);
            pstRoboState->eRunQuitReason = RUN_QUIT_REASON_MICROSWITCH;
            audio_play(AUDIO_STREAM_STOP);
#ifdef IS_SUPPORT_BRUSH_BOARD
            set_power_wheel_backup(BackupMode_StandLock);
#endif
            NextState = SID_Ready;
        }
    }

    return NextState;
}
arStateId_e run_event_brush_process(RoboEventId eEventId)
{
    arStateId_e NextState = SID_Run;

    switch (eEventId)
    {
    case RE_Brush_OverCurrent:
        set_close_fan_keep_2s_flag(AUDIO_STREAM_BRUSH_OVER_CURRENT);
        // set_led_dirty(LED_Dirty_Breath_Fast, 0);
#if (RR_LED_MODE == RR_LED_PWM)
        set_led_param(LED_FLASHING_ALONE, LED_RED, LED_FOUR, LED_FLASHING_PERIOD);
        Led_pwm.Led_errorevent = LED_EVENT_BRUSH;
#endif
        NextState = SID_Error;
        break;
    default:
        break;
    }

    return NextState;
}

arStateId_e run_event_dock_process(RoboEventId eEventId, RoboStateStore_st *pRoboStateStore)
{
    arStateId_e NextState = SID_Run;
    if (NULL == pRoboStateStore)
    {
        return NextState;
    }

    switch (eEventId)
    {
    case RE_Dock_Connect:
        NextState = SID_Charge;
        if (is_ready_to_autoselfclean())
        {
            NextState = SID_AutoClean;
            return NextState;
        }
        if (stNVRAMData.PrivateData.u8FirstAtDock == 0)
        {
            return NextState;
        }
        if (pRoboStateStore->bConnectDock == true)
        {
            g_bNeedPlayStartCharge = false;
            if (pRoboStateStore->u8CurSoc < BAT_BEFORE_AUTOCLEAN)
            {
                audio_play(AUDIO_STREAM_CHARGESTART);
            }
            else
            {
                audio_play(AUDIO_STREAM_START_CHARGING_PRESS_KEY_AUTO_CLEAN);
            }
        }
        else
        {
            g_bNeedPlayStartCharge = true;
        }
        break;
    default:
        break;
    }

    return NextState;
}

arStateId_e run_event_flat_process(RoboEventId eEventId, RoboStateStore_st *pRoboStateStore)
{
    arStateId_e NextState = SID_Run;
    RoboProject_st *pRoboProject = get_robo_cur_project_point();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pRoboProject || NULL == pDeviceControlStateStore || NULL == pRoboStateStore)
    {
        return NextState;
    }

    switch (eEventId)
    {
    case RE_Flat_Detect:
        set_sewage_full_count(SEWAGE_CHECK_DURATION_COUNT_WITH_FLAT);
        pDeviceControlStateStore->stPumpCSS.bIsNeedMaxWaterOut = false;
#ifndef _NOWIFI_VER_
        mark_Flat_start_tick();
#endif
        break;
    case RE_Flat_Recover:
        set_sewage_full_count(SEWAGE_CHECK_DURATION_COUNT);
        if (RUN_MODE_AUTO == pRoboStateStore->eCurRunMode || RUN_MODE_UPTAKE == pRoboStateStore->eCurRunMode)
        {
            reset_pump_fan_phase_time();
        }
#ifndef _NOWIFI_VER_
        record_flat_work_log();
#endif
        break;
    default:
        break;
    }

    pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
    pDeviceControlStateStore->stFanCSS.bFanParaChanged = true;
    pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pRoboStateStore->eCurRunMode);
    pDeviceControlStateStore->stFanCSS.u8FanPower = get_current_fan_power(pRoboStateStore->eCurRunMode);

    rr_devices_control();

    return NextState;
}

arStateId_e run_event_clean_water_tank_process(RoboEventId eEventId, RoboStateStore_st *pRoboStateStore)
{
    arStateId_e NextState = SID_Run;
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
    EmptyForSpary_st *pEmptyForSpary = get_empty_for_spray_handle();
#endif
    if (NULL == pDeviceControlStateStore || NULL == pRoboStateStore)
    {
        return NextState;
    }

    switch (eEventId)
    {
    case RE_CleanWaterTank_Empty_RunStart:
    case RE_CleanWaterTank_Empty_HoldStep3:
        if (RUN_MODE_UPTAKE == pRoboStateStore->eCurRunMode)
        {
            pDeviceControlStateStore->stPumpCSS.bIsNeedOffWhenCWTankEmpty = true;
            pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pRoboStateStore->eCurRunMode);
            pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
            rr_devices_control();
            NextState = SID_Run;
        }
        else
        {
            set_close_fan_keep_2s_flag(AUDIO_STREAM_ADDWATER_CLEANTANK_PLACE_CORRECT);
            // set_led_dirty(LED_Dirty_Breath_Fast, 0);
            NextState = SID_Error;
        }
        set_clean_box_empty_alarm_tick(get_ms_tick());
        break;
    case RE_CleanWaterTank_Empty_HoldStep1:
    case RE_CleanWaterTank_Empty_HoldStep2:
        if (RUN_MODE_UPTAKE != pRoboStateStore->eCurRunMode)
        {
            set_led_error_icon(LED_Blink_Slow);
            set_led_error_icon_event(RE_CleanWaterTank_Empty_HoldStep2);
#if (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE) || (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL) || ((RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC) && (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX))
            set_led_icons_at_once();
#endif
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
            if (pEmptyForSpary->bHaveSpeaker == 0)
#endif
            {
                audio_play(AUDIO_STREAM_ADDWATER_CLEANTANK_PLACE_CORRECT);
            }
            set_clean_box_empty_alarm_tick(get_ms_tick());
        }
        NextState = SID_Run;
        break;
    case RE_CleanWaterTank_Not_Empty:
        if (RE_CleanWaterTank_Empty_HoldStep2 == get_led_error_icon_event())
        {
            set_led_error_icon(LED_OFF);
#if (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE) || (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL) || ((RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC) && (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX))
            set_led_icons_at_once();
#endif
        }
        NextState = SID_Run;
        break;
    default:
        break;
    }

    return NextState;
}

arStateId_e run_event_dirty_water_tank_process(RoboEventId eEventId)
{
    arStateId_e NextState = SID_Run;

    switch (eEventId)
    {
    case RE_DirtyWaterTank_Right_Full:
    case RE_DirtyWaterTank_NotPlace:
        audio_play(AUDIO_STREAM_CHECK_DIRTY_WATER_PLACE_CORRECT);
#if (RR_LED_MODE == RR_LED_PWM)
        set_led_param(LED_ON_ALWAYS, LED_RED, LED_TWO, LED_FLASHING_PERIOD);
        Led_pwm.Led_errorevent = LED_EVENT_DIRTY_FULL;
#endif
        NextState = SID_Error;
        break;
    default:
        break;
    }

    return NextState;
}

arStateId_e run_event_dirtycheck_process(RoboEventId eEventId, RoboStateStore_st *pRoboStateStore)
{
    arStateId_e NextState = SID_Run;
    RoboProject_st *pRoboProject = get_robo_cur_project_point();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pRoboProject || NULL == pDeviceControlStateStore)
    {
        return NextState;
    }

    switch (eEventId)
    {
    case RE_DirtyCheck_Run_Into_DeepDirty:
        reset_pump_fan_phase_time();
        break;
    case RE_DirtyCheck_Run_Outof_DeepDirty:
        set_run_outof_deepdirty_tick(get_ms_tick());
        break;
    default:
        break;
    }

    pDeviceControlStateStore->stFanCSS.bFanParaChanged = true;
    pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
    pDeviceControlStateStore->stFanCSS.u8FanPower = get_current_fan_power(pRoboStateStore->eCurRunMode);
    pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pRoboStateStore->eCurRunMode);
    rr_devices_control();

    return NextState;
}

arStateId_e run_event_battery_process(RoboEventId eEventId, RoboStateStore_st *pRoboStateStore)
{
    arStateId_e NextState = SID_Run;

    switch (eEventId)
    {
    case RE_Battery_Discharge_HighTemp:
    case RE_Battery_Discharge_LowTemp:
        set_close_fan_keep_2s_flag(AUDIO_STREAM_TEMPERATUREPROTECTANDWAITTONORMAL);
        // set_led_dirty(LED_Dirty_Breath_Fast, 0);
#if (RR_LED_MODE == RR_LED_PWM)
        set_led_param(LED_FLASHING_ALONE, LED_RED, LED_FIVE, LED_FLASHING_PERIOD);
        Led_pwm.Led_errorevent = LED_EVENT_BATTERY_TEMP;
#endif
        NextState = SID_Error;
        break;
    case RE_Battery_Undervoltage:
        set_led_soc(LED_Blink_Slow, LED_Direct_Run, pRoboStateStore->u8CurSoc);
#if (RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC) || (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE) || \
    (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL)
        set_led_icons_at_once();
        audio_play(AUDIO_STREAM_LOW_BATTERY_DOCK_CHARGING);
#elif (RR_LED_MODE == RR_LED_PWM)
        set_led_param(LED_UNDERVOLTAGE, LED_RED, LED_SIX, LED_FLASHING_PERIOD);
        Led_pwm.Led_errorevent = LED_EVENT_BATTERY_UNDERVOLTAGE;
#else
        audio_play(AUDIO_STREAM_LOW_BATTERY_DOCK_CHARGING);
#endif
        NextState = SID_Error;
        break;
    case RE_Battery_SOC_Lower_20:
    case RE_Battery_SOC_Lower_5:
#if (RR_LED_MODE == RR_LED_PWM)
        set_led_param(LED_SLOW_FLASHING, LED_WHITE, LED_NONE, LED_FLASHING_PERIOD);
#else
        audio_play(AUDIO_STREAM_LOW_BATTERY_DOCK_CHARGING);
#endif
        break;
    default:
        break;
    }

    return NextState;
}

arStateId_e run_event_boiler_process(RoboEventId eEventId, RoboStateStore_st *pRoboStateStore, arStateId_e CurState)
{
    arStateId_e NextState = CurState;
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (pRoboStateStore == NULL || pDeviceControlStateStore == NULL)
    {
        return NextState;
    }
    switch (eEventId)
    {
    case RE_Boiler_TemperatureState_Change:
    case RE_Boiler_SlowDown_Update:
        pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
        break;
    case RE_Boiler_TriggerNTCPortect:
    case RE_Boiler_TriggerEmptyPortect:
    case RE_Boiler_ReleaseNTCPortect:
    case RE_Boiler_ReleaseEmptyPortect:
        if (SID_Run == CurState)
        {
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
            pDeviceControlStateStore->stBoilerCSS.u16Power = get_current_boiler_power(pRoboStateStore->eCurRunMode);
            pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
#endif
#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
            if (RUN_MODE_STEAM == pRoboStateStore->eCurRunMode)
            {
                steam_temperature_detect_restart();
            }
#endif
            if (RUN_MODE_HOTWATER == pRoboStateStore->eCurRunMode)
            {
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
                pDeviceControlStateStore->stBoilerCSS.u16Power = 0;
                pDeviceControlStateStore->stBoilerCSS.bHaveOpenWhenHotWaterMode = false;
                pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
#endif
            }
        }
        else if (SID_AutoClean == CurState)
        {
#if ((RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX))
            if (AUTOCLEAN_HIGH_TEMP == pRoboStateStore->eAutoCleanTemperatureType)
            {
                if (pDeviceControlStateStore->stBoilerCSS.bTriggerNTCProtect == false &&
                    pDeviceControlStateStore->stBoilerCSS.bTriggerEmptyProtect == false)
                {
                    pDeviceControlStateStore->stBoilerCSS.u16Power = get_hotwater_autoclean_target_boiler_power();
                }
                else
                {
                    pDeviceControlStateStore->stBoilerCSS.u16Power = 0;
                }
                pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
            }
            else
            {
                steam_temperature_detect_restart();
            }
#endif
        }
        break;
    }
    rr_devices_control();
    return NextState;
}

uint8_t get_current_fan_power(Run_Mode_e eCurMode)
{
    uint8_t u8FanPower = FAN_POWER_AUTO;
    uint8_t u8PumpModeStep = get_run_pump_mode_step();

    if (RUN_MODE_AUTO == eCurMode)
    {
#ifndef _NOWIFI_VER_
        Gear_App_Level_e eAppDevicesLevel = get_addon_set_fan_level();
        if (GEAR_APP_LEVEL_MID == eAppDevicesLevel)
        {
            u8FanPower = FAN_POWER_AUTO;
            if (0 != u8PumpModeStep)
            {
                u8FanPower = FAN_POWER_AUTO_LIFE;
            }
        }
        else if (GEAR_APP_LEVEL_HIGH == eAppDevicesLevel)
        {
            u8FanPower = FAN_POWER_AUTOPLUS;
        }
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
        else if (GEAR_APP_LEVEL_180W == eAppDevicesLevel)
        {
            u8FanPower = FAN_POWER_AUTO_TEST_180W;
        }
        else if (GEAR_APP_LEVEL_210W == eAppDevicesLevel)
        {
            u8FanPower = FAN_POWER_AUTO_TEST_210W;
        }
        else if (GEAR_APP_LEVEL_350W == eAppDevicesLevel)
        {
            u8FanPower = FAN_POWER_AUTO_TEST_350W;
        }
#endif
        else
        {
            u8FanPower = FAN_POWER_AUTOMINUS;
        }
#else
        u8FanPower = FAN_POWER_AUTO;
        if (0 != u8PumpModeStep)
        {
            u8FanPower = FAN_POWER_AUTO_LIFE;
        }
#endif
        if (true == get_flat_status())
        {
            u8FanPower = (u8FanPower < FAN_AUTO_FLAT_POWER) ? u8FanPower : FAN_AUTO_FLAT_POWER;
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) || (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
            return u8FanPower;
#endif
        }
    }
    else if (RUN_MODE_UPTAKE == eCurMode)
    {
#ifndef _NOWIFI_VER_
        Gear_App_Level_e eAppDevicesLevel = get_addon_set_fan_level_uptake();
        if (GEAR_APP_LEVEL_MID == eAppDevicesLevel)
        {
            u8FanPower = FAN_POWER_UPTAKE;
        }
        else if (GEAR_APP_LEVEL_HIGH == eAppDevicesLevel)
        {
            u8FanPower = FAN_POWER_UPTAKEPLUS;
        }
        else
        {
            u8FanPower = FAN_POWER_UPTAKEMINUS;
        }
#else
        u8FanPower = FAN_POWER_UPTAKE;
#endif
        if (true == get_flat_status())
        {
            u8FanPower = (u8FanPower < FAN_UPTAKE_FLAT_POWER) ? u8FanPower : FAN_UPTAKE_FLAT_POWER;
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) || (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
            return u8FanPower;
#endif
        }
    }
    else if (RUN_MODE_MAX == eCurMode)
    {
        u8FanPower = FAN_POWER_MAX;
        if (true == get_flat_status())
        {
            u8FanPower = (u8FanPower < FAN_MAX_FLAT_POWER) ? u8FanPower : FAN_MAX_FLAT_POWER;
        }
    }
    else if (RUN_MODE_LIGHT == eCurMode)
    {
        u8FanPower = FAN_POWER_LIGHT;
        if (true == get_flat_status())
        {
            u8FanPower = (u8FanPower < FAN_LIGHT_FLAT_POWER) ? u8FanPower : FAN_LIGHT_FLAT_POWER;
        }
    }
    else if (RUN_MODE_STEAM == eCurMode)
    {
        if (STEAM_MODE_LARGE == get_steam_mode())
        {
            u8FanPower = FAN_POWER_LARGE_STEAM;
            if (true == get_flat_status())
            {
                u8FanPower = (u8FanPower < FAN_LARGE_STEAM_FLAT_POWER) ? u8FanPower : FAN_LARGE_STEAM_FLAT_POWER;
            }
        }
        else
        {
            u8FanPower = FAN_POWER_STEAM;
            if (true == get_flat_status())
            {
                u8FanPower = (u8FanPower < FAN_STEAM_FLAT_POWER) ? u8FanPower : FAN_STEAM_FLAT_POWER;
            }
        }
    }
    else if (RUN_MODE_HOTWATER == eCurMode)
    {
        u8FanPower = FAN_POWER_HOTWATER;
        if (true == get_flat_status())
        {
            u8FanPower = (u8FanPower < FAN_HOT_FLAT_POWER) ? u8FanPower : FAN_HOT_FLAT_POWER;
        }
    }

    // 脏污检测
    if (true == get_is_run_deep_dirty())
    {
        if (RUN_MODE_AUTO == eCurMode)
        {
            u8FanPower = FAN_AUTO_DIRTY_POWER;
        }
        else if (RUN_MODE_UPTAKE == eCurMode)
        {
            u8FanPower = FAN_UPTAKE_DIRTY_POWER;
        }
    }

    return u8FanPower;
}

uint16_t get_current_brush_voltage(Run_Mode_e eCurMode)
{
    uint16_t u16BrushVoltage = BRUSH_AUTO_HIGH_VOLTAGE;
    bool bIsFlat = get_flat_status();

    if (RUN_MODE_AUTO == eCurMode)
    {
        if (true == bIsFlat)
        {
            set_brush_run_vol(BRUSH_AUTO_FLAT_VOLTAGE);
            return BRUSH_AUTO_FLAT_VOLTAGE;
        }
#ifndef _NOWIFI_VER_
        Gear_App_Level_e eAppDevicesLevel = get_addon_set_brush_level();
        if (GEAR_APP_LEVEL_HIGH == eAppDevicesLevel)
        {
            u16BrushVoltage = BRUSH_AUTO_HIGH_VOLTAGE;
        }
        else
        {
            u16BrushVoltage = BRUSH_AUTO_LOW_VOLTAGE;
        }
#else
        u16BrushVoltage = BRUSH_AUTO_HIGH_VOLTAGE;
#endif
    }
    else if (RUN_MODE_UPTAKE == eCurMode)
    {
        if (true == bIsFlat)
        {
            set_brush_run_vol(BRUSH_UPTAKE_FLAT_VOLTAGE);
            return BRUSH_UPTAKE_FLAT_VOLTAGE;
        }
#ifndef _NOWIFI_VER_
        Gear_App_Level_e eAppDevicesLevel = get_addon_set_brush_level_uptake();
        if (GEAR_APP_LEVEL_HIGH == eAppDevicesLevel)
        {
            u16BrushVoltage = BRUSH_UPTAKE_HIGH_VOLTAGE;
        }
        else
        {
            u16BrushVoltage = BRUSH_UPTAKE_LOW_VOLTAGE;
        }
#else
        u16BrushVoltage = BRUSH_UPTAKE_HIGH_VOLTAGE;
#endif
    }
    else if (RUN_MODE_MAX == eCurMode)
    {
        u16BrushVoltage = (true == bIsFlat) ? BRUSH_MAX_FLAT_VOLTAGE : BRUSH_MAX_VOLTAGE;
    }
    else if (RUN_MODE_LIGHT == eCurMode)
    {
        u16BrushVoltage = (true == bIsFlat) ? BRUSH_LIGHT_FLAT_VOLTAGE : BRUSH_LIGHT_VOLTAGE;
    }
    else if (RUN_MODE_STEAM == eCurMode)
    {
        if (STEAM_MODE_LARGE == get_steam_mode())
        {
            u16BrushVoltage = (true == bIsFlat) ? BRUSH_LARGE_STEAM_FLAT_VOLTAGE : BRUSH_LARGE_STEAM_VOLTAGE;
        }
        else
        {
            u16BrushVoltage = (true == bIsFlat) ? BRUSH_STEAM_FLAT_VOLTAGE : BRUSH_STEAM_VOLTAGE;
        }
    }
    else if (RUN_MODE_HOTWATER == eCurMode)
    {
        u16BrushVoltage = (true == bIsFlat) ? BRUSH_HOT_FLAT_VOLTAGE : BRUSH_HOT_VOLTAGE;
    }
    set_brush_run_vol(u16BrushVoltage);
    return u16BrushVoltage;
}

/******************************************************************************
 * 蠕动泵
 * -- 更新流量的时机
 * 1.开始/停止运行 √
 * 2.进入退出躺平 √
 * 3.进入退出脏污 √
 * 4.大水量结束 √
 * 5.切换模式 √
 * 6.分阶段出水阶段结束时 √
 * 7.APP设置新的流量（待定是下一次运行生效还是立即生效） todo@chenruiyun
 * 8.检测到缺水后的大水量和大水量结束 √
 * 9.大水冲锅炉结束，若大水量没结束，继续大水量 √
 * ......
 *
 * -- 优先级（数字越小越高）
 * 0.缺水大水量(如果在15V大水量则维持15V大水量)
 * 1.满足语音播报并且停机的缺水检测状态时，关闭清水泵(YG吸水模式)
 * 2.躺平
 * 3.大水冲锅炉
 * 4.大水量
 * 5.脏污
 * 6.常规模式
 */
Pump_Mode_e get_current_pump_flow(Run_Mode_e eCurMode)
{
    Pump_Mode_e ePumpMode = PUMP_MODE_STEP_OFF;
    uint8_t u8PumpModeStep = get_run_pump_mode_step();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();

    if (pDeviceControlStateStore == NULL)
    {
        return PUMP_MODE_STEP_OFF;
    }

    // 任意模式都可以触发滋水，触发滋水后滋水的流量优先级最高
    if (pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyWater)
    {
        return PUMP_MODE_SPARY_WATER;
    }

    if (pDeviceControlStateStore->stPumpCSS.bIsNeedForEmptyDet == true)
    {
        return PUMP_MODE_FOR_EMPTY_DET;
    }

    if (true == pDeviceControlStateStore->stPumpCSS.bIsNeedOffWhenCWTankEmpty && RUN_MODE_UPTAKE == eCurMode)
    {
        return PUMP_MODE_STEP_OFF;
    }

    // 躺平水量
    if (get_flat_status() == true)
    {
        if (RUN_MODE_AUTO == eCurMode)
        {
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
#else
            return PUMP_AUTO_FLAT_FLOW;
#endif
        }
        else if (RUN_MODE_UPTAKE == eCurMode)
        {
            return PUMP_UPTAKE_FLAT_FLOW;
        }
        else if (RUN_MODE_MAX == eCurMode)
        {
            return PUMP_MAX_FLAT_FLOW;
        }
        else if (RUN_MODE_LIGHT == eCurMode)
        {
            return PUMP_LIGHT_FLAT_FLOW;
        }
        else if (RUN_MODE_HOTWATER == eCurMode)
        {
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
            if (0 == u8PumpModeStep)
            {
                return PUMP_MODE_HOTWATER_STEP1;
            }
            else if (1 == u8PumpModeStep)
            {
                return PUMP_MODE_HOTWATER_STEP2;
            }
            else
            {
                return PUMP_MODE_HOTWATER_STEP3;
            }
#endif
        }
        else if (RUN_MODE_STEAM == eCurMode)
        {
            if (STEAM_MODE_LARGE == get_steam_mode())
            {
                return PUMP_LARGE_STEAM_FLAT_FLOW;
            }
            if (pDeviceControlStateStore->stPumpCSS.bIsNeedSteamModeMaxWaterOut == true && pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut == true)
            {
                return COLD_BOILER_PUMP_FLOW;
            }
        }
    }

    // 大水量
    if (pDeviceControlStateStore->stPumpCSS.bIsNeedMaxWaterOut == true)
    {
        // 蒸汽模式大水冲锅炉策略，优先于大水量、冲完后，继续大水量策略，bIsNeedSteamModeMaxWaterOut会在运行后清除、
        if (RUN_MODE_STEAM == eCurMode && pDeviceControlStateStore->stPumpCSS.bIsNeedSteamModeMaxWaterOut)
        {
            return COLD_BOILER_PUMP_FLOW;
        }
        else
        {
            return PUMP_MAX_WATER_OUT_FLOW;
        }
    }

    // 不同模式
    if (RUN_MODE_AUTO == eCurMode)
    {
        // 脏污水量
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) || (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
        if (get_is_run_deep_dirty() && get_flat_status() == false)
#else
        if (get_is_run_deep_dirty())
#endif
        {
            return PUMP_AUTO_DIRTY_FLOW;
        }
#if RR_PROJECT_YAOGUANGALL
        else if (is_pump_maintaining_large_flow())
        {
            return PUMP_AUTO_DIRTY_FLOW;
        }
#endif /*(RR_PROJECT_ID == RR_PROJECT_YAOGUANG)*/
        else
        {
            Gear_App_Level_e eAppDevicesFanLevel = get_addon_set_fan_level();
            Gear_App_Level_e eAppDevicesPumpLevel = get_addon_set_pump_level();
            if (eAppDevicesFanLevel == GEAR_APP_LEVEL_MID)
            {
                if (eAppDevicesPumpLevel == GEAR_APP_LEVEL_MID)
                {
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
                    ePumpMode = AUTO_MODE_FAN_MID_PUMP_MID;
#else
                    if (0 == u8PumpModeStep)
                    {
                        ePumpMode = AUTO_MODE_FAN_MID_PUMP_MID_STEP1;
                    }
                    else if (1 == u8PumpModeStep)
                    {
                        ePumpMode = AUTO_MODE_FAN_MID_PUMP_MID_STEP2;
                    }
                    else if (2 == u8PumpModeStep)
                    {
                        ePumpMode = AUTO_MODE_FAN_MID_PUMP_MID_STEP3;
                    }
#endif
                }
                else if (eAppDevicesPumpLevel == GEAR_APP_LEVEL_HIGH)
                {
                    ePumpMode = AUTO_MODE_FAN_MID_PUMP_HIGH;
                }
                else
                {
                    ePumpMode = AUTO_MODE_FAN_MID_PUMP_LOW;
                }
            }
            else if (eAppDevicesFanLevel == GEAR_APP_LEVEL_HIGH)
            {
                if (eAppDevicesPumpLevel == GEAR_APP_LEVEL_MID)
                {
                    ePumpMode = AUTO_MODE_FAN_HIGH_PUMP_MID;
                }
                else if (eAppDevicesPumpLevel == GEAR_APP_LEVEL_HIGH)
                {
                    ePumpMode = AUTO_MODE_FAN_HIGH_PUMP_HIGH;
                }
                else
                {
                    ePumpMode = AUTO_MODE_FAN_HIGH_PUMP_LOW;
                }
            }
            else if (eAppDevicesFanLevel == GEAR_APP_LEVEL_LOW)
            {
                if (eAppDevicesPumpLevel == GEAR_APP_LEVEL_MID)
                {
                    ePumpMode = AUTO_MODE_FAN_LOW_PUMP_MID;
                }
                else if (eAppDevicesPumpLevel == GEAR_APP_LEVEL_HIGH)
                {
                    ePumpMode = AUTO_MODE_FAN_LOW_PUMP_HIGH;
                }
                else
                {
                    ePumpMode = AUTO_MODE_FAN_LOW_PUMP_LOW;
                }
            }
#if 0
            // 分时水量
            if(0 == u8PumpModeStep)
            {
#ifndef _NOWIFI_VER_
                Gear_App_Level_e eAppDevicesLevel = get_addon_set_pump_level();
                if(GEAR_APP_LEVEL_MID == eAppDevicesLevel)
                {
                    ePumpMode = PUMP_MODE_AUTO_STEP1;
                }
                else if(GEAR_APP_LEVEL_HIGH == eAppDevicesLevel)
                {
                    ePumpMode = PUMP_MODE_AUTO_PLUS_STEP1;
                }
                else
                {
                    ePumpMode = PUMP_MODE_AUTO_MINUS_STEP1;
                }
#else
                ePumpMode = PUMP_MODE_AUTO_STEP1;
#endif
            }
            else if(1 == u8PumpModeStep)
            {
#ifndef _NOWIFI_VER_
                Gear_App_Level_e eAppDevicesLevel = get_addon_set_pump_level();
                if(GEAR_APP_LEVEL_MID == eAppDevicesLevel)
                {
                    ePumpMode = PUMP_MODE_AUTO_STEP2;
                }
                else if(GEAR_APP_LEVEL_HIGH == eAppDevicesLevel)
                {
                    ePumpMode = PUMP_MODE_AUTO_PLUS_STEP2;
                }
                else
                {
                    ePumpMode = PUMP_MODE_AUTO_MINUS_STEP2;
                }
#else
                ePumpMode = PUMP_MODE_AUTO_STEP2;
#endif
            }
            else if(2 == u8PumpModeStep)
            {
#ifndef _NOWIFI_VER_
                Gear_App_Level_e eAppDevicesLevel = get_addon_set_pump_level();
                if(GEAR_APP_LEVEL_MID == eAppDevicesLevel)
                {
                    ePumpMode = PUMP_MODE_AUTO_STEP3;
                }
                else if(GEAR_APP_LEVEL_HIGH == eAppDevicesLevel)
                {
                    ePumpMode = PUMP_MODE_AUTO_PLUS_STEP3;
                }
                else
                {
                    ePumpMode = PUMP_MODE_AUTO_MINUS_STEP3;
                }
#else
                ePumpMode = PUMP_MODE_AUTO_STEP3;
#endif
            }
#endif
        }
    }
    else if (RUN_MODE_UPTAKE == eCurMode)
    {
        if (0 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_UPTAKE_STEP1;
        }
        else if (1 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_UPTAKE_STEP2;
        }
        else if (2 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_UPTAKE_STEP3;
        }
    }
    else if (RUN_MODE_MAX == eCurMode)
    {
        if (0 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_MAX_STEP1;
        }
        else if (1 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_MAX_STEP2;
        }
        else if (2 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_MAX_STEP3;
        }
    }
    else if (RUN_MODE_LIGHT == eCurMode)
    {
        if (0 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_LIGHT_STEP1;
        }
        else if (1 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_LIGHT_STEP2;
        }
        else if (2 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_LIGHT_STEP3;
        }
    }
    else if (RUN_MODE_STEAM == eCurMode)
    {
        if (STEAM_MODE_LARGE == get_steam_mode())
        {
            return PUMP_MODE_LARGE_STEAM;
        }
        if (pDeviceControlStateStore->stPumpCSS.bIsNeedSteamModeMaxWaterOut == true && pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut == true)
        {
            return COLD_BOILER_PUMP_FLOW;
        }
        else
        {
            if (0 == u8PumpModeStep)
            {
                ePumpMode = PUMP_MODE_STEAM_STEP1;
            }
            else if (1 == u8PumpModeStep)
            {
                ePumpMode = PUMP_MODE_STEAM_STEP2;
            }
        }
    }
    else if (RUN_MODE_HOTWATER == eCurMode)
    {
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
        // 启用热水自适应
        ePumpMode = PUMP_MODE_FOR_HOT_WATER_SELF_ADAPTION;
#endif
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
        if (0 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_HOTWATER_STEP1;
        }
        else if (1 == u8PumpModeStep)
        {
            ePumpMode = PUMP_MODE_HOTWATER_STEP2;
        }
        else
        {
            ePumpMode = PUMP_MODE_HOTWATER_STEP3;
        }
#endif
    }

    return ePumpMode;
}

/******************************************************************************
 * 锅炉
 * 目前仅针对YGL
 * 更新时机
 * 1. 触发/解除 NTC保护的时候
 * 2. 触发/解除 缺水保护的时候
 * 3. 热水模式下 泵阶段发生变化的时候
 */
uint16_t get_current_boiler_power(Run_Mode_e eCurMode)
{
    uint8_t u8PumpModeStep = get_run_pump_mode_step();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();

    // 触发保护时 返回0
    if (pDeviceControlStateStore->stBoilerCSS.bTriggerNTCProtect == true ||
        pDeviceControlStateStore->stBoilerCSS.bTriggerEmptyProtect == true)
    {
        return 0;
    }

    // 滋水时
    if (pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyWater == true)
    {
        if (eCurMode == RUN_MODE_HOTWATER || DP_INDEX_HOTAWTER_KEY_ALLMODE_HOTWATER == get_addon_set_spary_water_mode())
        {
            return BOILER_POWER_SPARY_WATER;
        }
    }

    // 热水模式
    if (eCurMode == RUN_MODE_HOTWATER)
    {
        if (u8PumpModeStep == 0)
        {
            return BOILER_POWER_HOTWATER_STEP1;
        }
        else if (u8PumpModeStep == 2)
        {
            return BOILER_POWER_HOTWATER_STEP2;
        }
        else
        {
            return BOILER_POWER_HOTWATER_STEP3;
        }
    }

    return 0;
}

void rr_devices_control()
{
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pDeviceControlStateStore)
    {
        return;
    }

    if (true == pDeviceControlStateStore->stPumpCSS.bPumpParaChanged)
    {
        set_pump_mode(pDeviceControlStateStore->stPumpCSS.ePumpMode);
        pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = false;
    }
    if (true == pDeviceControlStateStore->stFanCSS.bFanParaChanged)
    {
        set_fan_power(pDeviceControlStateStore->stFanCSS.u8FanPower);
        pDeviceControlStateStore->stFanCSS.bFanParaChanged = false;
    }
    if (true == pDeviceControlStateStore->stBrushCSS.bBrushParaChanged)
    {
        set_brush(pDeviceControlStateStore->stBrushCSS.eBrushDir, pDeviceControlStateStore->stBrushCSS.eBrushMode);
        pDeviceControlStateStore->stBrushCSS.bBrushParaChanged = false;
    }
#if ((RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX))
    if (true == pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged)
    {
        set_boiler_power(pDeviceControlStateStore->stBoilerCSS.u16Power);
        pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = false;
    }
#endif
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
    if (true == pDeviceControlStateStore->stSteamPumpCSS.bSteamPumpParaChanged)
    {
        set_steam_pump_param((steamPumpFlow_e)pDeviceControlStateStore->stSteamPumpCSS.eSteamPumpCurFlow);
        pDeviceControlStateStore->stSteamPumpCSS.bSteamPumpParaChanged = false;
    }
#endif
    if (true == pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged)
    {
        rr_brush_solenoid_valve_param_set(pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus);
        pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = false;
    }
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
    if (true == pDeviceControlStateStore->stFoamPumpCSS.bFoamPumpParaChanged)
    {
        set_foampump_vol(pDeviceControlStateStore->stFoamPumpCSS.u16FoamPumpVoltage);
        pDeviceControlStateStore->stFoamPumpCSS.bFoamPumpParaChanged = false;
    }
    if (true == pDeviceControlStateStore->stAirPumpCSS.bAirPumpParaChanged)
    {
        set_airpump_vol(pDeviceControlStateStore->stAirPumpCSS.u16AirPumpVoltage);
        pDeviceControlStateStore->stAirPumpCSS.bAirPumpParaChanged = false;
    }
#endif
}

bool is_max_water_out_running()
{
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pDeviceControlStateStore)
    {
        return false;
    }
    if (true == pDeviceControlStateStore->stPumpCSS.bIsNeedMaxWaterOut && get_current_state_id() == SID_Run)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_steam_mode_max_water_out_running()
{
    RoboStateStore_st *pstRoboState = get_robo_state_store_point();
    if (pstRoboState == NULL)
    {
        return false;
    }
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pDeviceControlStateStore)
    {
        return false;
    }
    if (true == pDeviceControlStateStore->stPumpCSS.bIsNeedSteamModeMaxWaterOut && get_current_state_id() == SID_Run && pstRoboState->eCurRunMode == RUN_MODE_STEAM && STEAM_MODE_NORMAL == get_steam_mode())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void set_close_fan_keep_2s_flag(u16 delay_audio)
{
    RoboStateStore_st *pstRoboState = get_robo_state_store_point();
    s_u16CloseFanKeep2sFlag = delay_audio;
    s_u16DelayAudioID = delay_audio;

    if (delay_audio)
    {
        if (get_flat_status() == true)
        {
            set_fan_power(get_current_fan_power(pstRoboState->eCurRunMode)); // 躺平模式洗地模式停止，风机仍以躺平功率运行x秒
        }
        else
        {
            set_fan_power(FAN_SLOW_STOP_POWER); // 洗地模式停止，风机仍以120W运行x秒
        }
    }
}

static void refresh_devices_argument(void)
{
    RoboStateStore_st *pstRoboState = get_robo_state_store_point();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();

    if (pstRoboState == NULL)
    {
        return;
    }
    if (pDeviceControlStateStore == NULL)
    {
        return;
    }
    pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pstRoboState->eCurRunMode);
    pDeviceControlStateStore->stFanCSS.u8FanPower = get_current_fan_power(pstRoboState->eCurRunMode);
    pDeviceControlStateStore->stBrushCSS.u16BrushVoltage = get_current_brush_voltage(pstRoboState->eCurRunMode);
    pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
    pDeviceControlStateStore->stBrushCSS.bBrushParaChanged = true;
    pDeviceControlStateStore->stFanCSS.bFanParaChanged = true;
    rr_devices_control();
}

u16 get_delay_audio_id(void)
{
    uint16_t temp = s_u16DelayAudioID;
    s_u16DelayAudioID = 0xff;
    return temp;
}

u16 get_close_fan_keep_2s_flag(void)
{
    return s_u16CloseFanKeep2sFlag;
}

void reset_pump_fan_phase_time()
{
    // 如下四个情况都重新计时：
    // 1. 遇到脏污，
    // 2. 遇到故障
    // 3. 切换模式
    // 4. 切换到躺平状态
    s_u64PumpFanSwitchModeTime = get_ms_tick();
    s_u8PumpModeStep = 0;
    s_u8LastPumpModeStep = 0XFF;
}

uint64_t get_pump_fan_phase_time()
{
    return s_u64PumpFanSwitchModeTime;
}

uint8_t get_run_pump_mode_step(void)
{
    return s_u8PumpModeStep;
}

void set_run_pump_mode_step(uint8_t u8PumpModeStep)
{
    s_u8PumpModeStep = u8PumpModeStep;
}

#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
void stop_spraying_foam(DeviceControlStateStore_st *pDeviceControlStateStore)
{
    // 喷泡沫停止： 气泵继续工作1.5s开始缓停1s
    if (get_hot_key_up_airpump_tick() != INVALID_TIME && get_ms_tick() - get_hot_key_up_airpump_tick() >= AIR_PUMP_DELAY_WORK_HK_UP)
    {
        set_hot_key_up_airpump_tick(INVALID_TIME);
        pDeviceControlStateStore->stAirPumpCSS.u16AirPumpVoltage = 0;
        pDeviceControlStateStore->stAirPumpCSS.bAirPumpParaChanged = true;
        rr_devices_control();
    }
    // 喷泡沫停止： 泡沫泵直接停止 1.5s后电磁阀开启，开启1.5s后关闭
    if (get_hot_key_up_valve_tick() != INVALID_TIME && get_ms_tick() - get_hot_key_up_valve_tick() >= VAVLE_DELAY_START_HK_UP + VAVLE_DELAY_WORK_HK_UP)
    {
        set_hot_key_up_valve_tick(INVALID_TIME);
        pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_OFF;
        pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
        pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyFoam = false;
        rr_devices_control();
    }
    else if (get_hot_key_up_valve_tick() != INVALID_TIME && get_ms_tick() - get_hot_key_up_valve_tick() >= VAVLE_DELAY_START_HK_UP)
    {
        if (pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus != VALVE_ON)
        {
            pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_ON;
            pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
            rr_devices_control();
        }
    }
    return;
}
#endif

arStateId_e run_enter(RoboEvent_t *RoboEvent, RoboStateStore_st *pRoboStateStore)
{
    rr_println("%s", __func__);
    arStateId_e NextState = SID_Run;
    reset_silent_ota_bootflag_when_force_off_power();

    RoboProject_st *pRoboProject = get_robo_cur_project_point();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pRoboStateStore || NULL == pRoboProject || NULL == pDeviceControlStateStore)
    {
        return NextState;
    }

    // 报错或告警
    if (pRoboStateStore->stWaterTankStateStore.eDirtyWaterTankRightFull == WATERTANK_FULL)
    {
        audio_play(AUDIO_STREAM_CHECK_DIRTY_WATER_PLACE_CORRECT);
        return SID_Error;
    }

    // 灯显
    set_led_run_mode(pRoboStateStore->eCurRunMode);
    set_led_wifi((LED_Blink_e)(get_wifi_led_state()), LED_Direct_Run);
    set_led_dirty(LED_Dirty_Gradual, 0);
    if (pRoboStateStore->bConnectDock == true || pRoboStateStore->bConnectDockComm == true)
    {
        set_led_charge(LED_ON);
    }
    else if (pRoboStateStore->bConnectDock == false && pRoboStateStore->bConnectDockComm == false)
    {
        set_led_charge(LED_OFF);
    }
    if (pRoboStateStore->bConnectDock == true)
    {
        set_led_soc(LED_Breath, LED_Direct_Run, pRoboStateStore->u8CurSoc);
    }
    else
    {
        set_led_soc(LED_ON, LED_Direct_Run, pRoboStateStore->u8CurSoc);
    }
#if (RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC) || (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE) || \
    (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL)
    set_led_icons_at_once();
#elif (RR_LED_MODE == RR_LED_PWM)
    set_led_param(LED_ON_ALWAYS, LED_WHITE, LED_NONE, LEDPERIOD);
    Led_pwm.Led_socstate = LED_SOC_NONE;
#endif

#if RR_PROJECT_YAOGUANGALL
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
    if (stNVRAMData.CustomOptionV2.IsBrushLightOn)
#else // (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
    if ((((pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyWater == true) ||
          (get_flat_status() == true)) &&
         stNVRAMData.CustomOptionV2.IsBrushLightOn == DP_INDEX_BRUSH_LIGHT_FLAT_AND_SPRAY_ON) ||
        stNVRAMData.CustomOptionV2.IsBrushLightOn == DP_INDEX_BRUSH_LIGHT_ALL_ON)
#endif
    {
        set_board_led(true);
    }
    else
    {
        set_board_led(false);
    }
#endif

#if (RR_TEST_VERSION_LIFE_TEST)
    if (!life_test_wheel_off())
    {
        set_power_wheel_open(true);
    }
#else
    set_power_wheel_open(true);
#endif
    // 主机风机
    reset_fan_warning_state_store_cnt(pRoboStateStore);
    set_close_fan_keep_2s_flag(0);
    set_fan_power_on_off(true);

    // 躺平/直立进运行
    switch (pRoboStateStore->eCurRunMode)
    {
    case RUN_MODE_AUTO:
        audio_play(AUDIO_STREAM_AUTO_MODE);
        break;
    case RUN_MODE_MAX:
        audio_play(AUDIO_STREAM_MAX_MODE);
        break;
    case RUN_MODE_LIGHT:
        audio_play(AUDIO_STREAM_LIGHTPOWER_MODE);
        break;
    case RUN_MODE_UPTAKE:
        audio_play(AUDIO_STREAM_DRYING_MODE);
        break;
    case RUN_MODE_HOTWATER:
        pDeviceControlStateStore->stBoilerCSS.u16Power = 0;
        pDeviceControlStateStore->stBoilerCSS.bHaveOpenWhenHotWaterMode = false;
        pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
        set_hot_water_mode_start_tick(get_ms_tick());
        audio_play(AUDIO_STREAM_HOT_WATER_MODE);
        set_led_dirty(LED_Dirty_All_Red, 0);                            // 特殊效果，单独设置
        pDeviceControlStateStore->stPumpCSS.bIsNeedMaxWaterOut = false; // 热水模式无大水量策略
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
        // 进入热水模式，前5s先不开电磁阀
        pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_1_2_OFF;
        pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
#endif
        break;
#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
    case RUN_MODE_STEAM:
        if (pRoboStateStore->eSteamMode == STEAM_MODE_LARGE)
        {
            audio_play(AUDIO_STREAM_TURBO_STEAM);
        }
        else
        {
            audio_play(AUDIO_STREAM_STEAM_MODE);
            audio_play(AUDIO_STREAM_STEAM_MAKING);
            if (pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut == true)
            {
                pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_1_2_ON;
                pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
            }
        }
        set_steam_mode_start_tick(get_ms_tick()); // 蒸汽语音计时
        steam_temperature_detect_restart();

        // yaoguang蒸汽模式，如果温度高于90度，不显示蒸汽制备灯效，直接显示蒸汽运行灯效
        // 如果需要显示蒸汽制备灯效，灯条先全灭
        if (get_boiler_temperature() > BOILER_TEMP_HOT_WATER)
        {
            // set_led_dirty(LED_Dirty_Steam, 0);
            set_led_dirty(LED_Dirty_All_On, 0);
        }
        else
        {
            set_led_dirty(LED_Dirty_All_OFF, 0);
        }
        if (STEAM_MODE_LARGE == get_steam_mode())
        {
            pDeviceControlStateStore->stPumpCSS.bIsNeedMaxWaterOut = false; // 大蒸汽模式无大水量策略
        }
        break;
#endif
    default:
        break;
    }
    pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
    pDeviceControlStateStore->stPumpCSS.bIsNeedOffWhenCWTankEmpty = false;
    pDeviceControlStateStore->stPumpCSS.bIsNeedForEmptyDet = false;
    pDeviceControlStateStore->stFanCSS.bFanParaChanged = true;
    pDeviceControlStateStore->stBrushCSS.bBrushParaChanged = true;
    pDeviceControlStateStore->stBrushCSS.eBrushDir = BRUSH_DIR_CW;
    if (true == get_flat_status())
    {
        set_sewage_full_count(SEWAGE_CHECK_DURATION_COUNT_WITH_FLAT);
        // 躺平开机 无10s大水量
        pDeviceControlStateStore->stPumpCSS.bIsNeedMaxWaterOut = false;
    }
    else // (false == get_flat_status())
    {
        set_sewage_full_count(SEWAGE_CHECK_DURATION_COUNT);
        // 每次下桩第一次直立运行 前10s大水量
    }
    pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pRoboStateStore->eCurRunMode);
    pDeviceControlStateStore->stFanCSS.u8FanPower = get_current_fan_power(pRoboStateStore->eCurRunMode);
    pDeviceControlStateStore->stBrushCSS.u16BrushVoltage = get_current_brush_voltage(pRoboStateStore->eCurRunMode);
    pDeviceControlStateStore->stBrushCSS.eBrushMode = BRUSH_MODE_RUN_VOLTAGE;
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
    pDeviceControlStateStore->stAirPumpCSS.u16AirPumpVoltage = 0;
    pDeviceControlStateStore->stAirPumpCSS.bAirPumpParaChanged = true;
    pDeviceControlStateStore->stFoamPumpCSS.u16FoamPumpVoltage = 0;
    pDeviceControlStateStore->stFoamPumpCSS.bFoamPumpParaChanged = true;
    pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_OFF;
    pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
#endif
    rr_devices_control();

    // 插件
#ifndef _NOWIFI_VER_
#ifdef IS_SUPPORT_BRUSH_BOARD
    power_wheel_app_control_500ms_check();
    set_power_wheel_backup(BackupMode_Stop);
#endif
    sync_data_points_changed_with_run_mode_to_addon(pRoboStateStore->eCurRunMode);
    BB_Clean_ModeDuration_init();
    BB_Brush_ModeDuration_init();
    mark_Wheel_work_start_tick();
    mark_Fan_work_start_tick();
    mark_Pump_work_start_tick();
    mark_brush_work_start_tick();
    mark_mesh_work_start_tick();
#endif
    set_allowed_to_autoselfclean(1);
    // 时间戳
    set_run_start_tick(get_ms_tick());
    set_switch_mode_tick(INVALID_TIME);
    set_clean_box_empty_alarm_tick(INVALID_TIME);
    reset_pump_fan_phase_time();
    set_run_outof_deepdirty_tick(INVALID_TIME);
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
    set_hot_key_up_airpump_tick(INVALID_TIME);
    set_hot_key_down_foampump_tick(INVALID_TIME);
    set_hot_key_up_valve_tick(INVALID_TIME);
    set_hot_key_down_valve_tick(INVALID_TIME);
#endif

    // 缺水检测
    clear_clwater_bucket();
    clear_clean_box_det_history();
    clean_box_empty_det_for_boiler_init();
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
    clear_empty_detect_for_spray();
#endif
    return NextState;
}

void run_leave(RoboEvent_t *RoboEvent, RoboStateStore_st *pRoboStateStore)
{
    rr_println("%s", __func__);
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    if (NULL == pRoboStateStore || NULL == pDeviceControlStateStore)
    {
        set_pump_mode(PUMP_MODE_STEP_OFF);
        set_brush(BRUSH_DIR_CW, BRUSH_MODE_OFF);
        set_fan_power(0);
        set_power_wheel_open(false);
        return;
    }

    if (RUN_MODE_STEAM == pRoboStateStore->eCurRunMode)
    {
        pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut = false;
        // 切挡时，开启大水量冲锅炉，运行后清除、
        pDeviceControlStateStore->stPumpCSS.bIsNeedSteamModeMaxWaterOut = false;
    }

    // 滋水模式标志位
    pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyWater = false;

    // 灯显
    set_led_error_icon(LED_OFF);
    set_led_run_mode(pRoboStateStore->eCurRunMode);
#if (RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC) || (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE) || \
    (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL)
    set_led_icons_at_once();
#endif
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || \
    (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
    set_board_led(false);
#endif
    // 每次下桩第一次直立运行 前10s大水量 运行后清除
    pDeviceControlStateStore->stPumpCSS.bIsNeedMaxWaterOut = false;
    pDeviceControlStateStore->stPumpCSS.bIsNeedForEmptyDet = false;
    pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
    pDeviceControlStateStore->stPumpCSS.ePumpMode = PUMP_MODE_STEP_OFF;
    // 带助力轮的产品支持滚刷回退策略
    pDeviceControlStateStore->stBrushCSS.bBrushParaChanged = true;
    pDeviceControlStateStore->stBrushCSS.eBrushDir = BRUSH_DIR_CW;
#ifdef IS_SUPPORT_BRUSH_BOARD
    pDeviceControlStateStore->stBrushCSS.eBrushMode = BRUSH_MODE_OFF_STILL_ON;
#else
    pDeviceControlStateStore->stBrushCSS.eBrushMode = BRUSH_MODE_OFF;
#endif

    // 锅炉策略
    pDeviceControlStateStore->stBoilerCSS.u16Power = 0;
    pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;

    // 关闭蒸汽泵（刷头板上）
    pDeviceControlStateStore->stSteamPumpCSS.eSteamPumpCurFlow = STEAM_PUMP_0_ML;
    pDeviceControlStateStore->stSteamPumpCSS.bSteamPumpParaChanged = true;

    // 关闭电磁阀（刷头板上）
    pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
    pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_ALL_OFF;

    // 风机缓停
    if (0 == get_close_fan_keep_2s_flag())
    {
        pDeviceControlStateStore->stFanCSS.bFanParaChanged = true;
        pDeviceControlStateStore->stFanCSS.u8FanPower = 0;
    }

    // 气泵、泡沫泵
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
    pDeviceControlStateStore->stFoamPumpCSS.u16FoamPumpVoltage = 0;
    pDeviceControlStateStore->stFoamPumpCSS.bFoamPumpParaChanged = true;
    if (pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyFoam == false) // 若喷泡沫中意外退出运行模式，还需要保留电磁阀泄压策略
    {
        pDeviceControlStateStore->stAirPumpCSS.u16AirPumpVoltage = 0;
        pDeviceControlStateStore->stAirPumpCSS.bAirPumpParaChanged = true;
    }
    else
    {
        set_hot_key_up_airpump_tick(get_ms_tick());
        set_hot_key_down_foampump_tick(INVALID_TIME);
        set_hot_key_up_valve_tick(get_ms_tick());
        set_hot_key_down_valve_tick(INVALID_TIME);
        record_FoamMode_work_log();
    }
#endif

    rr_devices_control();
    // 助力轮
    set_power_wheel_open(false);

#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
    // 清除蒸汽模式计时
    set_steam_mode_start_tick(INVALID_TIME);
#endif
#if (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE) || (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL) || ((RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC) && (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX))
    set_led_dirty(LED_Dirty_All_Blue, 0);
    if (get_screen_feature_flag() == 2) // E_SCREEN_STEAM == 2,仅蒸汽退出才需恢复电量
    {
        set_screen_soc();
    }
    set_led_soc(LED_ON, LED_Direct_Run, pRoboStateStore->u8CurSoc);
    set_led_icons_at_once();
#endif

    // 清除热水模式计时
    set_hot_water_mode_start_tick(INVALID_TIME);
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
    set_self_adaption_enter_state(true); // 热水模式，水量自适应计时器清除、
#endif
#ifndef _NOWIFI_VER_
    record_fan_work_log();
    record_pump_work_log();
    record_brush_work_log();
    record_mesh_work_log();
    record_wheel_work_log();
    BB_Clean_SetDuration(get_ms_tick() - get_run_start_tick());
    BB_pocket_deal(BB_ACTTYPE_CLEAN, RoboEvent->EventId);
    if (get_power_wheel_backupmode() != BackupMode_StandLock)
    {
        set_power_wheel_backup(BackupMode_Normal);
    }
#endif

    reset_fan_warning_state_store_cnt(pRoboStateStore);
    smart_wash_dry_actions_when_run_leave(get_ms_tick() - get_run_start_tick());
    set_is_run_deep_dirty(false);

    set_switch_mode_tick(INVALID_TIME);
    set_clean_box_empty_alarm_tick(INVALID_TIME);
    set_run_start_tick(INVALID_TIME);
    set_run_outof_deepdirty_tick(INVALID_TIME);
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
    if (pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyFoam == false) // 若喷泡沫中意外退出运行模式，还需要保留电磁阀泄压策略
    {
        set_hot_key_up_airpump_tick(INVALID_TIME);
        set_hot_key_down_foampump_tick(INVALID_TIME);
        set_hot_key_up_valve_tick(INVALID_TIME);
        set_hot_key_down_valve_tick(INVALID_TIME);
    }
#endif
}

arStateId_e run_running(RoboEvent_t *RoboEvent, RoboStateStore_st *pRoboStateStore)
{
    if (NULL == RoboEvent || NULL == pRoboStateStore)
    {
        return SID_Run;
    }
    arStateId_e NextState = SID_Run;
    RoboEventId EventId = RoboEvent->EventId;

    switch (EventId)
    {
#if RR_TEST_VERSION_LIFE_TEST
    case RE_Life_Test_One_Step_TimeOut:
        NextState = SID_Ready;
        break;
#endif
    /******************************************************
    按键、APP
    *******************************************************/
    case RE_Key_Power_Short:
    case RE_App_Run_Stop:
    case RE_Key_Power_Long_10s:
        NextState = run_event_power_key_process(EventId);
        break;
    case RE_Key_Switch_Short:
    case RE_App_Set_Switch_Mode:
        // case RE_Key_Switch_Long_2s:
        NextState = run_or_ready_event_switch_mode_process(EventId, SID_Run, pRoboStateStore);
        break;
    case RE_App_Switch_Run_Type:
        refresh_devices_argument();
        break;
    case RE_Key_HotWater_Down:
    case RE_Key_HotWater_Up:
    case RE_Key_HotWater_TimeOut10s:
        NextState = run_event_hotwater_key_process(EventId);
    /******************************************************
    刷头、刷头板、AT
    *******************************************************/
    case RE_Brush_OverCurrent:
        NextState = run_event_brush_process(EventId);
        break;
    case RE_Brush_Board_Disconnect:
        set_led_errorcode(LED_ErrorCode_E7, LED_Direct_Run);
        NextState = SID_Error;
        break;
    case RE_Run_Waitfor_AT:
        NextState = SID_AT;
        break;
    /******************************************************
    电池
    *******************************************************/
    case RE_Battery_Discharge_HighTemp:
    case RE_Battery_Discharge_LowTemp:
    case RE_Battery_Undervoltage:
    case RE_Battery_SOC_Lower_20:
    case RE_Battery_SOC_Lower_5:
        NextState = run_event_battery_process(EventId, pRoboStateStore);
        break;
    /******************************************************
    桩
    *******************************************************/
    case RE_Dock_Connect:
        NextState = run_event_dock_process(EventId, pRoboStateStore);
        break;
    /******************************************************
    清水桶
    *******************************************************/
    case RE_CleanWaterTank_Empty_RunStart:
    case RE_CleanWaterTank_Empty_HoldStep3:
    case RE_CleanWaterTank_Empty_HoldStep1:
    case RE_CleanWaterTank_Empty_HoldStep2:
    case RE_CleanWaterTank_Not_Empty:
        NextState = run_event_clean_water_tank_process(EventId, pRoboStateStore);
        break;
    /******************************************************
    污水桶
    *******************************************************/
    case RE_DirtyWaterTank_Right_Full:
    case RE_DirtyWaterTank_NotPlace:
        NextState = run_event_dirty_water_tank_process(EventId);
        break;
    /******************************************************
    直立到躺平、躺平到直立
    *******************************************************/
    case RE_Flat_Detect:
    case RE_Flat_Recover:
        NextState = run_event_flat_process(EventId, pRoboStateStore);
        break;
    /******************************************************
    主机风机
    *******************************************************/
    case RE_Fan_BlockedHole:
    case RE_Fan_BlockedRotor:
    case RE_Fan_OverVoltage:
    case RE_Fan_UnderVoltage:
    case RE_Fan_HWOverCurrent:
    case RE_Fan_SWOverCurrent:
    case RE_Fan_HighTemperature:
    case RE_Fan_Comm_Fail:
    case RE_Fan_Power_0:
    case RE_Fan_BlockedHole_TimeOut_1s:
    case RE_Fan_BlockedRotor_TimeOut_1s:
    case RE_Fan_OverVoltage_TimeOut_1s:
    case RE_Fan_UnderVoltage_TimeOut_1s:
    case RE_Fan_HWOverCurrent_TimeOut_1s:
    case RE_Fan_SWOverCurrent_TimeOut_1s:
    case RE_Fan_HighTemperature_TimeOut_1s:
    case RE_Fan_Comm_Fail_TimeOut_1s:
        NextState = cur_state_event_fan_process(EventId, pRoboStateStore, SID_Run);
        break;
    /******************************************************
    脏污检测
    *******************************************************/
    case RE_DirtyCheck_Run_Into_DeepDirty:
    case RE_DirtyCheck_Run_Outof_DeepDirty:
        NextState = run_event_dirtycheck_process(EventId, pRoboStateStore);
        break;
    /******************************************************
    运行时长相关
    *******************************************************/

    /******************************************************
    锅炉相关
    *******************************************************/
    case RE_Boiler_TemperatureState_Change:
    case RE_Boiler_SlowDown_Update:
    case RE_Boiler_TriggerNTCPortect:
    case RE_Boiler_ReleaseNTCPortect:
    case RE_Boiler_TriggerEmptyPortect:
    case RE_Boiler_ReleaseEmptyPortect:
        NextState = run_event_boiler_process(EventId, pRoboStateStore, SID_Run);
        break;
    case RE_MicroSwitch_Lock:
        NextState = run_event_microswitch_process(EventId);
        break;
#if (IS_MULTIPLE_LIFE_CYCLE_TEST_VERSION)
    case RE_Life_Test_Run_One_Step_TimeOut:
        NextState = SID_Ready;
        break;
#endif

    default:
        break;
    }
    return NextState;
}

RoboEventId run_100ms_trigger(void)
{
    /******************************************************
    寿命测试
    *******************************************************/
#if (IS_MULTIPLE_LIFE_CYCLE_TEST_VERSION)
    if ((get_ms_tick() - get_run_start_tick()) >= s_MultiLifeTest[s_u8CurLifeTestStep].RunTimeMin * MIN_TO_MS)
    {
        s_u8CurLifeTestStep++;
        if (s_u8CurLifeTestStep >= MUTI_LIFE_CYCLE_TEST_STEP_NUM)
        {
            s_u8CurLifeTestStep = 0;
        }
        return RE_Life_Test_Run_One_Step_TimeOut;
    }
#endif

    RoboEventId eRoboEventId = RE_INVALID;
    RoboStateStore_st *pstRoboState = get_robo_state_store_point();
    BatCommDynamicInfo_st *pbat_info = rr_bat_get_dynamic_info();
    RoboProject_st *pRoboProject = get_robo_cur_project_point();
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    PumpControlStateStore_st *pPumpStore = get_robo_pump_control_state_store_point();

#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
    eRoboEventId = empty_detect_for_triggerKey_100ms();
    if (RE_INVALID != eRoboEventId)
    {
        return eRoboEventId;
    }
#endif

    if (NULL == pstRoboState || NULL == pbat_info || NULL == pRoboProject || NULL == pDeviceControlStateStore)
    {
        return RE_INVALID;
    }
    // static uint8_t s_500ms_trigger_tick = 0;

    /******************************************************
    刷头板
    *******************************************************/
#ifdef IS_SUPPORT_BRUSH_BOARD
    if (get_check_brush_connect_heartbeat() == false)
    {
        return RE_Brush_Board_Disconnect;
    }
#endif
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
    if ((stNVRAMData.CustomOptionV2.IsBrushLightOn) && (!get_board_led_state()))
    {
        set_board_led(true);
    }
    else if ((!stNVRAMData.CustomOptionV2.IsBrushLightOn) && (get_board_led_state()))
    {
        set_board_led(false);
    }
#endif
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
    if (stNVRAMData.CustomOptionV2.IsBrushLightOn == DP_INDEX_BRUSH_LIGHT_FLAT_AND_SPRAY_ON && ((get_flat_status() == true) ||
                                                                                                (pDeviceControlStateStore->stDeviceFlagsCSS.bIsSparyWater == true)) ||
        stNVRAMData.CustomOptionV2.IsBrushLightOn == DP_INDEX_BRUSH_LIGHT_ALL_ON)
    {
        if (false == get_board_led_state())
        {
            set_board_led(true);
        }
    }
    else
    {
        if (true == get_board_led_state())
        {
            set_board_led(false);
        }
    }
#endif

    /******************************************************
    电池
    *******************************************************/
    if (pbat_info->uBatWarningInfo.UnderVoltage)
    {
        return RE_Battery_Undervoltage;
    }
    if (pbat_info->uBatWarningInfo.DischargeLowTemperature)
    {
        return RE_Battery_Discharge_LowTemp;
    }
    if (pbat_info->uBatWarningInfo.DischargeHighTemperature)
    {
        return RE_Battery_Discharge_HighTemp;
    }

    /******************************************************
    清水桶、污水桶
    *******************************************************/
    if (pstRoboState->stWaterTankStateStore.eDirtyWaterTankRightFull == WATERTANK_FULL)
    {
#ifndef _NOWIFI_VER_
        get_dirty_full_status();
#endif
        return RE_DirtyWaterTank_Right_Full;
    }
    eRoboEventId = check_if_clean_water_tank_empty(pstRoboState);
    if (RE_INVALID != eRoboEventId)
    {
        return eRoboEventId;
    }

    /******************************************************
    主机风机
    *******************************************************/
    eRoboEventId = check_fan_powerdown_timeout(pstRoboState);
    if (RE_INVALID != eRoboEventId)
    {
        return eRoboEventId;
    }

    /******************************************************
    泵定时
    *******************************************************/
    if ((get_ms_tick() - get_run_start_tick()) > MAX_WATER_OUT_TIME && is_max_water_out_running() == true)
    {
        pDeviceControlStateStore->stPumpCSS.bIsNeedMaxWaterOut = false;
        pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
        pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pstRoboState->eCurRunMode);
        rr_devices_control();
    }

    /******************************************************
    蒸汽模式-泵定时
    *******************************************************/
#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
    //  蒸汽模式会大水量冲锅炉，冲完后停止
    if (RUN_MODE_STEAM == pstRoboState->eCurRunMode)
    {
        if (get_steam_mode_work_time() > STEAM_MODE_MAX_WATER_OUT_TIME && pPumpStore->bIsNeedSteamModeMaxWaterOut == true)
        {
            pPumpStore->bIsNeedSteamModeMaxWaterOut = false;
            pPumpStore->bPumpParaChanged = true;
            pPumpStore->ePumpMode = get_current_pump_flow(pstRoboState->eCurRunMode); // 若大水量没结束，会继续执行大水量
            rr_devices_control();
        }
    }
#endif

    if (RUN_MODE_AUTO == pstRoboState->eCurRunMode)
    {
        if ((get_ms_tick() > get_pump_fan_phase_time()) && ((get_ms_tick() - get_pump_fan_phase_time()) > PUMP_AUTO_STEP2_TIME))
        {
            set_run_pump_mode_step(2);
        }
        else if ((get_ms_tick() > get_pump_fan_phase_time()) && ((get_ms_tick() - get_pump_fan_phase_time()) > PUMP_AUTO_STEP1_TIME))
        {
            set_run_pump_mode_step(1);
        }
    }
    else if (RUN_MODE_STEAM == pstRoboState->eCurRunMode)
    {
        if ((get_ms_tick() > get_pump_fan_phase_time()) && ((get_ms_tick() - get_pump_fan_phase_time()) > PUMP_STEAM_STEP1_TIME))
        {
            set_run_pump_mode_step(1);
        }
    }
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
    else if (RUN_MODE_HOTWATER == pstRoboState->eCurRunMode)
    {
        s_u64HotWaterRunCnt100ms++;
        if (s_u64HotWaterRunCnt100ms < HOTER_WATER_STEP1_100MS_NUM)
        {
            set_run_pump_mode_step(0);
        }
        else if (s_u64HotWaterRunCnt100ms < HOTER_WATER_STEP2_100MS_NUM)
        {
            set_run_pump_mode_step(1);
        }
        else
        {
            set_run_pump_mode_step(2);
        }
    }
#endif

    if (get_run_pump_mode_step() != s_u8LastPumpModeStep)
    {
        rr_println("Pump step change %d-->%d", s_u8LastPumpModeStep, get_run_pump_mode_step());
        LOG_INFO("Pump step change %d-->%d", s_u8LastPumpModeStep, get_run_pump_mode_step());
        s_u8LastPumpModeStep = get_run_pump_mode_step();
        pDeviceControlStateStore->stFanCSS.bFanParaChanged = true;
        pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
        if (RUN_MODE_HOTWATER == pstRoboState->eCurRunMode)
        {
            pDeviceControlStateStore->stBoilerCSS.u16Power = get_current_boiler_power(pstRoboState->eCurRunMode);
            pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
        }
#endif
        pDeviceControlStateStore->stFanCSS.u8FanPower = get_current_fan_power(pstRoboState->eCurRunMode);
        pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pstRoboState->eCurRunMode);
        rr_devices_control();
    }

    if (get_run_outof_deepdirty_time() > PUMP_LARGE_FLOW_MAINTAIN_TIME_MS)
    {
        pDeviceControlStateStore->stPumpCSS.bPumpParaChanged = true;
        pDeviceControlStateStore->stPumpCSS.ePumpMode = get_current_pump_flow(pstRoboState->eCurRunMode);
        rr_devices_control();
        set_run_outof_deepdirty_tick(INVALID_TIME);
    }

    /******************************************************
    锅炉
    *******************************************************/
#if ((RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX))
    if (pDeviceControlStateStore->stBoilerCSS.bTriggerNTCProtect == false && pDeviceControlStateStore->stBoilerCSS.bTriggerEmptyProtect == false)
    {
        if (RUN_MODE_STEAM == pstRoboState->eCurRunMode)
        {
            /**
             * @details
             * 1. 识别到蒸汽模式运行立刻开启锅炉
             * 2. 大蒸汽模式。无大水冲锅炉
             */
            eRoboEventId = steam_out_check_temp_change(pstRoboState, pstRoboState->eSteamMode);
            if (eRoboEventId != RE_INVALID)
            {
                return eRoboEventId;
            }
        }
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
        else if (RUN_MODE_HOTWATER == pstRoboState->eCurRunMode)
        {
            if (get_ms_tick() - get_hot_water_mode_start_tick() > MAX_HOT_WATER_DELAY_TIME && get_ms_tick() - get_hot_water_mode_start_tick() < HOT_WATER_PUMP_FLOW_CHANGE_TIME)
            {
                if (pDeviceControlStateStore->stBoilerCSS.bHaveOpenWhenHotWaterMode == false)
                {
                    pDeviceControlStateStore->stBoilerCSS.bHaveOpenWhenHotWaterMode = true;
                    pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
                    pDeviceControlStateStore->stBoilerCSS.u16Power = BOILER_HOT_WATER_START_POWER;
                    rr_devices_control();
                }

                pDeviceControlStateStore->stBoilerCSS.bNeed10MinPowerDown = true; // 运行10min后，需要降低功率
            }

            // 10min后，锅炉功率切换至190W
            if (get_ms_tick() - get_hot_water_mode_start_tick() >= HOT_WATER_PUMP_FLOW_CHANGE_TIME)
            {
                if (pDeviceControlStateStore->stBoilerCSS.bHaveOpenWhenHotWaterMode == false || pDeviceControlStateStore->stBoilerCSS.bNeed10MinPowerDown == true)
                {
                    pDeviceControlStateStore->stBoilerCSS.bHaveOpenWhenHotWaterMode = true;
                    pDeviceControlStateStore->stBoilerCSS.bBoilerParaChanged = true;
                    pDeviceControlStateStore->stBoilerCSS.u16Power = BOILER_HOT_WATER_LOW_POWER;
                    rr_devices_control();
                }

                pDeviceControlStateStore->stBoilerCSS.bNeed10MinPowerDown = false; // 保证只进入一次
            }
        }
#endif
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
        // 暂无策略
#endif
    }
#endif

    /******************************************************
    蒸汽泵
    *******************************************************/
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
    if (RUN_MODE_STEAM == pstRoboState->eCurRunMode)
    {
        steamPumpFlow_e flowByTemp;
        /**
         * 刷头板NTC失效，返回检测范围内的温度最大值，关闭蒸汽泵
         * 触发锅炉缺水保护时，要关闭蒸汽泵
         *
         * 刷头板NTC正常，且锅炉没有触发缺水保护时，蒸汽泵正常执行逻辑
         */
        if (get_boiler_temperature() >= BOILER_NTC_TEMPMAX || pDeviceControlStateStore->stBoilerCSS.bTriggerEmptyProtect == true)
        {
            flowByTemp = STEAM_PUMP_0_ML;
        }
        else
        {
            // 普通蒸汽模式大水冲锅炉策略，涡轮蒸汽模式无该策略
            if (get_steam_mode() == STEAM_MODE_NORMAL && get_steam_mode_work_time() < STEAM_MODE_MAX_WATER_OUT_TIME && pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut == true)
            {
                flowByTemp = STEAM_PUMP_6_ML;
            }
            // 普通蒸汽模式、涡轮蒸汽模式读取锅炉NTC温度，执行蒸汽泵策略
            else
            {
                flowByTemp = steam_pump_calc_flow_by_tempreture(get_steam_mode(), get_boiler_temperature());
            }
        }
        steam_pump_update_state_store_flow(pDeviceControlStateStore, flowByTemp);
        rr_devices_control();
    }
#endif

    /******************************************************
    电磁阀
    *******************************************************/
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_INT) || (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX)
    if (VALVE_1_2_ON == pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus)
    { // 蒸汽模式中，只有满足大水冲锅炉的条件，才会开启电磁阀。大水冲锅炉5s后关闭电磁阀，此处已到5s
        if (RUN_MODE_STEAM == pstRoboState->eCurRunMode && get_steam_mode() == STEAM_MODE_NORMAL && get_ms_tick() - get_steam_mode_start_tick() >= STEAM_MODE_MAX_WATER_OUT_TIME)
        {
            pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_1_2_OFF;
            pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
            rr_devices_control();
        }
    }
    if (VALVE_1_2_OFF == pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus)
    {
        // 热水模式前5s先不开电磁阀，此处已到5s
        if (RUN_MODE_HOTWATER == pstRoboState->eCurRunMode && get_ms_tick() - get_hot_water_mode_start_tick() >= HotwaterModeTime_5s)
        {
            pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_1_2_ON;
            pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
            rr_devices_control();
        }
    }
#endif

    /******************************************************
    喷泡沫：气泵、泡沫泵、电磁阀
    *******************************************************/
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
    // 若喷泡沫中意外退出运行模式，还需要保留电磁阀泄压策略，抽象停止喷泡沫相关器件动作
    // 喷泡沫停止： stop_spraying_foam(pDeviceControlStateStore);

    // 喷泡沫开始： 气泵12V缓启1s 泡沫泵延迟500ms启动
    if (get_hot_key_down_foampump_tick() != INVALID_TIME && get_ms_tick() - get_hot_key_down_foampump_tick() >= FOAM_PUMP_DELAY_START_HK_DOWN)
    {
        set_hot_key_down_foampump_tick(INVALID_TIME);
        pDeviceControlStateStore->stFoamPumpCSS.u16FoamPumpVoltage = FOAM_PUMP_VOLTAGE;
        pDeviceControlStateStore->stFoamPumpCSS.bFoamPumpParaChanged = true;
        rr_devices_control();
    }
    // 喷泡沫开始：电磁阀立刻开启，500ms后关闭
    if (get_hot_key_down_valve_tick() != INVALID_TIME && get_ms_tick() - get_hot_key_down_valve_tick() >= VAVLE_DELAY_OFF_HK_DOWN)
    {
        set_hot_key_down_valve_tick(INVALID_TIME);
        pDeviceControlStateStore->stSolenoidValveCSS.eSolenoidValveStatus = VALVE_OFF;
        pDeviceControlStateStore->stSolenoidValveCSS.bSolenoidValveParaChanged = true;
        rr_devices_control();
    }
#endif

    /******************************************************
     蒸汽模式语音与屏幕显示进度
    *******************************************************/
#if (RR_SUPPORT_RUN_MODE == RR_MODE_AUTO_STEAM_HOTWATER_UPTAKE)
    if (get_steam_mode_start_tick() != INVALID_TIME && pstRoboState->eCurRunMode == RUN_MODE_STEAM)
    {
        if (get_ms_tick() - get_steam_mode_start_tick() >= TIME_1s && s_u16SteamPlayMode == STEAM_PLAY_STOP)
        {
            if (get_boiler_temperature() <= BOILER_TEMP_HOT_WATER)
            {
                s_u16SteamPlayMode = STEAM_PLAY_LOW_TEMP;
            }
            else
            {
                s_u16SteamPlayMode = STEAM_PLAY_HIGH_TEMP;
            }
            s_u8StreamProgress = 0;
            s_u8StreamProgressPre = 0;
            s_u8StreamStripProgress = 0;
            s_u8StreamStripProgressPre = 0;
#if (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE)
            set_led_seg_for_steammode(0, '0', '0', 0);
            set_led_dirty(LED_Dirty_Steam_Progress, 3);
#elif (RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC)
            if (s_u16SteamPlayMode == STEAM_PLAY_LOW_TEMP)
            {
                set_led_time_for_steammode(LED_NUMBER_MODE_STEAM_PREPARE, (TIME_72s / 1000));
            }
            else if (s_u16SteamPlayMode == STEAM_PLAY_HIGH_TEMP)
            {
                set_led_time_for_steammode(LED_NUMBER_MODE_STEAM_PREPARE, (TIME_32s / 1000));
            }
#endif
        }
#if (RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC)                                                                                                                                                                                                           // 电池通信需要考虑到延迟增加1S给电池灯效的反应时间
        if ((get_ms_tick() - get_steam_mode_start_tick() >= (TIME_72s + TIME_1s) && s_u16SteamPlayMode == STEAM_PLAY_LOW_TEMP) || (get_ms_tick() - get_steam_mode_start_tick() >= (TIME_32s + TIME_1s) && s_u16SteamPlayMode == STEAM_PLAY_HIGH_TEMP)) // 62s=1(语音播报)-1(播报后等1s)-60(计时60s)
        {
            // audio_play(AUDIO_STREAM_STEAM_FINISHED);
            set_steam_mode_start_tick(INVALID_TIME);
            s_u64SteamModeBlinkTick = get_ms_tick();
        }
#elif (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE)
        if ((get_ms_tick() - get_steam_mode_start_tick() >= TIME_72s && s_u16SteamPlayMode == STEAM_PLAY_LOW_TEMP) || (get_ms_tick() - get_steam_mode_start_tick() >= TIME_32s && s_u16SteamPlayMode == STEAM_PLAY_HIGH_TEMP)) // 62s=1(语音播报)-1(播报后等1s)-60(计时60s)
        {
            // audio_play(AUDIO_STREAM_STEAM_FINISHED);
            set_steam_mode_start_tick(INVALID_TIME);
            set_led_seg_for_steammode(1, '0', '0', 1);      // 快闪
            set_led_dirty(LED_Dirty_Breath_Fast_Purple, 0); // 整个灯带快闪
            s_u64SteamModeBlinkTick = get_ms_tick();
        }
#endif
        if (s_u16SteamPlayMode == STEAM_PLAY_LOW_TEMP || s_u16SteamPlayMode == STEAM_PLAY_HIGH_TEMP)
        {
            if (s_u16SteamPlayMode == STEAM_PLAY_LOW_TEMP)
            {
                s_u8StreamProgress = (uint8_t)((get_ms_tick() - get_steam_mode_start_tick()) / TIME_PROGRESS_PERIOD_LOW_TEMP);
                s_u8StreamStripProgress = (uint8_t)((get_ms_tick() - get_steam_mode_start_tick()) / TIME_STRIP_PROGRESS_PERIOD_LOW_TEMP);
            }
            else
            {
                s_u8StreamProgress = (uint8_t)((get_ms_tick() - get_steam_mode_start_tick()) / TIME_PROGRESS_PERIOD_HIGH_TEMP);
                s_u8StreamStripProgress = (uint8_t)((get_ms_tick() - get_steam_mode_start_tick()) / TIME_STRIP_PROGRESS_PERIOD_HIGH_TEMP);
            }
            if (s_u8StreamProgress > s_u8StreamProgressPre)
            {
                s_u8StreamProgressPre = s_u8StreamProgress;
#if (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE)
                set_led_seg_for_steammode(s_u8StreamProgress / 100, '0' + s_u8StreamProgress / 10, '0' + s_u8StreamProgress % 10, 0); //<90度，70s刷新100次，>90度，30s刷新100次
#endif
            }

            if (s_u8StreamStripProgress != s_u8StreamStripProgressPre)
            {
                s_u8StreamStripProgressPre = s_u8StreamStripProgress;
#if (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE)
                set_led_dirty(LED_Dirty_Steam_Progress, 2 + s_u8StreamStripProgress); // 开局先亮2个，按总时间均分12个灯
#endif
            }
        }
    }
    if (s_u64SteamModeBlinkTick != INVALID_TIME && pstRoboState->eCurRunMode == RUN_MODE_STEAM) // 蒸汽模式进度满后的快闪结束与电量逐渐显示
    {
        if (get_ms_tick() - s_u64SteamModeBlinkTick >= TIME_2S)
        {
            s_u64SteamModeBlinkTick = INVALID_TIME;
            set_led_dirty(LED_Dirty_All_On, 0);
            set_screen_soc();
            set_led_soc(LED_Slow_On, LED_Direct_Run, pstRoboState->u8CurSoc);
#if (RR_LED_MODE == RR_LED_DIRECT_IIC_OPTIMIZE_QUE) || (RR_LED_MODE == RR_LED_OPTIMIZE_QUE_GENERAL) || ((RR_LED_MODE == RR_LED_INDIRECT_BAT_IIC) && (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_MIX))
            set_led_icons_at_once();
#endif
        }
    }
#endif
    return eRoboEventId;
}

RoboEventId run_500ms_trigger(void)
{
    DeviceControlStateStore_st *pDeviceControlStateStore = get_robo_device_control_state_store_point();
    RoboStateStore_st *pstRoboState = get_robo_state_store_point();
    // 脏污检测
    run_deep_dirty_judge();
    if (pstRoboState->eCurRunMode == RUN_MODE_HOTWATER && get_ms_tick() - get_hot_water_mode_start_tick() >= WATER_WASH_BOILER_JUDGE_TIME && pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut == false)
    {
        pDeviceControlStateStore->stDeviceFlagsCSS.bNeedSteamModeMaxWaterOut = true;
    }

    // APP遥控助力轮
#ifndef _NOWIFI_VER_
#ifdef IS_SUPPORT_BRUSH_BOARD
    power_wheel_app_control_500ms_check();
#endif
#endif
    return RE_INVALID;
}
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
void reset_hotwater_run_cnt(void)
{
    s_u64HotWaterRunCnt100ms = 0;
}
#endif
// 寿命测试
#if (IS_MULTIPLE_LIFE_CYCLE_TEST_VERSION)
void update_mlct_mode(void)
{
    RoboStateStore_st *pstRoboState = get_robo_state_store_point();
    pstRoboState->eCurRunMode = s_MultiLifeTest[s_u8CurLifeTestStep].eRunMode;
    pstRoboState->eSteamMode = s_MultiLifeTest[s_u8CurLifeTestStep].eSteamMode;
}
#endif

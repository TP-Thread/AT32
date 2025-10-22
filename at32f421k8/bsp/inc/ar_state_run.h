#ifndef AR_STATE_RUN
#define AR_STATE_RUN

#include "main.h"

arStateId_e run_enter(RoboEvent_t *RoboEvent, RoboStateStore_st *pRoboStateStore);
void run_leave(RoboEvent_t *RoboEvent, RoboStateStore_st *pRoboStateStore);
arStateId_e run_running(RoboEvent_t *RoboEvent, RoboStateStore_st *pRoboStateStore);
arStateId_e event_key_switch_short_action(bool bRunAction);
RoboEventId run_submsg_key_filter_valid_event(SUBMSG_STATEMACHINE_KEY_ID_e eSubMsgKey, u32 u32KeyPara);
RoboEventId run_submsg_brush_filter_valid_event(SUBMSG_STATEMACHINE_BRUSH_ID_e eSubMsgBrush, u32 u32BrushPara);
RoboEventId run_submsg_app_filter_valid_event(SUBMSG_STATEMACHINE_APP_ID_e eSubMsgApp, u32 u32AppPara);
RoboEventId run_submsg_battery_filter_valid_event(SUBMSG_STATEMACHINE_BATTERY_ID_e eSubMsgBattery, u32 u32BatteryPara);
RoboEventId run_submsg_dock_filter_valid_event(SUBMSG_STATEMACHINE_DOCK_ID_e eSubMsgDock, u32 u32DockPara);

RoboEventId run_submsg_clean_water_tank_filter_valid_event(SUBMSG_STATEMACHINE_CLEAN_WATER_TANK_ID_e eSubMsgCleanWaterTank,
                                                           u32 u32CleanWaterTankPara);

u16 get_close_fan_keep_2s_flag(void);
u16 get_delay_audio_id(void);
void set_close_fan_keep_2s_flag(u16 delay_audio);
bool is_max_water_out_running();
RoboEventId run_100ms_trigger(void);
RoboEventId run_500ms_trigger(void);

RoboEventId run_submsg_dirtycheck_filter_valid_event(SUBMSG_STATEMACHINE_DIRTYCHECK_ID_e eSubMsgDirtyCheck, u32 u32DirtyCheckPara);
#if RR_PROJECT_YAOGUANGALL
RoboEventId run_submsg_boiler_filter_valid_event(SUBMSG_STATEMACHINE_BOILER_ID_e eSubMsgBoiler, u32 u32BoilerPara);
bool is_steam_mode_max_water_out_running();
#endif
arStateId_e run_or_ready_event_switch_mode_process(RoboEventId eEventId, arStateId_e eCurState, RoboStateStore_st *pRoboStateStore);
uint8_t get_current_fan_power(Run_Mode_e eCurMode);
uint16_t get_current_brush_voltage(Run_Mode_e eCurMode);
Pump_Mode_e get_current_pump_flow(Run_Mode_e eCurMode);
uint16_t get_current_boiler_power(Run_Mode_e eCurMode);
void rr_devices_control();
void reset_pump_fan_phase_time();
uint8_t get_run_pump_mode_step(void);
arStateId_e run_event_boiler_process(RoboEventId eEventId, RoboStateStore_st *pRoboStateStore, arStateId_e CurState);

uint64_t get_run_start_tick(void);
void set_run_start_tick(uint64_t u64Tick);
uint64_t get_switch_mode_tick(void);
void set_switch_mode_tick(uint64_t u64Tick);
uint64_t get_clean_box_empty_alarm_tick(void);
void set_clean_box_empty_alarm_tick(uint64_t u64Tick);
uint64_t get_hot_water_mode_start_tick(void);
void set_hot_water_mode_start_tick(uint64_t u64Tick);
void set_hot_key_up_valve_tick(uint64_t u64Tick);
uint64_t get_hot_key_up_valve_tick();
void set_hot_key_down_valve_tick(uint64_t u64Tick);
uint64_t get_hot_key_down_valve_tick();
void set_hot_key_up_airpump_tick(uint64_t u64Tick);
uint64_t get_hot_key_up_airpump_tick();
void set_hot_key_down_foampump_tick(uint64_t u64Tick);
uint64_t get_hot_key_down_foampump_tick();

Run_Mode_e get_run_mode_enum(RoboProject_st *pProject, uint8_t u8Index);
void set_current_run_mode(RoboProject_st *pProject, uint8_t u8Index);
uint8_t get_current_run_enum_index(RoboProject_st *pProject, Run_Mode_e eCurrentMode);
#if (RR_PROJECT_ID == RR_PROJECT_YAOGUANG_LITE)
void reset_hotwater_run_cnt(void);
#endif
#if (RR_PROJECT_ID == RR_PROJECT_MORPH_PRO_X)
void stop_spraying_foam(DeviceControlStateStore_st *pDeviceControlStateStore);
#endif
// 寿命测试
typedef struct
{
    Run_Mode_e eRunMode;
    SteamMode_e eSteamMode;
    uint8_t RunTimeMin;
} MultiLifeTestSingleStep_st;
void update_mlct_mode(void);

#endif /* AR_STATE_RUN */

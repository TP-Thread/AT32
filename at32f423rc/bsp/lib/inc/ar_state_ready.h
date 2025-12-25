#ifndef AR_STATE_READY
#define AR_STATE_READY

#include "main.h"
#include "ar_thread_state.h"

arStateId_e readyEnter(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore);
void readyLeave(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore);
arStateId_e readyRun(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore);
arEventId_e readyKeyEvent(SUBMSG_STATEMACHINE_KEY_ID_e eSubMsgKey, uint32_t u32KeyPara);

#endif /* AR_STATE_READY */

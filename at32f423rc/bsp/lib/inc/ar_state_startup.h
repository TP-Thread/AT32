#ifndef AR_STATE_STARTUP
#define AR_STATE_STARTUP

#include "main.h"
#include "ar_thread_state.h"

arStateId_e startupEnter(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore);
void startupLeave(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore);
arStateId_e startupRun(arEvent_t *RoboEvent, arStateStore_t *pRoboStateStore);

#endif /* AR_STATE_STARTUP */

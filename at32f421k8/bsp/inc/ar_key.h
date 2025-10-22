#ifndef AR_KEY_H
#define AR_KEY_H

#include "main.h"

typedef enum
{
    AR_KEY_NONE_EVENT = 0,
    AR_KEY_UP_EVENT,
    AR_KEY_DOWN_EVENT,
    AR_KEY_MAX_EVENT,
} arKeyEvent_e;

void arKey1EventSet(arKeyEvent_e eKeyEvent);
void arKey2EventSet(arKeyEvent_e eKeyEvent);

void arCheckKey(void);

#endif /* AR_KEY_H */

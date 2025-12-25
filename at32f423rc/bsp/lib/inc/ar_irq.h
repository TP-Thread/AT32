#ifndef AR_IRQ_H
#define AR_IRQ_H

#include "main.h"

void HAL_UART_Callback(UART_HandleTypeDef *huart);
void HAL_TIM_Callback(TIM_HandleTypeDef *htim);

#endif /* AR_IRQ_H */

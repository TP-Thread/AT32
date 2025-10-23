#ifndef AR_WATER_DET_H
#define AR_WATER_DET_H

/* Includes ------------------------------------------------------------------*/
#include "at32f421_wk_config.h"

extern uint16_t g_au16ADCVal[5];

/* Exported functions prototypes ---------------------------------------------*/
void set_water_pwm_p_high(void);
void set_water_pwm_p_low(void);
void set_water_pwm_n_high(void);
void set_water_pwm_n_low(void);

void rr_water_det(void);
void water_det_value_update(uint16_t new_value);
uint16_t water_det_voltage_mv_get(void);

#endif /* AR_WATER_DET_H */

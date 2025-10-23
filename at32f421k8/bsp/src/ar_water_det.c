/**
 ******************************************************************************
 * @file    ar_water_det.c
 * @author  TP-Thread
 * @brief   Water detect.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ar_water_det.h"

/* Private variables ---------------------------------------------------------*/
uint16_t g_au16ADCVal[5] = {0};
uint16_t g_u16VoltageValue;
uint8_t g_u8Updated;

/* 漏水检测 */
void set_water_pwm_p_high(void)
{
    gpio_bits_set(GPIOB, GPIO_PINS_5);
}

void set_water_pwm_p_low(void)
{
    gpio_bits_reset(GPIOB, GPIO_PINS_5);
}

void set_water_pwm_n_high(void)
{
    gpio_bits_set(GPIOA, GPIO_PINS_11);
}

void set_water_pwm_n_low(void)
{
    gpio_bits_reset(GPIOA, GPIO_PINS_11);
}

void water_det_value_update(uint16_t new_value)
{
    g_u16VoltageValue = new_value;
    g_u8Updated = 1;
}

uint16_t water_det_voltage_mv_get(void)
{
    uint32_t result = g_au16ADCVal[3];
    return 3300 * result / 4095;
}

void rr_water_det(void)
{
    uint16_t new_value = 0;
    if (g_u8Updated)
    {
        new_value = g_u16VoltageValue;
        g_u8Updated = 0;
        printf("water det adc value:%d\r\n", new_value);
    }
}

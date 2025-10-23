/* add user code begin Header */
/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f421_wk_config.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "ar_tim.h"
#include "ar_water_det.h"

/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */
extern uint64_t g_system_runTime;
extern uint16_t g_u16VoltageValue;
/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
 * @brief main function.
 * @param  none
 * @retval none
 */
int main(void)
{
    /* add user code begin 1 */

    /* add user code end 1 */

    /* system clock config. */
    wk_system_clock_config();

    /* config periph clock. */
    wk_periph_clock_config();

    /* nvic config. */
    wk_nvic_config();

    /* timebase config. */
    wk_timebase_init();

    /* init gpio function. */
    wk_gpio_config();

    /* init adc1 function. */
    wk_adc1_init();

    /* init dma1 channel1 */
    wk_dma1_channel1_init();
    /* config dma channel transfer parameter */
    /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
    wk_dma_channel_config(DMA1_CHANNEL1,
                          (uint32_t)&ADC1->odt,
                          (uint32_t)g_au16ADCVal,
                          5);
    dma_channel_enable(DMA1_CHANNEL1, TRUE);

    /* init dma1 channel2 */
    wk_dma1_channel2_init();
    /* config dma channel transfer parameter */
    /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
    wk_dma_channel_config(DMA1_CHANNEL2,
                          (uint32_t)&USART1->dt,
                          DMA1_CHANNEL2_MEMORY_BASE_ADDR,
                          DMA1_CHANNEL2_BUFFER_SIZE);
    dma_channel_enable(DMA1_CHANNEL2, TRUE);

    /* init dma1 channel3 */
    wk_dma1_channel3_init();
    /* config dma channel transfer parameter */
    /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
    wk_dma_channel_config(DMA1_CHANNEL3,
                          (uint32_t)&USART1->dt,
                          DMA1_CHANNEL3_MEMORY_BASE_ADDR,
                          DMA1_CHANNEL3_BUFFER_SIZE);
    dma_channel_enable(DMA1_CHANNEL3, TRUE);

    /* init dma1 channel5 */
    wk_dma1_channel5_init();
    /* config dma channel transfer parameter */
    /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
    wk_dma_channel_config(DMA1_CHANNEL5,
                          (uint32_t)&USART2->dt,
                          DMA1_CHANNEL5_MEMORY_BASE_ADDR,
                          DMA1_CHANNEL5_BUFFER_SIZE);
    dma_channel_enable(DMA1_CHANNEL5, TRUE);

    /* init usart1 function. */
    wk_usart1_init();

    /* init usart2 function. */
    wk_usart2_init();

    /* init exint function. */
    wk_exint_config();

    /* init tmr1 function. */
    wk_tmr1_init();

    /* init tmr3 function. */
    wk_tmr3_init();

    /* init tmr6 function. */
    wk_tmr6_init();

    /* init tmr14 function. */
    wk_tmr14_init();

    /* add user code begin 2 */
    g_system_runTime = 0;
    // pwm_fan_set(1000); // 设置风机PWM占空比约55%

    /* add user code end 2 */

    while (1)
    {
        /* add user code begin 3 */
        rr_water_det();
        wk_delay_ms(1000);

        /* add user code end 3 */
    }
}

/* add user code begin 4 */

/* add user code end 4 */

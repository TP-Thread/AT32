/* add user code begin Header */
/**
 **************************************************************************
 * @file     at32f421_int.c
 * @brief    main interrupt service routines.
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

/* includes ------------------------------------------------------------------*/
#include "at32f421_int.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
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

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/* external variables ---------------------------------------------------------*/
/* add user code begin external variables */

/* add user code end external variables */

/**
 * @brief  this function handles nmi exception.
 * @param  none
 * @retval none
 */
void NMI_Handler(void)
{
    /* add user code begin NonMaskableInt_IRQ 0 */

    /* add user code end NonMaskableInt_IRQ 0 */

    /* add user code begin NonMaskableInt_IRQ 1 */

    /* add user code end NonMaskableInt_IRQ 1 */
}

/**
 * @brief  this function handles hard fault exception.
 * @param  none
 * @retval none
 */
void HardFault_Handler(void)
{
    /* add user code begin HardFault_IRQ 0 */

    /* add user code end HardFault_IRQ 0 */
    /* go to infinite loop when hard fault exception occurs */
    while (1)
    {
        /* add user code begin W1_HardFault_IRQ 0 */

        /* add user code end W1_HardFault_IRQ 0 */
    }
}

/**
 * @brief  this function handles memory manage exception.
 * @param  none
 * @retval none
 */
void MemManage_Handler(void)
{
    /* add user code begin MemoryManagement_IRQ 0 */

    /* add user code end MemoryManagement_IRQ 0 */
    /* go to infinite loop when memory manage exception occurs */
    while (1)
    {
        /* add user code begin W1_MemoryManagement_IRQ 0 */

        /* add user code end W1_MemoryManagement_IRQ 0 */
    }
}

/**
 * @brief  this function handles bus fault exception.
 * @param  none
 * @retval none
 */
void BusFault_Handler(void)
{
    /* add user code begin BusFault_IRQ 0 */

    /* add user code end BusFault_IRQ 0 */
    /* go to infinite loop when bus fault exception occurs */
    while (1)
    {
        /* add user code begin W1_BusFault_IRQ 0 */

        /* add user code end W1_BusFault_IRQ 0 */
    }
}

/**
 * @brief  this function handles usage fault exception.
 * @param  none
 * @retval none
 */
void UsageFault_Handler(void)
{
    /* add user code begin UsageFault_IRQ 0 */

    /* add user code end UsageFault_IRQ 0 */
    /* go to infinite loop when usage fault exception occurs */
    while (1)
    {
        /* add user code begin W1_UsageFault_IRQ 0 */

        /* add user code end W1_UsageFault_IRQ 0 */
    }
}

/**
 * @brief  this function handles svcall exception.
 * @param  none
 * @retval none
 */
void SVC_Handler(void)
{
    /* add user code begin SVCall_IRQ 0 */

    /* add user code end SVCall_IRQ 0 */
    /* add user code begin SVCall_IRQ 1 */

    /* add user code end SVCall_IRQ 1 */
}

/**
 * @brief  this function handles debug monitor exception.
 * @param  none
 * @retval none
 */
void DebugMon_Handler(void)
{
    /* add user code begin DebugMonitor_IRQ 0 */

    /* add user code end DebugMonitor_IRQ 0 */
    /* add user code begin DebugMonitor_IRQ 1 */

    /* add user code end DebugMonitor_IRQ 1 */
}

/**
 * @brief  this function handles pendsv_handler exception.
 * @param  none
 * @retval none
 */
void PendSV_Handler(void)
{
    /* add user code begin PendSV_IRQ 0 */

    /* add user code end PendSV_IRQ 0 */
    /* add user code begin PendSV_IRQ 1 */

    /* add user code end PendSV_IRQ 1 */
}

/**
 * @brief  this function handles systick handler.
 * @param  none
 * @retval none
 */
void SysTick_Handler(void)
{
    /* add user code begin SysTick_IRQ 0 */
    g_system_runTime++;

    /* add user code end SysTick_IRQ 0 */

    wk_timebase_handler();

    /* add user code begin SysTick_IRQ 1 */

    /* add user code end SysTick_IRQ 1 */
}

/**
 * @brief  this function handles DMA1 Channel 3 & 2 handler.
 * @param  none
 * @retval none
 */
void DMA1_Channel3_2_IRQHandler(void)
{
    /* add user code begin DMA1_Channel3_2_IRQ 0 */

    /* add user code end DMA1_Channel3_2_IRQ 0 */
    /* add user code begin DMA1_Channel3_2_IRQ 1 */

    /* add user code end DMA1_Channel3_2_IRQ 1 */
}

/**
 * @brief  this function handles TMR1 brake overflow trigger and hall handler.
 * @param  none
 * @retval none
 */
void TMR1_BRK_OVF_TRG_HALL_IRQHandler(void)
{
    /* add user code begin TMR1_BRK_OVF_TRG_HALL_IRQ 0 */
    if (tmr_interrupt_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
    {
        // printf("TMR1 update interrupt occurred.\r\n");
        tmr_flag_clear(TMR1, TMR_OVF_FLAG);

        set_water_pwm_p_high();
        set_water_pwm_n_low();
    }
    /* add user code end TMR1_BRK_OVF_TRG_HALL_IRQ 0 */

    /* add user code begin TMR1_BRK_OVF_TRG_HALL_IRQ 1 */

    /* add user code end TMR1_BRK_OVF_TRG_HALL_IRQ 1 */
}

/**
 * @brief  this function handles TMR1 channel handler.
 * @param  none
 * @retval none
 */
void TMR1_CH_IRQHandler(void)
{
    /* add user code begin TMR1_CH_IRQ 0 */
    if (tmr_interrupt_flag_get(TMR1, TMR_C1_FLAG))
    {
        // printf("11111111111111111111111111\r\n");
        tmr_flag_clear(TMR1, TMR_C1_FLAG);

        water_det_value_update(water_det_voltage_mv_get());
    }
    if (tmr_interrupt_flag_get(TMR1, TMR_C2_FLAG))
    {
        // printf("22222222222222222222222222\r\n");
        tmr_flag_clear(TMR1, TMR_C2_FLAG);

        set_water_pwm_p_low();
        set_water_pwm_n_high();
    }
    if (tmr_interrupt_flag_get(TMR1, TMR_C3_FLAG))
    {
        // printf("33333333333333333333333333\r\n");
        tmr_flag_clear(TMR1, TMR_C3_FLAG);

        set_water_pwm_n_low();
    }
    /* add user code end TMR1_CH_IRQ 0 */
    /* add user code begin TMR1_CH_IRQ 1 */

    /* add user code end TMR1_CH_IRQ 1 */
}

/**
 * @brief  this function handles TMR3 handler.
 * @param  none
 * @retval none
 */
void TMR3_GLOBAL_IRQHandler(void)
{
    /* add user code begin TMR3_GLOBAL_IRQ 0 */
    if (tmr_interrupt_flag_get(TMR3, TMR_OVF_FLAG) != RESET)
    {
        // printf("TMR3 update interrupt occurred.\r\n");
        tmr_flag_clear(TMR3, TMR_OVF_FLAG);
    }
    /* add user code end TMR3_GLOBAL_IRQ 0 */

    /* add user code begin TMR3_GLOBAL_IRQ 1 */

    /* add user code end TMR3_GLOBAL_IRQ 1 */
}

/**
 * @brief  this function handles TMR6 handler.
 * @param  none
 * @retval none
 */
void TMR6_GLOBAL_IRQHandler(void)
{
    /* add user code begin TMR6_GLOBAL_IRQ 0 */
    if (tmr_interrupt_flag_get(TMR6, TMR_OVF_FLAG) != RESET)
    {
        // printf("TMR6 update interrupt occurred.\r\n");
        tmr_flag_clear(TMR6, TMR_OVF_FLAG);
    }

    /* add user code end TMR6_GLOBAL_IRQ 0 */

    /* add user code begin TMR6_GLOBAL_IRQ 1 */

    /* add user code end TMR6_GLOBAL_IRQ 1 */
}

/**
 * @brief  this function handles USART1 handler.
 * @param  none
 * @retval none
 */
void USART1_IRQHandler(void)
{
    /* add user code begin USART1_IRQ 0 */

    /* add user code end USART1_IRQ 0 */
    /* add user code begin USART1_IRQ 1 */

    /* add user code end USART1_IRQ 1 */
}

/**
 * @brief  this function handles USART2 handler.
 * @param  none
 * @retval none
 */
void USART2_IRQHandler(void)
{
    /* add user code begin USART2_IRQ 0 */

    /* add user code end USART2_IRQ 0 */
    /* add user code begin USART2_IRQ 1 */

    /* add user code end USART2_IRQ 1 */
}

/* add user code begin 1 */

/* add user code end 1 */

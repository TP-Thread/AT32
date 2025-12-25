/**
 * @file drv_rcc.c
 * @author A-rtos (A-rtos@outlook.com)
 * @brief RCC reset and clock related.
 * @version 0.1
 * @date 2025-12-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "drv_rcc.h"

/**
 * @brief Microsecond delay
 *
 * @param delay Delay in microseconds (delay < 1000)
 */
void wk_delay_us(uint16_t delay)
{
    uint32_t tickwait = delay * 64; // Number of ticks to wait (64MHz / 1MHz = 64 ticks per us)
    uint32_t tickcnt;               // Used to record the number of ticks delayed

    uint32_t reload = SysTick->LOAD; // Reload value
    uint32_t tickstart = SysTick->VAL;

    while (1)
    {
        uint32_t ticknow = SysTick->VAL;
        if (ticknow != tickstart)
        {
            if (ticknow < tickstart)
            {
                tickcnt = tickstart - ticknow;
            }
            else
            {
                tickcnt = reload - ticknow + tickstart;
            }

            if (tickcnt >= tickwait)
            {
                break;
            }
        }
    }
}

/**
 * @brief  打印系统复位原因和时钟频率
 */
void rcc_information_print(void)
{
    crm_clocks_freq_type crm_clocks;

    printf("Reset Reason: ");

    /* Read reset flags */
    if (crm_flag_get(CRM_NRST_RESET_FLAG) != RESET)
        printf("Pin Reset ");
    if (crm_flag_get(CRM_POR_RESET_FLAG) != RESET)
        printf("Power-On/Power-Down Reset ");
    if (crm_flag_get(CRM_SW_RESET_FLAG) != RESET)
        printf("Software Reset ");
    if (crm_flag_get(CRM_WDT_RESET_FLAG) != RESET)
        printf("Independent Watchdog Reset ");
    if (crm_flag_get(CRM_WWDT_RESET_FLAG) != RESET)
        printf("Window Watchdog Reset ");
    if (crm_flag_get(CRM_LOWPOWER_RESET_FLAG) != RESET)
        printf("Low-Power Reset ");
    if (pwc_flag_get(PWC_WAKEUP_FLAG) != RESET)
        printf("Standby Wakeup ");
    if (pwc_flag_get(PWC_STANDBY_FLAG) != RESET)
        printf("Standby Flag ");

    printf("\r\n");

    /* Clear reset flags */
    CRM->ctrlsts_bit.rstfc = TRUE;
    /* Clear PWC flags */
    pwc_flag_clear(PWC_WAKEUP_FLAG);
    pwc_flag_clear(PWC_STANDBY_FLAG);

    /* Get and print current clock frequency */
    crm_clocks_freq_get(&crm_clocks);
    printf("SYSCLK=%ld HCLK=%ld PCLK1=%ld PCLK2=%ld\r\n",
           crm_clocks.sclk_freq,
           crm_clocks.ahb_freq,
           crm_clocks.apb1_freq,
           crm_clocks.apb2_freq);
}

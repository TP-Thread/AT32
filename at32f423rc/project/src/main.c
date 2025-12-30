/* add user code begin Header */
/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 * Copyright (c) 2025, Artery Technology, All rights reserved.
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
#include "at32f423_wk_config.h"
#include "freertos_app.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include <stdio.h>
#include "algo_crc.h"
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

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */
void crc_calculate_example(void)
{
    uint32_t dataBuffer[] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0};
    uint32_t crc_hw = 0;
    uint32_t crc_sw_byte = 0xFFFFFFFF;
    uint32_t crc_sw_word = 0xFFFFFFFF;

    /* 硬件 CRC 按 32 位字处理 */
    crc_data_reset(); /* 复位 CRC 计算单元 */
    crc_hw = crc_block_calculate(dataBuffer, 4);
    printf("\r\n1. 硬件 CRC: 0x%08X\r\n", crc_hw);

    /* 软件 CRC 按字节流处理 */
    crc32_table_init(CRC_DEFAULT_POLYNOMIAL, 0, 0);
    crc_sw_byte = crc32_calculate(crc_sw_byte, (const uint8_t *)dataBuffer, 16);
    printf("2. 软件 CRC: 0x%08X\r\n", crc_sw_byte);

    /* 软件 CRC 按 32 位字处理 */
    crc_sw_word = 0xFFFFFFFF;
    for (int i = 0; i < 4; i++)
    {
        /* 将 32 位数据拆分为 4 个字节，按大端序处理*/
        uint8_t bytes[4];
        bytes[0] = (dataBuffer[i] >> 24) & 0xFF;
        bytes[1] = (dataBuffer[i] >> 16) & 0xFF;
        bytes[2] = (dataBuffer[i] >> 8) & 0xFF;
        bytes[3] = (dataBuffer[i] >> 0) & 0xFF;
        crc_sw_word = crc32_calculate(crc_sw_word, bytes, 4);
    }
    printf("3. 软件 CRC: 0x%08X\r\n", crc_sw_word);
}

/* add user code end 0 */

/**
 * @brief  take some delay for waiting power stable, delay is about 60ms with frequency 8MHz.
 * @param  none
 * @retval none
 */
static void wk_wait_for_power_stable(void)
{
    volatile uint32_t delay = 0;
    for (delay = 0; delay < 50000; delay++)
        ;
}

/**
 * @brief main function.
 * @param  none
 * @retval none
 */
int main(void)
{
    /* add user code begin 1 */

    /* add user code end 1 */

    /* add a necessary delay to ensure that Vdd is higher than the operating
       voltage of battery powered domain (2.57V) when the battery powered
       domain is powered on for the first time and being operated. */
    wk_wait_for_power_stable();

    /* system clock config. */
    wk_system_clock_config();

    /* config periph clock. */
    wk_periph_clock_config();

    /* nvic config. */
    wk_nvic_config();

    /* init gpio function. */
    wk_gpio_config();

    /* init adc1 function. */
    wk_adc1_init();

    /* init usart1 function. */
    wk_usart1_init();

    /* init usart2 function. */
    wk_usart2_init();

    /* init usart3 function. */
    wk_usart3_init();

    /* init usart5 function. */
    wk_usart5_init();

    /* init i2c1 function. */
    wk_i2c1_init();

    /* init crc function. */
    wk_crc_init();

    /* init exint function. */
    wk_exint_config();

    /* add user code begin 2 */
    rcc_information_print();

    /* CRC 校验示例 */
    crc_calculate_example();

    /* add user code end 2 */

    /* init freertos function. */
    wk_freertos_init();

    while (1)
    {
        /* add user code begin 3 */

        /* add user code end 3 */
    }
}

/* add user code begin 4 */

/* add user code end 4 */

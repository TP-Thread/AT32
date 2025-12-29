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
void crc_calculate_example(void);

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

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

/**
 * @brief  CRC 计算示例函数
 * @param  none
 * @retval none
 */
void crc_calculate_example(void)
{
    /* 要计算 CRC 的数据数组 */
    uint32_t dataBuffer[] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0};
    uint32_t dataLength = sizeof(dataBuffer) / sizeof(dataBuffer[0]);
    uint32_t crc_result = 0;

    /* 重置 CRC 计算单元 */
    crc_data_reset();

    /* 计算数据块的 CRC 值 */
    crc_result = crc_block_calculate(dataBuffer, dataLength);

    /* 打印 CRC 计算结果 */
    printf("CRC Calculation Example:\r\n");
    printf("Data: 0x%08X 0x%08X 0x%08X 0x%08X\r\n",
           dataBuffer[0], dataBuffer[1], dataBuffer[2], dataBuffer[3]);
    printf("CRC Result: 0x%08X\r\n\r\n", crc_result);

    /* 示例：逐个字计算 CRC */
    crc_data_reset();
    for (uint32_t i = 0; i < dataLength; i++)
    {
        crc_result = crc_one_word_calculate(dataBuffer[i]);
        printf("After data[%d]: CRC = 0x%08X\r\n", i, crc_result);
    }
    printf("\r\n");
}

/* add user code end 4 */

/* add user code begin Header */
/**
 **************************************************************************
 * @file     at32f423_wk_config.c
 * @brief    work bench config program
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

#include "at32f423_wk_config.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

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

/* add user code end 0 */

/**
  * @brief  system clock config program
  * @note   the system clock is configured as follow:
  *         system clock (sclk)   = (hick / 6 * pll_ns)/(pll_ms * pll_fr)
  *         system clock source   = HICK_VALUE
  *         - lick                = on
  *         - lext                = off
  *         - hick                = on
  *         - hext                = off
  *         - sclk                = 64000000
  *         - ahbdiv              = 1
  *         - ahbclk              = 64000000
  *         - apb1div             = 2
  *         - apb1clk             = 32000000
  *         - apb2div             = 1
  *         - apb2clk             = 64000000
  *         - pll_ns              = 64
  *         - pll_ms              = 1
  *         - pll_fr              = 4
  *         - flash_wtcyc         = 1 cycle
  * @param  none
  * @retval none
  */
void wk_system_clock_config(void)
{
  /* reset crm */
  crm_reset();

  /* config flash psr register */
  flash_psr_set(FLASH_WAIT_CYCLE_1);

  /* enable pwc periph clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  
  /* config ldo voltage */
  pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V1);

  /* enable lick */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

  /* wait till lick is ready */
  while(crm_flag_get(CRM_LICK_STABLE_FLAG) != SET)
  {
  }

  /* enable hick */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);

  /* wait till hick is ready */
  while(crm_flag_get(CRM_HICK_STABLE_FLAG) != SET)
  {
  }

  /* config pll clock resource
  common frequency config list: pll source selected  hick or hext(8mhz)
  _____________________________________________________________________________
  |        |         |         |         |         |         |        |        |
  | sysclk |   150   |   144   |   120   |   108   |   96    |   72   |   36   |
  |________|_________|_________|_________|_________|_________|_________________|
  |        |         |         |         |         |         |        |        |
  |pll_ns  |   75    |   72    |   120   |   108   |   96    |   72   |   72   |
  |        |         |         |         |         |         |        |        |
  |pll_ms  |   1     |   1     |   1     |   1     |   1     |   1    |   1    |
  |        |         |         |         |         |         |        |        |
  |pll_fr  |   FR_2  |   FR_2  |   FR_4  |   FR_4  |   FR_4  |   FR_4 |   FR_8 |
  |________|_________|_________|_________|_________|_________|________|________|

  if pll clock source selects hext with other frequency values, or configure pll to other
  frequency values, please use the at32 new clock  configuration tool for configuration. */
  crm_pll_config(CRM_PLL_SOURCE_HICK, 64, 1, CRM_PLL_FR_4);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }

  /* config ahbclk */
  crm_ahb_div_set(CRM_AHB_DIV_1);

  /* config apb2clk, the maximum frequency of APB2 clock is 150 MHz  */
  crm_apb2_div_set(CRM_APB2_DIV_1);

  /* config apb1clk, the maximum frequency of APB1 clock is 120 MHz  */
  crm_apb1_div_set(CRM_APB1_DIV_2);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }

  /* update system_core_clock global variable */
  system_core_clock_update();
}

/**
  * @brief  config periph clock
  * @param  none
  * @retval none
  */
void wk_periph_clock_config(void)
{
  /* enable gpioa periph clock */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* enable gpiob periph clock */
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* enable gpioc periph clock */
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);

  /* enable gpiod periph clock */
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);

  /* enable gpiof periph clock */
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);

  /* enable crc periph clock */
  crm_periph_clock_enable(CRM_CRC_PERIPH_CLOCK, TRUE);

  /* enable usart2 periph clock */
  crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);

  /* enable usart3 periph clock */
  crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE);

  /* enable usart5 periph clock */
  crm_periph_clock_enable(CRM_USART5_PERIPH_CLOCK, TRUE);

  /* enable i2c1 periph clock */
  crm_periph_clock_enable(CRM_I2C1_PERIPH_CLOCK, TRUE);

  /* enable usart1 periph clock */
  crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);

  /* enable adc1 periph clock */
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);

  /* enable scfg periph clock */
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
}

/**
  * @brief  nvic config
  * @param  none
  * @retval none
  */
void wk_nvic_config(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
  nvic_irq_enable(EXINT0_IRQn, 14, 0);
  nvic_irq_enable(EXINT9_5_IRQn, 14, 0);
  nvic_irq_enable(USART1_IRQn, 15, 0);
  nvic_irq_enable(EXINT15_10_IRQn, 14, 0);
  nvic_irq_enable(USART5_IRQn, 15, 0);
}

/**
  * @brief  init gpio_input/gpio_output/gpio_analog/eventout function.
  * @param  none
  * @retval none
  */
void wk_gpio_config(void)
{
  /* add user code begin gpio_config 0 */

    /* add user code end gpio_config 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin gpio_config 1 */

    /* add user code end gpio_config 1 */

  /* gpio analog config */
  gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
  gpio_init_struct.gpio_pins = GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2 | 
                               GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_7 | 
                               GPIO_PINS_8 | GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_8 | GPIO_PINS_6;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOF, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
  gpio_init_struct.gpio_pins = GPIO_PINS_1 | GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_7 | 
                               GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_15;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_2 | GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12 | 
                               GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15 | GPIO_PINS_3 | GPIO_PINS_4 | 
                               GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* add user code begin gpio_config 2 */

    /* add user code end gpio_config 2 */
}

/**
  * @brief  init exint function.
  * @param  none
  * @retval none
  */
void wk_exint_config(void)
{
  /* add user code begin exint_config 0 */

    /* add user code end exint_config 0 */

  gpio_init_type gpio_init_struct;
  exint_init_type exint_init_struct;

  /* add user code begin exint_config 1 */

    /* add user code end exint_config 1 */

  /* configure the EXINT0 */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_0;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_0;
  exint_init_struct.line_polarity = EXINT_TRIGGER_BOTH_EDGE;
  exint_init(&exint_init_struct);

  /* configure the EXINT8 */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE8);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_8;
  exint_init_struct.line_polarity = EXINT_TRIGGER_BOTH_EDGE;
  exint_init(&exint_init_struct);

  /* configure the EXINT15 */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_15;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE15);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_15;
  exint_init_struct.line_polarity = EXINT_TRIGGER_BOTH_EDGE;
  exint_init(&exint_init_struct);

  /* add user code begin exint_config 2 */

    /* add user code end exint_config 2 */
}

/**
  * @brief  init usart1 function
  * @param  none
  * @retval none
  */
void wk_usart1_init(void)
{
  /* add user code begin usart1_init 0 */

    /* add user code end usart1_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart1_init 1 */

    /* add user code end usart1_init 1 */

  /* configure the TX pin */
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9, GPIO_MUX_7);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the RX pin */
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE10, GPIO_MUX_7);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* config usart1 clock source */
  crm_usart_clock_select(CRM_USART1, CRM_USART_CLOCK_SOURCE_PCLK);

  /* configure param */
  usart_init(USART1, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART1, TRUE);
  usart_receiver_enable(USART1, TRUE);
  usart_parity_selection_config(USART1, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART1, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart1_init 2 */

    /* add user code end usart1_init 2 */

  usart_enable(USART1, TRUE);

  /* add user code begin usart1_init 3 */

    /* add user code end usart1_init 3 */
}

/**
  * @brief  init usart2 function
  * @param  none
  * @retval none
  */
void wk_usart2_init(void)
{
  /* add user code begin usart2_init 0 */

    /* add user code end usart2_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart2_init 1 */

    /* add user code end usart2_init 1 */

  /* configure the TX pin */
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_7);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the RX pin */
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_7);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_3;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* config usart2 clock source */
  crm_usart_clock_select(CRM_USART2, CRM_USART_CLOCK_SOURCE_PCLK);

  /* configure param */
  usart_init(USART2, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART2, TRUE);
  usart_receiver_enable(USART2, TRUE);
  usart_parity_selection_config(USART2, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART2, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart2_init 2 */

    /* add user code end usart2_init 2 */

  usart_enable(USART2, TRUE);

  /* add user code begin usart2_init 3 */

    /* add user code end usart2_init 3 */
}

/**
  * @brief  init usart3 function
  * @param  none
  * @retval none
  */
void wk_usart3_init(void)
{
  /* add user code begin usart3_init 0 */

    /* add user code end usart3_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart3_init 1 */

    /* add user code end usart3_init 1 */

  /* configure the TX pin */
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE10, GPIO_MUX_7);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* configure the RX pin */
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE11, GPIO_MUX_7);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_11;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* config usart3 clock source */
  crm_usart_clock_select(CRM_USART3, CRM_USART_CLOCK_SOURCE_PCLK);

  /* configure param */
  usart_init(USART3, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART3, TRUE);
  usart_receiver_enable(USART3, TRUE);
  usart_parity_selection_config(USART3, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART3, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart3_init 2 */

    /* add user code end usart3_init 2 */

  usart_enable(USART3, TRUE);
  
  /* add user code begin usart3_init 3 */

    /* add user code end usart3_init 3 */
}

/**
  * @brief  init usart5 function
  * @param  none
  * @retval none
  */
void wk_usart5_init(void)
{
  /* add user code begin usart5_init 0 */

    /* add user code end usart5_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart5_init 1 */

    /* add user code end usart5_init 1 */

  /* configure the TX pin */
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE12, GPIO_MUX_10);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_12;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* configure the RX pin */
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE2, GPIO_MUX_8);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* configure param */
  usart_init(USART5, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART5, TRUE);
  usart_receiver_enable(USART5, TRUE);
  usart_parity_selection_config(USART5, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART5, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart5_init 2 */

    /* add user code end usart5_init 2 */

  usart_enable(USART5, TRUE);
  
  /* add user code begin usart5_init 3 */

    /* add user code end usart5_init 3 */
}

/**
  * @brief  init i2c1 function.
  * @param  none
  * @retval none
  */
void wk_i2c1_init(void)
{
  /* add user code begin i2c1_init 0 */

    /* add user code end i2c1_init 0 */

  gpio_init_type gpio_init_struct;

  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin i2c1_init 1 */

    /* add user code end i2c1_init 1 */

  /* configure the SCL pin */
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE8, GPIO_MUX_4);
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init(GPIOB, &gpio_init_struct);

  /* configure the SDA pin */
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE9, GPIO_MUX_4);
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init(GPIOB, &gpio_init_struct);

  /* config i2c1 clock source */
  crm_i2c_clock_select(CRM_I2C1, CRM_I2C_CLOCK_SOURCE_PCLK);
  
  i2c_init(I2C1, 0, 0x20E02D2D);
  i2c_own_address1_set(I2C1, I2C_ADDRESS_MODE_7BIT, 0x0);
  i2c_ack_enable(I2C1, TRUE);
  i2c_clock_stretch_enable(I2C1, TRUE);
  i2c_general_call_enable(I2C1, FALSE);
  
  i2c_analog_filter_enable(I2C1, TRUE);

  /* add user code begin i2c1_init 2 */

    /* add user code end i2c1_init 2 */

  i2c_enable(I2C1, TRUE);

  /* add user code begin i2c1_init 3 */

    /* add user code end i2c1_init 3 */
}

/**
  * @brief  init adc1 function.
  * @param  none
  * @retval none
  */
void wk_adc1_init(void)
{
  /* add user code begin adc1_init 0 */

    /* add user code end adc1_init 0 */

  gpio_init_type gpio_init_struct;
  adc_common_config_type adc_common_struct;  
  adc_base_config_type adc_base_struct;

  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin adc1_init 1 */

    /* add user code end adc1_init 1 */

  /* configure the IN9 pin */
  gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init(GPIOB, &gpio_init_struct);

  adc_reset();
  /* adc_common_settings--------------------------------------------------------------*/
  /* config adc clock source */
  crm_adc_clock_select(CRM_ADC_CLOCK_SOURCE_HCLK);

  adc_common_default_para_init(&adc_common_struct);  
  /* config adc clock division */
  adc_common_struct.div = ADC_HCLK_DIV_2;
  /* config inner temperature sensor and vintrv */
  adc_common_struct.tempervintrv_state = FALSE;
  adc_common_config(&adc_common_struct);  
  
  /* adc_settings------------------------------------------------------------------- */
  adc_base_default_para_init(&adc_base_struct);
  adc_base_struct.sequence_mode = FALSE;
  adc_base_struct.repeat_mode = FALSE;
  adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = 1;
  adc_base_config(ADC1, &adc_base_struct);

  adc_resolution_set(ADC1, ADC_RESOLUTION_12B);

  /* adc_ordinary_conversionmode---------------------------------------------------- */
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9, 1, ADC_SAMPLETIME_6_5);

  /* When "ADC_ORDINARY_TRIG_SOFTWARE" is selected, user can only use software trigger. \
  The software trigger function is adc_ordinary_software_trigger_enable(ADCx, TRUE); */
  adc_ordinary_conversion_trigger_set(ADC1, ADC_ORDINARY_TRIG_SOFTWARE, ADC_ORDINARY_TRIG_EDGE_NONE);

  /* add user code begin adc1_init 2 */

    /* add user code end adc1_init 2 */

  adc_enable(ADC1, TRUE);
  while(adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET);

  /* adc calibration---------------------------------------------------------------- */
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));

  /* add user code begin adc1_init 3 */

    /* add user code end adc1_init 3 */
}

/**
  * @brief  init crc function.
  * @param  none
  * @retval none
  */
void wk_crc_init(void)
{
  /* add user code begin crc_init 0 */

    /* add user code end crc_init 0 */

  crc_init_data_set(0xFFFFFFFF);
  crc_poly_size_set(CRC_POLY_SIZE_32B);
  crc_poly_value_set(0x04C11DB7);
  crc_reverse_input_data_set(CRC_REVERSE_INPUT_NO_AFFECTE);
  crc_reverse_output_data_set(CRC_REVERSE_OUTPUT_NO_AFFECTE);
  crc_data_reset();

  /* add user code begin crc_init 1 */

    /* add user code end crc_init 1 */
}

/* add user code begin 1 */
int __write(int fd, char *pbuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        while (usart_flag_get(USART5, USART_TDBE_FLAG) == RESET)
            ;
        usart_data_transmit(USART5, (uint16_t)(*pbuffer++));
        while (usart_flag_get(USART5, USART_TDC_FLAG) == RESET)
            ;
    }

    return size;
}

/* add user code end 1 */

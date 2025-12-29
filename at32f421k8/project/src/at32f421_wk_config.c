/* add user code begin Header */
/**
 **************************************************************************
 * @file     at32f421_wk_config.c
 * @brief    work bench config program
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

#include "at32f421_wk_config.h"

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
extern uint16_t g_au16ADCVal[5];

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
 *         system clock (sclk)   = hick / 12 * pll_mult
 *         system clock source   = HICK_VALUE
 *         - sclk                = 72000000
 *         - ahbdiv              = 1
 *         - ahbclk              = 72000000
 *         - apb1div             = 2
 *         - apb1clk             = 36000000
 *         - apb2div             = 2
 *         - apb2clk             = 36000000
 *         - pll_mult            = 18
 *         - flash_wtcyc         = 2 cycle
 * @param  none
 * @retval none
 */
void wk_system_clock_config(void)
{
    /* reset crm */
    crm_reset();

    /* config flash psr register */
    flash_psr_set(FLASH_WAIT_CYCLE_2);

    /* enable lick */
    crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

    /* wait till lick is ready */
    while (crm_flag_get(CRM_LICK_STABLE_FLAG) != SET)
    {
    }

    /* enable hick */
    crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);

    /* wait till hick is ready */
    while (crm_flag_get(CRM_HICK_STABLE_FLAG) != SET)
    {
    }

    /* config pll clock resource */
    crm_pll_config(CRM_PLL_SOURCE_HICK, CRM_PLL_MULT_18);

    /* enable pll */
    crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

    /* wait till pll is ready */
    while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
    {
    }

    /* config ahbclk */
    crm_ahb_div_set(CRM_AHB_DIV_1);

    /* config apb2clk */
    crm_apb2_div_set(CRM_APB2_DIV_2);

    /* config apb1clk */
    crm_apb1_div_set(CRM_APB1_DIV_2);

    /* select pll as system clock source */
    crm_sysclk_switch(CRM_SCLK_PLL);

    /* wait till pll is used as system clock source */
    while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
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
    /* enable dma1 periph clock */
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

    /* enable gpioa periph clock */
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

    /* enable gpiob periph clock */
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

    /* enable gpiof periph clock */
    crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);

    /* enable scfg periph clock */
    crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

    /* enable adc1 periph clock */
    crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);

    /* enable usart1 periph clock */
    crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);

    /* enable tmr3 periph clock */
    crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);

    /* enable tmr6 periph clock */
    crm_periph_clock_enable(CRM_TMR6_PERIPH_CLOCK, TRUE);

    /* enable tmr14 periph clock */
    crm_periph_clock_enable(CRM_TMR14_PERIPH_CLOCK, TRUE);

    /* enable usart2 periph clock */
    crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);
}

/**
 * @brief  nvic config
 * @param  none
 * @retval none
 */
void wk_nvic_config(void)
{
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_2);

    NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    nvic_irq_enable(DMA1_Channel3_2_IRQn, 3, 2);
    nvic_irq_enable(TMR3_GLOBAL_IRQn, 3, 2);
    nvic_irq_enable(TMR6_GLOBAL_IRQn, 0, 0);
    nvic_irq_enable(USART1_IRQn, 2, 2);
    nvic_irq_enable(USART2_IRQn, 2, 2);
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

    /* gpio output config */
    gpio_bits_reset(GPIOB, GPIO_PINS_0 | GPIO_PINS_3 | GPIO_PINS_5);
    gpio_bits_reset(GPIOA, GPIO_PINS_11);

    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_3 | GPIO_PINS_5;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(GPIOB, &gpio_init_struct);

    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins = GPIO_PINS_11;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(GPIOA, &gpio_init_struct);

    /* gpio analog config */
    gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
    gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(GPIOF, &gpio_init_struct);

    gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
    gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_8 | GPIO_PINS_15;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(GPIOA, &gpio_init_struct);

    gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
    gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_4 | GPIO_PINS_6 | GPIO_PINS_7 | GPIO_PINS_8;
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

    /* configure the EXINT12 */
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
    gpio_init_struct.gpio_pins = GPIO_PINS_12;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(GPIOA, &gpio_init_struct);

    scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE12);

    exint_default_para_init(&exint_init_struct);
    exint_init_struct.line_enable = TRUE;
    exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
    exint_init_struct.line_select = EXINT_LINE_12;
    exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
    exint_init(&exint_init_struct);

    /* add user code begin exint_config 2 */

    /* add user code end exint_config 2 */
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
    adc_base_config_type adc_base_struct;

    gpio_default_para_init(&gpio_init_struct);

    /* add user code begin adc1_init 1 */

    /* add user code end adc1_init 1 */

    /* gpio--------------------------------------------------------------------*/
    /* configure the IN1 pin */
    gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
    gpio_init_struct.gpio_pins = GPIO_PINS_1;
    gpio_init(GPIOA, &gpio_init_struct);

    /* configure the IN4 pin */
    gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
    gpio_init_struct.gpio_pins = GPIO_PINS_4;
    gpio_init(GPIOA, &gpio_init_struct);

    /* configure the IN5 pin */
    gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
    gpio_init_struct.gpio_pins = GPIO_PINS_5;
    gpio_init(GPIOA, &gpio_init_struct);

    /* configure the IN6 pin */
    gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
    gpio_init_struct.gpio_pins = GPIO_PINS_6;
    gpio_init(GPIOA, &gpio_init_struct);

    /* configure the IN7 pin */
    gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
    gpio_init_struct.gpio_pins = GPIO_PINS_7;
    gpio_init(GPIOA, &gpio_init_struct);

    adc_reset(ADC1);
    crm_adc_clock_div_set(CRM_ADC_DIV_2);

    /* adc_settings----------------------------------------------------------- */
    adc_base_default_para_init(&adc_base_struct);
    adc_base_struct.sequence_mode = TRUE;
    adc_base_struct.repeat_mode = TRUE;
    adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
    adc_base_struct.ordinary_channel_length = 5;
    adc_base_config(ADC1, &adc_base_struct);

    /* adc_ordinary_conversionmode-------------------------------------------- */
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5, 1, ADC_SAMPLETIME_55_5);
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_7, 2, ADC_SAMPLETIME_55_5);
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4, 3, ADC_SAMPLETIME_55_5);
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1, 4, ADC_SAMPLETIME_55_5);
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 5, ADC_SAMPLETIME_55_5);

    /* When "ADCx_ORDINARY_TRIG_SOFTWARE" is selected, user can only use software trigger. \
    The software trigger function is adc_ordinary_software_trigger_enable(ADCx, TRUE); */
    adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);

    adc_ordinary_part_mode_enable(ADC1, FALSE);

    adc_dma_mode_enable(ADC1, TRUE);
    /* add user code begin adc1_init 2 */

    /* add user code end adc1_init 2 */

    adc_enable(ADC1, TRUE);

    /* adc calibration-------------------------------------------------------- */
    adc_calibration_init(ADC1);
    while (adc_calibration_init_status_get(ADC1))
        ;
    adc_calibration_start(ADC1);
    while (adc_calibration_status_get(ADC1))
        ;

    /* add user code begin adc1_init 3 */
    adc_ordinary_software_trigger_enable(ADC1, TRUE);

    /* add user code end adc1_init 3 */
}

/**
 * @brief  init tmr3 function.
 * @param  none
 * @retval none
 */
void wk_tmr3_init(void)
{
    /* add user code begin tmr3_init 0 */

    /* add user code end tmr3_init 0 */

    /* add user code begin tmr3_init 1 */

    /* add user code end tmr3_init 1 */

    /* configure counter settings */
    tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
    tmr_clock_source_div_set(TMR3, TMR_CLOCK_DIV1);
    tmr_period_buffer_enable(TMR3, FALSE);
    tmr_base_init(TMR3, 71, 999);

    /* configure primary mode settings */
    tmr_sub_sync_mode_set(TMR3, FALSE);
    tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_RESET);

    tmr_counter_enable(TMR3, TRUE);

    /* add user code begin tmr3_init 2 */
    tmr_interrupt_enable(TMR3, TMR_OVF_INT, TRUE);

    /* add user code end tmr3_init 2 */
}

/**
 * @brief  init tmr6 function.
 * @param  none
 * @retval none
 */
void wk_tmr6_init(void)
{
    /* add user code begin tmr6_init 0 */

    /* add user code end tmr6_init 0 */

    /* add user code begin tmr6_init 1 */

    /* add user code end tmr6_init 1 */

    /* configure counter settings */
    tmr_cnt_dir_set(TMR6, TMR_COUNT_UP);
    tmr_period_buffer_enable(TMR6, FALSE);
    tmr_base_init(TMR6, 9999, 7199);

    /* configure primary mode settings */
    tmr_primary_mode_select(TMR6, TMR_PRIMARY_SEL_RESET);

    tmr_counter_enable(TMR6, TRUE);

    /* add user code begin tmr6_init 2 */
    /* 开启定时器更新中断 */
    tmr_interrupt_enable(TMR6, TMR_OVF_INT, TRUE);

    /* add user code end tmr6_init 2 */
}

/**
 * @brief  init tmr14 function.
 * @param  none
 * @retval none
 */
void wk_tmr14_init(void)
{
    /* add user code begin tmr14_init 0 */

    /* add user code end tmr14_init 0 */

    gpio_init_type gpio_init_struct;
    tmr_output_config_type tmr_output_struct;

    gpio_default_para_init(&gpio_init_struct);

    /* add user code begin tmr14_init 1 */

    /* add user code end tmr14_init 1 */

    /* configure the tmr14 CH1 pin */
    gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE1, GPIO_MUX_0);
    gpio_init_struct.gpio_pins = GPIO_PINS_1;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init(GPIOB, &gpio_init_struct);

    /* configure counter settings */
    tmr_cnt_dir_set(TMR14, TMR_COUNT_UP);
    tmr_clock_source_div_set(TMR14, TMR_CLOCK_DIV1);
    tmr_period_buffer_enable(TMR14, FALSE);
    tmr_base_init(TMR14, 3599, 0);

    /* configure channel 1 output settings */
    tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    tmr_output_struct.oc_output_state = TRUE;
    tmr_output_struct.occ_output_state = FALSE;
    tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_output_struct.oc_idle_state = FALSE;
    tmr_output_struct.occ_idle_state = FALSE;

    tmr_output_channel_config(TMR14, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
    tmr_channel_value_set(TMR14, TMR_SELECT_CHANNEL_1, 0);
    tmr_output_channel_buffer_enable(TMR14, TMR_SELECT_CHANNEL_1, TRUE);

    tmr_output_channel_immediately_set(TMR14, TMR_SELECT_CHANNEL_1, TRUE);

    tmr_counter_enable(TMR14, TRUE);

    /* add user code begin tmr14_init 2 */

    /* add user code end tmr14_init 2 */
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
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9, GPIO_MUX_1);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = GPIO_PINS_9;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init(GPIOA, &gpio_init_struct);

    /* configure the RX pin */
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE10, GPIO_MUX_1);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = GPIO_PINS_10;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(GPIOA, &gpio_init_struct);

    /* configure param */
    usart_init(USART1, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(USART1, TRUE);
    usart_receiver_enable(USART1, TRUE);
    usart_parity_selection_config(USART1, USART_PARITY_NONE);

    usart_dma_transmitter_enable(USART1, TRUE);

    usart_dma_receiver_enable(USART1, TRUE);

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
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_1);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = GPIO_PINS_2;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init(GPIOA, &gpio_init_struct);

    /* configure the RX pin */
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_1);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = GPIO_PINS_3;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init(GPIOA, &gpio_init_struct);

    /* configure param */
    usart_init(USART2, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(USART2, TRUE);
    usart_receiver_enable(USART2, TRUE);
    usart_parity_selection_config(USART2, USART_PARITY_NONE);

    usart_dma_receiver_enable(USART2, TRUE);

    usart_hardware_flow_control_set(USART2, USART_HARDWARE_FLOW_NONE);

    /* add user code begin usart2_init 2 */

    /* add user code end usart2_init 2 */

    usart_enable(USART2, TRUE);

    /* add user code begin usart2_init 3 */

    /* add user code end usart2_init 3 */
}

/**
 * @brief  init dma1 channel1 for "adc1"
 * @param  none
 * @retval none
 */
void wk_dma1_channel1_init(void)
{
    /* add user code begin dma1_channel1 0 */

    /* add user code end dma1_channel1 0 */

    dma_init_type dma_init_struct;

    dma_reset(DMA1_CHANNEL1);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_LOW;
    dma_init_struct.loop_mode_enable = TRUE;
    dma_init(DMA1_CHANNEL1, &dma_init_struct);

    /* add user code begin dma1_channel1 1 */

    /* add user code end dma1_channel1 1 */
}

/**
 * @brief  init dma1 channel2 for "usart1_tx"
 * @param  none
 * @retval none
 */
void wk_dma1_channel2_init(void)
{
    /* add user code begin dma1_channel2 0 */

    /* add user code end dma1_channel2 0 */

    dma_init_type dma_init_struct;

    dma_reset(DMA1_CHANNEL2);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL2, &dma_init_struct);

    /* add user code begin dma1_channel2 1 */

    /* add user code end dma1_channel2 1 */
}

/**
 * @brief  init dma1 channel3 for "usart1_rx"
 * @param  none
 * @retval none
 */
void wk_dma1_channel3_init(void)
{
    /* add user code begin dma1_channel3 0 */

    /* add user code end dma1_channel3 0 */

    dma_init_type dma_init_struct;

    dma_reset(DMA1_CHANNEL3);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL3, &dma_init_struct);

    /* add user code begin dma1_channel3 1 */

    /* add user code end dma1_channel3 1 */
}

/**
 * @brief  init dma1 channel5 for "usart2_rx"
 * @param  none
 * @retval none
 */
void wk_dma1_channel5_init(void)
{
    /* add user code begin dma1_channel5 0 */

    /* add user code end dma1_channel5 0 */

    dma_init_type dma_init_struct;

    dma_reset(DMA1_CHANNEL5);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL5, &dma_init_struct);

    /* add user code begin dma1_channel5 1 */

    /* add user code end dma1_channel5 1 */
}

/**
 * @brief  config dma channel transfer parameter
 * @param  dmax_channely: DMAx_CHANNELy
 * @param  peripheral_base_addr: peripheral address.
 * @param  memory_base_addr: memory address.
 * @param  buffer_size: buffer size.
 * @retval none
 */
void wk_dma_channel_config(dma_channel_type *dmax_channely, uint32_t peripheral_base_addr, uint32_t memory_base_addr, uint16_t buffer_size)
{
    /* add user code begin dma_channel_config 0 */

    /* add user code end dma_channel_config 0 */

    dmax_channely->dtcnt = buffer_size;
    dmax_channely->paddr = peripheral_base_addr;
    dmax_channely->maddr = memory_base_addr;

    /* add user code begin dma_channel_config 1 */

    /* add user code end dma_channel_config 1 */
}

/* add user code begin 1 */
/* retarget the C library printf function to the USART */
int __write(int fd, char *pbuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        while (usart_flag_get(USART2, USART_TDBE_FLAG) == RESET)
            ;
        usart_data_transmit(USART2, (uint16_t)(*pbuffer++));
        while (usart_flag_get(USART2, USART_TDC_FLAG) == RESET)
            ;
    }

    return size;
}

/* add user code end 1 */

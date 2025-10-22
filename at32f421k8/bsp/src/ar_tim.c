/**
 ******************************************************************************
 * @file    ar_tim.c
 * @author  TP-Thread
 * @brief   Timer.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ar_tim.h"

/* Private variables ---------------------------------------------------------*/
// 中断循环状态控制标志位
// uint8_t flag_tim7 = 0;

/**
 * @brief 风机PWM占空比设置
 *
 * @param pwm_value 最大3600，对应100%占空比
 */
void pwm_fan_set(uint32_t pwm_value)
{
    if (pwm_value > 3600)
        pwm_value = 3600;

    tmr_channel_value_set(TMR14, TMR_SELECT_CHANNEL_1, pwm_value);
    tmr_channel_enable(TMR14, TMR_SELECT_CHANNEL_1, TRUE);
    tmr_counter_enable(TMR14, TRUE);
}

/**
 * @简  述  TIM7 中断回调函数
 * @参  数  无
 * @返回值  无
 */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance == TIM7){
//		//中断处理内容
//		flag_tim7 = 1;  //置位10ms标志位
//		LED_R_Toggle();
//	}
// }

/**
 * @brief  检测是否产生中断
 * @param  None
 * @retval None
 */
// uint8_t TIM7_CheckIrqStatus(void)
// {
// 	//确认中断,进入控制周期
// 	if(flag_tim7 != 0)
// 	{
// 		flag_tim7 = 0;
// 		return 1;
// 	}

// 	return 0;
// }

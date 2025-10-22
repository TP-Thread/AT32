/**
 ******************************************************************************
 * @file    drv_adc.c
 * @author  TP-Thread
 * @brief   ADC检测 - 支持滑动窗口算法.
 ******************************************************************************
 */
#include "ar_adc.h"

float g_fVolScale;

/* 滑动窗口实例 */
static arSlidingWindow_t slider_window = {0};
static arSlidingWindow_t vref_window = {0};

// arADC_t tADCs[] = {
//     {RR_ADC_VBAT_CHANNEL, RR_ADC_VBAT_ID, 0},
//     {RR_ADC_VCHARGE_CHANNEL, RR_ADC_VCHARGE_ID, 0},
// };

/**
 * @brief  ADC原始读取函数
 * @param  hadc: ADC句柄
 * @param  timeout: 超时时间
 * @retval ADC原始值
 */
uint32_t adcRawRead(ADC_HandleTypeDef *hadc, uint32_t timeout)
{
    HAL_ADC_Start(hadc);
    if (HAL_ADC_PollForConversion(hadc, timeout) == HAL_OK)
    {
        return HAL_ADC_GetValue(hadc);
    }
    return 0;
}

/**
 * @brief  使用滑动窗口读取参考电压平均值，进而获得更准确的电压模数转换比例
 * @retval 比例值
 */
float adcVolScaleSet(void)
{
    uint32_t vrefint_raw = adcRawRead(&hadc1, 10);
    uint32_t vrefint_avg = arSlidingWindowAdd(&vref_window, vrefint_raw);

    g_fVolScale = 1.2f / vrefint_avg;

    return g_fVolScale;
}

/**
 * @brief  读取滑动变阻器电压（使用滑动窗口算法）
 * @retval 电压值
 */
float adcSliderGet(void)
{
    uint32_t u32Slider_raw = adcRawRead(&hadc1, 10);
    uint32_t u32Slider_avg = arSlidingWindowAdd(&slider_window, u32Slider_raw);

    float voltage = u32Slider_avg * g_fVolScale;
    return voltage;
}

/**
 * @brief  获取滑动变阻器原始ADC值（滑动窗口平均）
 * @retval 原始ADC平均值
 */
uint32_t adcSliderGetRaw(void)
{
    uint32_t u32Slider_raw = adcRawRead(&hadc1, 8);
    return arSlidingWindowAdd(&slider_window, u32Slider_raw);
}

/**
 * @brief  连续采集多次并填充滑动窗口
 * @param  channel: 通道选择 (0: slider, 1: vref)
 * @param  samples: 采集次数
 * @retval None
 */
void adcSlidingWindowFillUp(uint8_t channel, uint8_t samples)
{
    for (uint8_t i = 0; i < samples; i++)
    {
        if (channel == 0)
        {
            adcSliderGetRaw();
        }
        else if (channel == 1)
        {
            adcVolScaleSet();
        }
    }
}

/**
 * @brief  初始化ADC滑动窗口
 * @retval None
 */
void adcSlidingWindowInit(void)
{
    slidingWindowInit(&slider_window);
    slidingWindowInit(&vref_window);
}

/**
 * @brief  初始化滑动窗口
 * @param  window: 滑动窗口结构体指针
 * @retval None
 */
void slidingWindowInit(arSlidingWindow_t *window)
{
    if (window == NULL)
        return;

    for (uint8_t i = 0; i < SLIDING_WINDOW_SIZE; i++)
    {
        window->buffer[i] = 0;
    }
    window->sum = 0;
    window->index = 0;
    window->count = 0;
    window->is_full = 0;
}

/**
 * @brief  向滑动窗口添加新值
 * @param  window: 滑动窗口结构体指针
 * @param  value: 新的采样值
 * @retval 当前窗口的平均值
 */
uint32_t arSlidingWindowAdd(arSlidingWindow_t *window, uint32_t value)
{
    if (window == NULL)
        return 0;

    /* 如果窗口已满，减去即将被覆盖的旧值 */
    if (window->is_full)
    {
        window->sum -= window->buffer[window->index];
    }

    /* 添加新值 */
    window->buffer[window->index] = value;
    window->sum += value;

    /* 更新索引 */
    window->index = (window->index + 1) % SLIDING_WINDOW_SIZE; // 循环索引

    /* 更新计数 */
    if (!window->is_full)
    {
        window->count++;
        if (window->count >= SLIDING_WINDOW_SIZE)
        {
            window->is_full = 1;
        }
    }

    if (window == NULL || window->count == 0)
        return 0;

    return window->sum / window->count;
}

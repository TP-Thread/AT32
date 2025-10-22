#ifndef DRV_ADC_H
#define DRV_ADC_H

#include "main.h"

// ADC Id
#define ADC_VREF_ID 1
#define ADC_SLIDER_ID 1

// ADC Channel
#define RR_ADC_VBAT_CHANNEL ADC_CHANNEL_12
#define RR_ADC_VCHARGE_CHANNEL ADC_CHANNEL_10

typedef struct
{
    uint8_t adcId;
    uint8_t adcCh;
    uint16_t adcData;
} arADC_t;

/* 滑动窗口相关定义 */
#define SLIDING_WINDOW_SIZE 16 /* 滑动窗口大小，必须是2的幂次方 */

/* 滑动窗口结构体 */
typedef struct
{
    uint32_t buffer[SLIDING_WINDOW_SIZE]; /* 采样值缓冲区 */
    uint32_t sum;                         /* 当前窗口内所有值的和 */
    uint8_t index;                        /* 当前写入位置 */
    uint8_t count;                        /* 已采样的数量 */
    uint8_t is_full;                      /* 窗口是否已满 */
} arSlidingWindow_t;

/* 基础ADC函数 */
uint32_t adcRawRead(ADC_HandleTypeDef *hadc, uint32_t timeout);

float adcVolScaleSet(void);
float adcSliderGet(void);

/* 滑动窗口相关函数 */
void slidingWindowInit(arSlidingWindow_t *window);
uint32_t arSlidingWindowAdd(arSlidingWindow_t *window, uint32_t value);

uint32_t adcSliderGetRaw(void);

void adcSlidingWindowReset(uint8_t channel);
float adcSlidingWindowGetVariance(uint8_t channel);
void adcSlidingWindowFillUp(uint8_t channel, uint8_t samples);

#endif /* DRV_ADC_H */

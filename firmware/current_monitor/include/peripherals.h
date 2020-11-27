#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_adc.h"
#include "stm32l0xx_hal_dma.h"
#include "stm32l0xx_hal_gpio.h"
#include "stm32l0xx_hal_rcc.h"
#include "stm32l0xx_hal_tim.h"

const static uint32_t pwm_frequency = 1000;

void Peripherals_Init(void);

extern TIM_HandleTypeDef TIM_InitStruct;
extern TIM_OC_InitTypeDef TIM_OC_InitStruct;

extern ADC_HandleTypeDef AdcHandle;
extern ADC_ChannelConfTypeDef AdcConf;

/* Variable used to get converted value */
extern uint32_t AdcReading;

#endif

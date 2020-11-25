#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_adc.h"
#include "stm32l0xx_hal_gpio.h"
#include "stm32l0xx_hal_rcc.h"
#include "stm32l0xx_hal_tim.h"

void Peripherals_Init(void);

extern GPIO_InitTypeDef GPIO_InitStruct;
extern GPIO_InitTypeDef SHUNT_PIN_InitStruct;
extern TIM_HandleTypeDef TIM_InitStruct;
extern TIM_OC_InitTypeDef TIM_OC_InitStruct;

/* ADC handle declaration */
extern ADC_HandleTypeDef   AdcHandle;

/* ADC channel configuration structure declaration */
extern ADC_ChannelConfTypeDef     AdcConf;

/* Variable used to get converted value */
extern __IO uint32_t AdcReading;

#endif

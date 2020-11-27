/**
 * Primary executable for the current monitor chip.
 */

#include "stdint.h"
#include "stdlib.h"

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"
#include "stm32l0xx_it.h"

#include "hw.h"
#include "io_utils.h"
#include "peripherals.h"

int main(void) {
  HAL_Init();

  Peripherals_Init();

  float duty_cycle = 0.0;
  uint32_t pulse_period = TIM_InitStruct.Init.Period;

  HAL_GPIO_WritePin(LED_PORT, GREEN_LED_PIN, GPIO_PIN_SET);
  while (1) {
    HAL_GPIO_TogglePin(LED_PORT, GREEN_LED_PIN);
    HAL_Delay(100);

    duty_cycle = read_voltage() * 0.01;
    TIM2->CCR1 = (uint32_t)(pulse_period * duty_cycle);
  }

  return 1;
}

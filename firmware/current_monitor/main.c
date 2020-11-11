/**
 * Primary executable for the current monitor chip.
 */

#include "stdint.h"
#include "stdlib.h"

#include "hw.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"

int main() {
  HAL_Init();

  while(1) {
    HAL_GPIO_WritePin(SHUNT_EN_PORT, SHUNT_EN_PIN, GPIO_PIN_SET);
    HAL_Delay(0x4000);
    HAL_GPIO_WritePin(SHUNT_EN_PORT, SHUNT_EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(0x4000);
  }

  return 1;
}

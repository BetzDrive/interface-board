#include "io_utils.h"

#include "stm32l0xx_hal_gpio.h"

#include "peripherals.h"

/**
 * Returns the voltage read from the ADC. This is very slow as there is no
 * FPU on this chipset.
 */
float read_voltage(void) {
  static float voltage = source_volt_per_count;
  AdcReading = ADC1->DR;
  return voltage * AdcReading;
}

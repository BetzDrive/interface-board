#ifndef __IO_UTILS_H__
#define __IO_UTILS_H__

#define VOLTAGE_DIVIDER(HIGH, LOW) (LOW / (LOW + HIGH))

/* The voltage divider composition as found on the schematic. */
const static float div_multiplier = VOLTAGE_DIVIDER(140000.0f, 6340.0f);
/* The ADC range (12 bit). */
const static float adc_values = 1 << 12;
/* The ADC reference voltage. */
const static float adc_reference_voltage = 3.3f;
/* The voltage a single count on the ADC represents. */
const static float volt_per_count = adc_reference_voltage / adc_values;
/**
 * The source voltage per count.
 *
 * This is the voltage before the voltage divider. This means we must divide
 * by the multiplier to obtain this voltage.
 *
 * v_sampled = v_source * div_multiplier
 * v_source = v_sampled / div_multiplier
 */
const static float source_volt_per_count = volt_per_count / div_multiplier;

float read_voltage(void);

#endif

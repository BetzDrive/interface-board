#include "peripherals.h"

#include "hw.h"

static void SystemClock_Config(void);
static void GPIO_Config(void);
static void TIM_Config(void);
static void ADC_Config(void);

__IO uint32_t AdcReading = 0;

GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitTypeDef SHUNT_PIN_InitStruct = {0};
TIM_HandleTypeDef TIM_InitStruct = {0};
TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
ADC_HandleTypeDef AdcHandle = {0};
ADC_ChannelConfTypeDef AdcConf = {0};

void Peripherals_Init() {
  // Initialize the system clock.
  SystemClock_Config();

  // Configure the GPIO pins.
  GPIO_Config();

  // Configure Timers.
  TIM_Config();

  // Configure ADC reads.
  ADC_Config();
}

void Error_Handler(void) {
  while(1);
}

/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and 
  *         you can add your own implementation.    
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  /* Get the converted value of regular channel */
  AdcReading = HAL_ADC_GetValue(AdcHandle);
}

void ADC_Config() {
  __HAL_RCC_ADC1_CLK_ENABLE();

  /* ### - 1 - Initialize ADC peripheral ################################### */
  /*
   *  Instance                  = ADC1.
   *  OversamplingMode          = Disabled
   *  ClockPrescaler            = PCLK clock with no division.
   *  LowPowerAutoPowerOff      = Disabled (For this exemple continuous mode is enabled with software start)
   *  LowPowerFrequencyMode     = Enabled (To be enabled only if ADC clock is lower than 2.8MHz) 
   *  LowPowerAutoWait          = Disabled (New conversion starts only when the previous conversion is completed)       
   *  Resolution                = 12 bit (increased to 16 bit with oversampler)
   *  SamplingTime              = 7.5 cycles od ADC clock.
   *  ScanConvMode              = Forward 
   *  DataAlign                 = Right
   *  ContinuousConvMode        = Enabled
   *  DiscontinuousConvMode     = Enabled
   *  ExternalTrigConvEdge      = None (Software start)
   *  EOCSelection              = End Of Conversion event
   *  DMAContinuousRequests     = DISABLE
   */

  AdcHandle.Instance = ADC1;
  
  AdcHandle.Init.OversamplingMode      = DISABLE;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
  AdcHandle.Init.LowPowerFrequencyMode = ENABLE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
    
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_7CYCLES_5;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
 
  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 2 - Channel configuration ####################################### */
  /* Select Channel 0 to be converted */
  AdcConf.Channel = ADC_CHANNEL_1;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &AdcConf) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 3 - Start conversion in Interrupt mode ########################## */
  if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

void GPIO_Config() {

  // Enable the GPIO Clock.
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pin = GREEN_LED_PIN | RED_LED_PIN;
  HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin  = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pin  = VOLTAGE_READ_PIN;
  HAL_GPIO_Init(VOLTAGE_READ_PORT, &GPIO_InitStruct);

  // Configure the PWM Pin.
  SHUNT_PIN_InitStruct.Mode = GPIO_MODE_AF_PP;
  SHUNT_PIN_InitStruct.Pull = GPIO_NOPULL;
  SHUNT_PIN_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  SHUNT_PIN_InitStruct.Pin = SHUNT_EN_PIN;
  SHUNT_PIN_InitStruct.Alternate = GPIO_AF5_TIM2;
  HAL_GPIO_Init(SHUNT_EN_PORT, &SHUNT_PIN_InitStruct);
}

/**
 * Configuring TIM2 Channel 1 to run PWM at 1kHz.
 **/
void TIM_Config() {
  __HAL_RCC_TIM2_CLK_ENABLE();

  TIM_InitStruct.Instance = TIM2;

  uint32_t period = (uint32_t)(SystemCoreClock / 1000) - 1;
  /* Clock prescaler is 1. Count on every clock tick. */
  TIM_InitStruct.Init.Prescaler = 0;
  TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
  /* Reset whenever the count reaches the value that equates to the desired
   * frequency as a fraction of the clock frequency. */
  TIM_InitStruct.Init.Period = period;
  /* Don't divide the clock. */
  TIM_InitStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  /* Reset the counter to 0. */
  TIM_InitStruct.Init.AutoReloadPreload = 0;

  TIM_InitStruct.Channel = TIM_CHANNEL_1;

  if (HAL_TIM_PWM_Init(&TIM_InitStruct) != HAL_OK) {
    Error_Handler();
  }

  TIM_OC_InitStruct.OCMode       = TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCPolarity   = TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCFastMode   = TIM_OCFAST_DISABLE;

  /* Set the pulse value for channel 1 to 50% duty cycle */
  TIM_OC_InitStruct.Pulse = period * 0.25;
  if (HAL_TIM_PWM_ConfigChannel(
        &TIM_InitStruct, &TIM_OC_InitStruct, TIM_CHANNEL_1) != HAL_OK
      ) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&TIM_InitStruct, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = HSI
  *            SYSCLK(Hz)                     = 16000000
  *            HCLK(Hz)                       = 16000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable HSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 1;
  RCC_OscInitStruct.MSIState = RCC_MSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Select HSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
}

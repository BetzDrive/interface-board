/**
 * Primary executable for the current monitor chip.
 */

#include "stdint.h"
#include "stdlib.h"

#include "hw.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"
#include "stm32l0xx_hal_rcc.h"
#include "stm32l0xx_hal_tim.h"


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void GPIO_Config(void);
static void TIM_Config(void);

static GPIO_InitTypeDef GPIO_InitStruct;
static GPIO_InitTypeDef SHUNT_PIN_InitStruct;
static TIM_HandleTypeDef TIM_InitStruct;
static TIM_OC_InitTypeDef sConfig;


int main(void) {
  HAL_Init();

  // Initialize the system clock.
  SystemClock_Config();

  // Configure the GPIO pins.
  GPIO_Config();

  // Configure Timers.
  TIM_Config();

  while (1) {
    HAL_GPIO_TogglePin(LED_PORT, GREEN_LED_PIN);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(LED_PORT, RED_LED_PIN);
  }

  return 1;
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
    while(1);
  }

  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;

  /* Set the pulse value for channel 1 to 50% duty cycle */
  sConfig.Pulse = period * 0.25;
  if (HAL_TIM_PWM_ConfigChannel(&TIM_InitStruct, &sConfig, TIM_CHANNEL_1) != HAL_OK) {
    while(1);
  }
  if (HAL_TIM_PWM_Start(&TIM_InitStruct, TIM_CHANNEL_1) != HAL_OK) {
    while(1);
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
    while(1); 
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
    while(1); 
  }
}

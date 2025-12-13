/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Flash all LEDs together - button toggles on/off
  ******************************************************************************
  * All LEDs flash together at the same time
  * Press button to enable/disable the flashing
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private define ------------------------------------------------------------*/
#define BLINK_DELAY_MS 500

/* Private variables ---------------------------------------------------------*/
COM_InitTypeDef BspCOMInit;
volatile uint8_t current_led = 0;  // Track which LED is active (0-4, or 5 for all)

/* GPIO pins for all LEDs */
#define EXT_LED2_PIN GPIO_PIN_6   // PA6 - External LED 2
#define EXT_LED2_PORT GPIOA

#define EXT_LED3_PIN GPIO_PIN_7   // PA7 - External LED 3
#define EXT_LED3_PORT GPIOA

#define EXT_LED4_PIN GPIO_PIN_6   // PB6 - External LED 4
#define EXT_LED4_PORT GPIOB

#define EXT_LED5_PIN GPIO_PIN_10  // PB10 - External LED 5
#define EXT_LED5_PORT GPIOB

/* Function prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Init_All_LEDs(void);
void Toggle_All_LEDs(void);
void Turn_Off_All_LEDs(void);
void Manual_Button_Init(void);

/* USER CODE BEGIN 0 */

/**
  * @brief  Initialize all LED GPIO pins
  */
void Init_All_LEDs(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable GPIO clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // Configure PA6 (LED 2)
  GPIO_InitStruct.Pin = EXT_LED2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EXT_LED2_PORT, &GPIO_InitStruct);

  // Configure PA7 (LED 3)
  GPIO_InitStruct.Pin = EXT_LED3_PIN;
  HAL_GPIO_Init(EXT_LED3_PORT, &GPIO_InitStruct);

  // Configure PB6 (LED 4)
  GPIO_InitStruct.Pin = EXT_LED4_PIN;
  HAL_GPIO_Init(EXT_LED4_PORT, &GPIO_InitStruct);

  // Configure PB10 (LED 5)
  GPIO_InitStruct.Pin = EXT_LED5_PIN;
  HAL_GPIO_Init(EXT_LED5_PORT, &GPIO_InitStruct);

  // Turn off all LEDs initially
  HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED3_PORT, EXT_LED3_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED4_PORT, EXT_LED4_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED5_PORT, EXT_LED5_PIN, GPIO_PIN_RESET);

  printf("All LEDs initialized\r\n");
}

/**
  * @brief  Toggle all LEDs at once
  */
void Toggle_All_LEDs(void)
{
  BSP_LED_Toggle(LED_GREEN);                       // Onboard LED
  HAL_GPIO_TogglePin(EXT_LED2_PORT, EXT_LED2_PIN); // LED 2
  HAL_GPIO_TogglePin(EXT_LED3_PORT, EXT_LED3_PIN); // LED 3
  HAL_GPIO_TogglePin(EXT_LED4_PORT, EXT_LED4_PIN); // LED 4
  HAL_GPIO_TogglePin(EXT_LED5_PORT, EXT_LED5_PIN); // LED 5
}

/**
  * @brief  Toggle the currently active LED(s)
  */
void Toggle_Current_LED(void)
{
  switch(current_led)
  {
    case 0:  // Onboard LED only (PA5)
      BSP_LED_Toggle(LED_GREEN);
      break;
    case 1:  // External LED 2 only (PA6)
      HAL_GPIO_TogglePin(EXT_LED2_PORT, EXT_LED2_PIN);
      break;
    case 2:  // External LED 3 only (PA7)
      HAL_GPIO_TogglePin(EXT_LED3_PORT, EXT_LED3_PIN);
      break;
    case 3:  // External LED 4 only (PB6)
      HAL_GPIO_TogglePin(EXT_LED4_PORT, EXT_LED4_PIN);
      break;
    case 4:  // External LED 5 only (PB10)
      HAL_GPIO_TogglePin(EXT_LED5_PORT, EXT_LED5_PIN);
      break;
    case 5:  // All LEDs together
      Toggle_All_LEDs();
      break;
  }
}

/**
  * @brief  Turn off all LEDs
  */
void Turn_Off_All_LEDs(void)
{
  BSP_LED_Off(LED_GREEN);
  HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED3_PORT, EXT_LED3_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED4_PORT, EXT_LED4_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED5_PORT, EXT_LED5_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  EXTI line detection callback
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_13)
  {
    // Debouncing
    static uint32_t last_interrupt_time = 0;
    uint32_t interrupt_time = HAL_GetTick();

    if (interrupt_time - last_interrupt_time > 200)
    {
      // Turn off all LEDs before switching
      Turn_Off_All_LEDs();

      // Cycle to next LED (0-5)
      current_led = (current_led + 1) % 6;

      // Print which LED/mode is active
      if (current_led == 5)
      {
        printf("Mode: ALL LEDs\r\n");
      }
      else
      {
        printf("Mode: LED %d only\r\n", current_led + 1);
      }
    }

    last_interrupt_time = interrupt_time;
  }
}

/**
  * @brief Configure GPIO pin for button (polling mode - no interrupt)
  */
void Manual_Button_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable GPIOC clock
  __HAL_RCC_GPIOC_CLK_ENABLE();

  // Configure PC13 as simple input (no interrupt)
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  printf("Button initialized (polling mode)\r\n");
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* Initialize COM1 port (115200 baud) */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  printf("\r\n=================================\r\n");
  printf("Individual LED Control\r\n");
  printf("Press button to cycle modes:\r\n");
  printf("LED 1 -> LED 2 -> LED 3 -> LED 4 -> LED 5 -> ALL\r\n");
  printf("=================================\r\n");

  /* Initialize onboard LED */
  BSP_LED_Init(LED_GREEN);

  /* Initialize all external LEDs */
  Init_All_LEDs();

  /* Initialize button using BSP (like the working code) */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  printf("System ready - LED 1 should be blinking\r\n\r\n");

  /* Infinite loop */
  while (1)
  {
    // Blink the currently selected LED(s)
    Toggle_Current_LED();
    HAL_Delay(BLINK_DELAY_MS);
  }
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

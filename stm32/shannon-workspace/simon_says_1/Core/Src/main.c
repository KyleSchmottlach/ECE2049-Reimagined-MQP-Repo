/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Simon Says Game - Watch and remember the sequence!
  ******************************************************************************
  * LEDs flash in a random sequence that gets longer each round
  * Press button to start the next round
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include <stdlib.h>

/* Private define ------------------------------------------------------------*/
#define FLASH_DURATION_MS 400
#define FLASH_GAP_MS 200
#define ROUND_DELAY_MS 2000
#define MAX_SEQUENCE_LENGTH 3

/* Private variables ---------------------------------------------------------*/
COM_InitTypeDef BspCOMInit;
volatile uint8_t button_pressed = 0;

/* Game state */
uint8_t sequence[MAX_SEQUENCE_LENGTH];
uint8_t current_level = 0;
uint8_t game_started = 0;

/* GPIO pins for all LEDs */
#define EXT_LED2_PIN GPIO_PIN_6   // PA6 - External LED 2
#define EXT_LED2_PORT GPIOA

#define EXT_LED3_PIN GPIO_PIN_7   // PA7 - External LED 3
#define EXT_LED3_PORT GPIOA

#define EXT_LED4_PIN GPIO_PIN_6   // PB6 - External LED 4
#define EXT_LED4_PORT GPIOB

#define EXT_LED5_PIN GPIO_PIN_10  // PB10 - External LED 5
#define EXT_LED5_PORT GPIOB

/* LED identifiers */
#define LED_ONBOARD 0
#define LED_2 1
#define LED_3 2
#define LED_4 3
#define LED_5 4
#define NUM_LEDS 5

/* Function prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Init_All_LEDs(void);
void Turn_Off_All_LEDs(void);
void Flash_LED(uint8_t led_id);
void Generate_Next_Sequence(void);
void Play_Sequence(void);
void Celebration_Flash(void);

/* USER CODE BEGIN 0 */

/**
  * @brief  Initialize all LED GPIO pins
  */
void Init_All_LEDs(void){
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
  Turn_Off_All_LEDs();

  printf("All LEDs initialized\r\n");
}

/**
  * @brief  Turn off all LEDs
  */
void Turn_Off_All_LEDs(void){
  BSP_LED_Off(LED_GREEN);
  HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED3_PORT, EXT_LED3_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED4_PORT, EXT_LED4_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED5_PORT, EXT_LED5_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  Flash a specific LED
  * @param  led_id: LED identifier (0-4)
  */
void Flash_LED(uint8_t led_id){
  // Turn on the selected LED
  switch(led_id){

    case LED_ONBOARD:
      BSP_LED_On(LED_GREEN);
      break;
    case LED_2:
      HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_SET);
      break;
    case LED_3:
      HAL_GPIO_WritePin(EXT_LED3_PORT, EXT_LED3_PIN, GPIO_PIN_SET);
      break;
    case LED_4:
      HAL_GPIO_WritePin(EXT_LED4_PORT, EXT_LED4_PIN, GPIO_PIN_SET);
      break;
    case LED_5:
      HAL_GPIO_WritePin(EXT_LED5_PORT, EXT_LED5_PIN, GPIO_PIN_SET);
      break;
  }

  HAL_Delay(FLASH_DURATION_MS);
  Turn_Off_All_LEDs();
  HAL_Delay(FLASH_GAP_MS);
}

/**
  * @brief  Generate the next sequence (adds one more LED to the sequence)
  */
void Generate_Next_Sequence(void){
  if (current_level < MAX_SEQUENCE_LENGTH){
    sequence[current_level] = rand() % NUM_LEDS;
    current_level++;
    printf("Level %d -\r\n", current_level);
  }
}

/**
  * @brief  Play the current sequence
  */
void Play_Sequence(void){

  printf("Playing sequence...\r\n");

  for (uint8_t i = 0; i < current_level; i++){
	  Flash_LED(sequence[i]);
  }
  if (current_level < MAX_SEQUENCE_LENGTH){
	  printf("Sequence complete! Press button for next round.\r\n\r\n");
  }
}

/**
  * @brief  Celebration flash animation
  */
void Celebration_Flash(void)
{
  for (uint8_t i = 0; i < 3; i++){

    // Turn all on
    BSP_LED_On(LED_GREEN);
    HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EXT_LED3_PORT, EXT_LED3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EXT_LED4_PORT, EXT_LED4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EXT_LED5_PORT, EXT_LED5_PIN, GPIO_PIN_SET);
    HAL_Delay(150);

    // Turn all off
    Turn_Off_All_LEDs();
    HAL_Delay(150);
  }
}

/**
  * @brief  EXTI line detection callback
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_13){
    // Debouncing
    static uint32_t last_interrupt_time = 0;
    uint32_t interrupt_time = HAL_GetTick();

    if (interrupt_time - last_interrupt_time > 300){
      button_pressed = 1;
    }

    last_interrupt_time = interrupt_time;
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  */
int main(void){
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
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE){
    Error_Handler();
  }

  printf("\r\n=================================\r\n");
  printf("      SIMON SAYS GAME\r\n");
  printf("=================================\r\n");
  printf("Watch the LED sequence carefully!\r\n");
  printf("Press button to start next round\r\n");
  printf("=================================\r\n\r\n");

  /* Initialize onboard LED */
  BSP_LED_Init(LED_GREEN);

  /* Initialize all external LEDs */
  Init_All_LEDs();

  /* Initialize button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Seed random number generator with current tick */
  srand(HAL_GetTick());

  /* Welcome animation */
  Celebration_Flash();

  printf("Press button to start!\r\n\r\n");

  /* Infinite loop */
  while (1){
      if (button_pressed){
        button_pressed = 0;

        if (!game_started){
          // First press - start the game
          game_started = 1;
          current_level = 0;
          printf("Game started!\r\n\r\n");
        }

        // Check if game is already won
        if (current_level > MAX_SEQUENCE_LENGTH){
          printf("Starting new game...\r\n\r\n");
          game_started = 0;
          current_level = 0;
          Celebration_Flash();
          HAL_Delay(1000);
          printf("Press button to start!\r\n\r\n");
          continue;
        }

        // Generate and play next sequence
        Generate_Next_Sequence();

        HAL_Delay(500);  // Brief pause before showing sequence
        Play_Sequence();

        // Check if player just won
        if (current_level >= MAX_SEQUENCE_LENGTH){
          HAL_Delay(500);
          Celebration_Flash();
          printf("\r\n============================\r\n");
          printf("      *** YOU WIN! ***\r\n");
          printf("You completed all %d levels!", MAX_SEQUENCE_LENGTH);
          printf("\r\n============================\r\n");
          printf("\r\nPress button to play again.\r\n\r\n");
          current_level = 0;
        }
      }

      HAL_Delay(50);  // Small delay to prevent CPU hogging
    }
  }


/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void){
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

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK){
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void){
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

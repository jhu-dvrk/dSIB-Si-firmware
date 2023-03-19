#include <Arduino.h>

int led = PB9;
int button = PB8;
int tx = PB6;
int rx = PB7;

int brake1 = PA7_ALT1;
int brake2 = PA8;
int brake3 = PA9;
int brake4 = PA10;
int brake5 = PA11;

int sense1 = PA1;
int sense2 = PA2;
int sense3 = PA3;
int sense4 = PA4;
int sense5 = PA5;
int sense_mv = PA0;

uint32_t last_release = 0;
int is_released = 0;

int DEBOUNCE_TIME = 100;
int HIGH_POWER_TIME = 200;
int HIGH_POWER = 40 * 255 / 48;
int LOW_POWER = 35 * 255 / 48;
int HIGH_POWER_Z = 45 * 255 / 48;
int LOW_POWER_Z = 40 * 255 / 48;

void setup() {
    pinMode(led, OUTPUT);
    pinMode(rx, INPUT_PULLUP);
    pinMode(button, INPUT_PULLUP);

    pinMode(brake1, OUTPUT);
    pinMode(brake2, OUTPUT);
    pinMode(brake3, OUTPUT);
    pinMode(brake4, OUTPUT);
    pinMode(brake5, OUTPUT);
    digitalWrite(brake1, 0);
    digitalWrite(brake2, 0);
    digitalWrite(brake3, 0);
    digitalWrite(brake4, 0);
    digitalWrite(brake5, 0);

    analogWriteFrequency(20000);
}

void loop () {
    uint32_t t = millis();
    if (digitalRead(rx) == 0 || digitalRead(button) == 0) {
        if (!is_released) {
            last_release = t;
            is_released = 1;
        }
    } else {
        is_released = 0;
    }
    if (is_released) {
        if (t - last_release < DEBOUNCE_TIME) {
          // do nothing
        } else if (t - last_release < DEBOUNCE_TIME + HIGH_POWER_TIME) {
            analogWrite(brake1, HIGH_POWER_Z);
            analogWrite(brake2, HIGH_POWER);
            analogWrite(brake3, HIGH_POWER);
            analogWrite(brake4, HIGH_POWER);
            analogWrite(brake5, HIGH_POWER);  
            digitalWrite(led, 255);

        } else {
            analogWrite(brake1, LOW_POWER_Z);
            analogWrite(brake2, LOW_POWER);
            analogWrite(brake3, LOW_POWER);
            analogWrite(brake4, LOW_POWER);
            analogWrite(brake5, LOW_POWER);     
            analogWrite(led, (millis() % 100) > 50 ? 40 : 0);

        }
    } else {
        analogWrite(brake1, 0);
        analogWrite(brake2, 0);
        analogWrite(brake3, 0);
        analogWrite(brake4, 0);
        analogWrite(brake5, 0);
        analogWrite(led, 1);

    }
}

extern "C" void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

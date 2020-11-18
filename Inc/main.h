/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MATRIX_K19_Pin GPIO_PIN_2
#define MATRIX_K19_GPIO_Port GPIOE
#define MATRIX_K20_Pin GPIO_PIN_3
#define MATRIX_K20_GPIO_Port GPIOE
#define MATRIX_K21_Pin GPIO_PIN_4
#define MATRIX_K21_GPIO_Port GPIOE
#define MATRIX_K22_Pin GPIO_PIN_5
#define MATRIX_K22_GPIO_Port GPIOE
#define MATRIX_K23_Pin GPIO_PIN_6
#define MATRIX_K23_GPIO_Port GPIOE
#define MATRIX_K14_Pin GPIO_PIN_13
#define MATRIX_K14_GPIO_Port GPIOC
#define MATRIX_K15_Pin GPIO_PIN_14
#define MATRIX_K15_GPIO_Port GPIOC
#define MATRIX_K16_Pin GPIO_PIN_15
#define MATRIX_K16_GPIO_Port GPIOC
#define MATRIX_K1_Pin GPIO_PIN_0
#define MATRIX_K1_GPIO_Port GPIOC
#define MATRIX_K2_Pin GPIO_PIN_1
#define MATRIX_K2_GPIO_Port GPIOC
#define MATRIX_K3_Pin GPIO_PIN_2
#define MATRIX_K3_GPIO_Port GPIOC
#define MATRIX_K4_Pin GPIO_PIN_3
#define MATRIX_K4_GPIO_Port GPIOC
#define ADC1_IN3_VBAT_Pin GPIO_PIN_3
#define ADC1_IN3_VBAT_GPIO_Port GPIOA
#define ADC1_IN4_CHRG_Pin GPIO_PIN_4
#define ADC1_IN4_CHRG_GPIO_Port GPIOA
#define MCU_CE_Pin GPIO_PIN_5
#define MCU_CE_GPIO_Port GPIOA
#define SPI_WP_Pin GPIO_PIN_7
#define SPI_WP_GPIO_Port GPIOA
#define MATRIX_K5_Pin GPIO_PIN_4
#define MATRIX_K5_GPIO_Port GPIOC
#define MATRIX_K6_Pin GPIO_PIN_5
#define MATRIX_K6_GPIO_Port GPIOC
#define MATRIX_A1_Pin GPIO_PIN_0
#define MATRIX_A1_GPIO_Port GPIOB
#define MATRIX_A2_Pin GPIO_PIN_1
#define MATRIX_A2_GPIO_Port GPIOB
#define MATRIX_A3_Pin GPIO_PIN_2
#define MATRIX_A3_GPIO_Port GPIOB
#define MATRIX_K24_Pin GPIO_PIN_7
#define MATRIX_K24_GPIO_Port GPIOE
#define PWM_SPK1_N_Pin GPIO_PIN_8
#define PWM_SPK1_N_GPIO_Port GPIOE
#define PWM_SPK1_P_Pin GPIO_PIN_9
#define PWM_SPK1_P_GPIO_Port GPIOE
#define PWM_SPK2_N_Pin GPIO_PIN_10
#define PWM_SPK2_N_GPIO_Port GPIOE
#define PWM_SPK2_P_Pin GPIO_PIN_11
#define PWM_SPK2_P_GPIO_Port GPIOE
#define SPI4_NSS_Pin GPIO_PIN_15
#define SPI4_NSS_GPIO_Port GPIOE
#define MATRIX_A11_Pin GPIO_PIN_10
#define MATRIX_A11_GPIO_Port GPIOB
#define MATRIX_A12_Pin GPIO_PIN_11
#define MATRIX_A12_GPIO_Port GPIOB
#define MATRIX_A13_Pin GPIO_PIN_12
#define MATRIX_A13_GPIO_Port GPIOB
#define MATRIX_A14_Pin GPIO_PIN_13
#define MATRIX_A14_GPIO_Port GPIOB
#define MATRIX_A15_Pin GPIO_PIN_14
#define MATRIX_A15_GPIO_Port GPIOB
#define MATRIX_A16_Pin GPIO_PIN_15
#define MATRIX_A16_GPIO_Port GPIOB
#define MATRIX_A25_Pin GPIO_PIN_8
#define MATRIX_A25_GPIO_Port GPIOD
#define MATRIX_A26_Pin GPIO_PIN_9
#define MATRIX_A26_GPIO_Port GPIOD
#define MCU_MATRIX_EN_Pin GPIO_PIN_10
#define MCU_MATRIX_EN_GPIO_Port GPIOD
#define MATRIX_K7_Pin GPIO_PIN_6
#define MATRIX_K7_GPIO_Port GPIOC
#define MATRIX_K8_Pin GPIO_PIN_7
#define MATRIX_K8_GPIO_Port GPIOC
#define MATRIX_K9_Pin GPIO_PIN_8
#define MATRIX_K9_GPIO_Port GPIOC
#define MATRIX_K10_Pin GPIO_PIN_9
#define MATRIX_K10_GPIO_Port GPIOC
#define MCU_STBY_Pin GPIO_PIN_8
#define MCU_STBY_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define MCU_CHRG_Pin GPIO_PIN_10
#define MCU_CHRG_GPIO_Port GPIOA
#define MATRIX_K11_Pin GPIO_PIN_10
#define MATRIX_K11_GPIO_Port GPIOC
#define MATRIX_K12_Pin GPIO_PIN_11
#define MATRIX_K12_GPIO_Port GPIOC
#define MATRIX_K13_Pin GPIO_PIN_12
#define MATRIX_K13_GPIO_Port GPIOC
#define MATRIX_A17_Pin GPIO_PIN_0
#define MATRIX_A17_GPIO_Port GPIOD
#define MATRIX_A18_Pin GPIO_PIN_1
#define MATRIX_A18_GPIO_Port GPIOD
#define MATRIX_A19_Pin GPIO_PIN_2
#define MATRIX_A19_GPIO_Port GPIOD
#define MATRIX_A20_Pin GPIO_PIN_3
#define MATRIX_A20_GPIO_Port GPIOD
#define MATRIX_A21_Pin GPIO_PIN_4
#define MATRIX_A21_GPIO_Port GPIOD
#define MATRIX_A22_Pin GPIO_PIN_5
#define MATRIX_A22_GPIO_Port GPIOD
#define MATRIX_A23_Pin GPIO_PIN_6
#define MATRIX_A23_GPIO_Port GPIOD
#define MATRIX_A24_Pin GPIO_PIN_7
#define MATRIX_A24_GPIO_Port GPIOD
#define MATRIX_A4_Pin GPIO_PIN_3
#define MATRIX_A4_GPIO_Port GPIOB
#define MATRIX_A5_Pin GPIO_PIN_4
#define MATRIX_A5_GPIO_Port GPIOB
#define MATRIX_A6_Pin GPIO_PIN_5
#define MATRIX_A6_GPIO_Port GPIOB
#define MATRIX_A7_Pin GPIO_PIN_6
#define MATRIX_A7_GPIO_Port GPIOB
#define MATRIX_A8_Pin GPIO_PIN_7
#define MATRIX_A8_GPIO_Port GPIOB
#define MATRIX_A9_Pin GPIO_PIN_8
#define MATRIX_A9_GPIO_Port GPIOB
#define MATRIX_A10_Pin GPIO_PIN_9
#define MATRIX_A10_GPIO_Port GPIOB
#define MATRIX_K17_Pin GPIO_PIN_0
#define MATRIX_K17_GPIO_Port GPIOE
#define MATRIX_K18_Pin GPIO_PIN_1
#define MATRIX_K18_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

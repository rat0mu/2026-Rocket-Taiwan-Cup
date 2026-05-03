/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PYRO1_Pin GPIO_PIN_2
#define PYRO1_GPIO_Port GPIOE
#define PYRO2_Pin GPIO_PIN_3
#define PYRO2_GPIO_Port GPIOE
#define PAYLOAD_3_Pin GPIO_PIN_0
#define PAYLOAD_3_GPIO_Port GPIOC
#define PAYLOAD_4_Pin GPIO_PIN_1
#define PAYLOAD_4_GPIO_Port GPIOC
#define GPS_RX_Pin GPIO_PIN_0
#define GPS_RX_GPIO_Port GPIOA
#define GPS_TX_Pin GPIO_PIN_1
#define GPS_TX_GPIO_Port GPIOA
#define BACKUP_RX_Pin GPIO_PIN_2
#define BACKUP_RX_GPIO_Port GPIOA
#define CS_IMU_GYRO_Pin GPIO_PIN_3
#define CS_IMU_GYRO_GPIO_Port GPIOA
#define CS_IMU_ACCEL_Pin GPIO_PIN_4
#define CS_IMU_ACCEL_GPIO_Port GPIOA
#define INT3_IMU_GYRO_Pin GPIO_PIN_5
#define INT3_IMU_GYRO_GPIO_Port GPIOC
#define PAYLOAD_1_Pin GPIO_PIN_0
#define PAYLOAD_1_GPIO_Port GPIOB
#define PAYLOAD_2_Pin GPIO_PIN_1
#define PAYLOAD_2_GPIO_Port GPIOB
#define GPS_PPS_Pin GPIO_PIN_9
#define GPS_PPS_GPIO_Port GPIOE
#define CS_MAG_Pin GPIO_PIN_13
#define CS_MAG_GPIO_Port GPIOE
#define CS_BARO_Pin GPIO_PIN_14
#define CS_BARO_GPIO_Port GPIOE
#define CS_ACCEL_Pin GPIO_PIN_15
#define CS_ACCEL_GPIO_Port GPIOE
#define INT_BARO_Pin GPIO_PIN_14
#define INT_BARO_GPIO_Port GPIOB
#define INT_MAG_Pin GPIO_PIN_15
#define INT_MAG_GPIO_Port GPIOB
#define DRDY_MAG_Pin GPIO_PIN_8
#define DRDY_MAG_GPIO_Port GPIOD
#define LORA_RX_Pin GPIO_PIN_9
#define LORA_RX_GPIO_Port GPIOA
#define LORA_TX_Pin GPIO_PIN_10
#define LORA_TX_GPIO_Port GPIOA
#define GPS_RESET_Pin GPIO_PIN_11
#define GPS_RESET_GPIO_Port GPIOA
#define INT_GPS_Pin GPIO_PIN_12
#define INT_GPS_GPIO_Port GPIOA
#define SDIO_DET_Pin GPIO_PIN_1
#define SDIO_DET_GPIO_Port GPIOD
#define BACKUP_TX_Pin GPIO_PIN_6
#define BACKUP_TX_GPIO_Port GPIOD
#define LORA_AUX_Pin GPIO_PIN_4
#define LORA_AUX_GPIO_Port GPIOB
#define LORA_M0_Pin GPIO_PIN_5
#define LORA_M0_GPIO_Port GPIOB
#define LORA_M1_Pin GPIO_PIN_6
#define LORA_M1_GPIO_Port GPIOB
#define LORA_RESET_Pin GPIO_PIN_7
#define LORA_RESET_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

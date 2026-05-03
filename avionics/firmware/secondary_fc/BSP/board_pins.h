#ifndef SECONDARY_FC_BSP_BOARD_PINS_H
#define SECONDARY_FC_BSP_BOARD_PINS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

void Error_Handler(void);

#define INT1_IMU_ACCEL_Pin GPIO_PIN_13
#define INT1_IMU_ACCEL_GPIO_Port GPIOC
#define INT3_IMU_GYRO_Pin GPIO_PIN_14
#define INT3_IMU_GYRO_GPIO_Port GPIOC
#define INT_GPS_Pin GPIO_PIN_0
#define INT_GPS_GPIO_Port GPIOA
#define INT_BARO_Pin GPIO_PIN_1
#define INT_BARO_GPIO_Port GPIOA
#define CS_IMU_GYRO_Pin GPIO_PIN_11
#define CS_IMU_GYRO_GPIO_Port GPIOA
#define CS_IMU_ACCEL_Pin GPIO_PIN_12
#define CS_IMU_ACCEL_GPIO_Port GPIOA
#define CS_BARO_Pin GPIO_PIN_15
#define CS_BARO_GPIO_Port GPIOA
#define GPS_PPS_Pin GPIO_PIN_3
#define GPS_PPS_GPIO_Port GPIOB
#define CS_SD_Pin GPIO_PIN_4
#define CS_SD_GPIO_Port GPIOB
#define LORA_AUX_Pin GPIO_PIN_5
#define LORA_AUX_GPIO_Port GPIOB
#define LORA_RESET_Pin GPIO_PIN_6
#define LORA_RESET_GPIO_Port GPIOB
#define GPS_RESET_Pin GPIO_PIN_7
#define GPS_RESET_GPIO_Port GPIOB
#define LORA_M0_Pin GPIO_PIN_8
#define LORA_M0_GPIO_Port GPIOB
#define LORA_M1_Pin GPIO_PIN_9
#define LORA_M1_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif

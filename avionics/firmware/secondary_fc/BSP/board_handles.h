#ifndef SECONDARY_FC_BSP_BOARD_HANDLES_H
#define SECONDARY_FC_BSP_BOARD_HANDLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef g_hspi1;
extern SPI_HandleTypeDef g_hspi2;
extern TIM_HandleTypeDef g_htim2;
extern UART_HandleTypeDef g_huart1;
extern UART_HandleTypeDef g_huart2;
extern UART_HandleTypeDef g_huart3;

#ifdef __cplusplus
}
#endif

#endif

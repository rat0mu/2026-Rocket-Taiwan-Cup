#ifndef SECONDARY_FC_BSP_BOARD_SUPPORT_H
#define SECONDARY_FC_BSP_BOARD_SUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board_pins.h"

typedef enum
{
    SECONDARY_BSP_STATUS_OK = 0,
    SECONDARY_BSP_STATUS_HAL_ERROR = 1
} secondary_bsp_status_t;

/**
 * @brief Purpose: Initialize the secondary flight computer board support package.
 * @param None.
 * @return Secondary BSP initialization status.
 * @failure_behavior Returns `SECONDARY_BSP_STATUS_HAL_ERROR` when any hardware
 * initialization step fails; callers must enter a fail-safe state.
 * @criticality DAL-A-like.
 */
secondary_bsp_status_t secondary_bsp_init(void);

#ifdef __cplusplus
}
#endif

#endif

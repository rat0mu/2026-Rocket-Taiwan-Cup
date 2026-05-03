#include "board_support.h"

#include "backup_state.h"
#include "igniter.h"
#include "sync_handler.h"

/**
 * @brief Purpose: Bootstrap the secondary flight computer runtime.
 * @param None.
 * @return Process exit code, which is never expected to return during normal operation.
 * @failure_behavior Enters `Error_Handler()` if board initialization fails, otherwise
 * remains in a safe idle loop until application scheduling is integrated.
 * @criticality DAL-A-like.
 */
int main(void)
{
    const secondary_bsp_status_t init_status = secondary_bsp_init();
    secondary_sync_handler_t sync_handler = {0};
    secondary_backup_state_t backup_state = {0};
    secondary_igniter_state_t igniter_state = {0};

    if (init_status != SECONDARY_BSP_STATUS_OK)
    {
        Error_Handler();
    }

    if (secondary_sync_handler_init(&sync_handler) != SECONDARY_SYNC_STATUS_OK)
    {
        Error_Handler();
    }

    if (secondary_backup_state_init(&backup_state) !=
        SECONDARY_BACKUP_STATE_STATUS_OK)
    {
        Error_Handler();
    }

    if (secondary_igniter_init(&igniter_state) != SECONDARY_IGNITER_STATUS_OK)
    {
        Error_Handler();
    }

    while (1)
    {
    }
}

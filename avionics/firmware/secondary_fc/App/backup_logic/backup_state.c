#include "backup_state.h"

/**
 * @brief Purpose: Initialize backup application state to safe monitoring mode.
 * @param state Output backup state structure.
 * @return Backup state initialization status.
 * @failure_behavior Returns `SECONDARY_BACKUP_STATE_STATUS_INVALID_ARGUMENT` for null input.
 * @criticality DAL-A-like.
 */
secondary_backup_state_status_t secondary_backup_state_init(
    secondary_backup_state_t *const state)
{
    secondary_backup_state_status_t status = SECONDARY_BACKUP_STATE_STATUS_INVALID_ARGUMENT;

    if (state != 0)
    {
        state->system_armed = false;
        state->system_healthy = false;
        state->flight_valid = false;
        state->backup_mode = SECONDARY_BACKUP_MODE_MONITOR_ONLY;
        state->takeover_timestamp_ms = 0U;
        status = SECONDARY_BACKUP_STATE_STATUS_OK;
    }

    return status;
}

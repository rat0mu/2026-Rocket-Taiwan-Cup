#include "igniter.h"

/**
 * @brief Purpose: Initialize backup igniter state to a fail-safe inhibited condition.
 * @param state Output igniter state.
 * @return Igniter initialization status.
 * @failure_behavior Returns `SECONDARY_IGNITER_STATUS_INVALID_ARGUMENT` for null input.
 * @criticality DAL-A-like.
 */
secondary_igniter_status_t secondary_igniter_init(
    secondary_igniter_state_t *const state)
{
    secondary_igniter_status_t status = SECONDARY_IGNITER_STATUS_INVALID_ARGUMENT;

    if (state != 0)
    {
        state->system_armed = false;
        state->deployment_inhibit_active = true;
        state->drogue_backup_fired = false;
        state->main_backup_fired = false;
        status = SECONDARY_IGNITER_STATUS_OK;
    }

    return status;
}

/**
 * @brief Purpose: Request one backup igniter firing event.
 * @param state In-out igniter state.
 * @param channel Backup igniter channel to fire.
 * @param pulse_duration_ms Requested pulse duration in milliseconds.
 * @return Igniter request status.
 * @failure_behavior Returns explicit inhibited or not-configured status and never
 * energizes an unverified output mapping.
 * @criticality DAL-A-like.
 */
secondary_igniter_status_t secondary_igniter_fire(
    secondary_igniter_state_t *const state,
    const secondary_igniter_channel_t channel,
    const uint32_t pulse_duration_ms)
{
    secondary_igniter_status_t status = SECONDARY_IGNITER_STATUS_INVALID_ARGUMENT;

    if ((state != 0) && (pulse_duration_ms > 0U))
    {
        if ((state->system_armed == false) || (state->deployment_inhibit_active == true))
        {
            status = SECONDARY_IGNITER_STATUS_INHIBITED;
        }
        else if ((channel == SECONDARY_IGNITER_CHANNEL_DROGUE_BACKUP) &&
                 (state->drogue_backup_fired == false))
        {
            status = SECONDARY_IGNITER_STATUS_NOT_CONFIGURED;
        }
        else if ((channel == SECONDARY_IGNITER_CHANNEL_MAIN_BACKUP) &&
                 (state->main_backup_fired == false))
        {
            status = SECONDARY_IGNITER_STATUS_NOT_CONFIGURED;
        }
        else
        {
            status = SECONDARY_IGNITER_STATUS_INHIBITED;
        }
    }

    return status;
}

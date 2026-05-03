#include "parachute.h"

/**
 * @brief Purpose: Dispatch a backup parachute deployment request to the igniter layer.
 * @param igniter_state In-out igniter state.
 * @param event Deployment event to request.
 * @param pulse_duration_ms Requested fixed pulse duration in milliseconds.
 * @return Deployment dispatch status.
 * @failure_behavior Propagates explicit igniter rejection without attempting unsafe retries.
 * @criticality DAL-A-like.
 */
secondary_parachute_status_t secondary_parachute_request_deploy(
    secondary_igniter_state_t *const igniter_state,
    const secondary_parachute_event_t event,
    const uint32_t pulse_duration_ms)
{
    secondary_parachute_status_t status = SECONDARY_PARACHUTE_STATUS_INVALID_ARGUMENT;
    secondary_igniter_status_t igniter_status = SECONDARY_IGNITER_STATUS_INVALID_ARGUMENT;

    if (igniter_state != 0)
    {
        if (event == SECONDARY_PARACHUTE_EVENT_DROGUE)
        {
            igniter_status = secondary_igniter_fire(
                igniter_state,
                SECONDARY_IGNITER_CHANNEL_DROGUE_BACKUP,
                pulse_duration_ms);
        }
        else if (event == SECONDARY_PARACHUTE_EVENT_MAIN)
        {
            igniter_status = secondary_igniter_fire(
                igniter_state,
                SECONDARY_IGNITER_CHANNEL_MAIN_BACKUP,
                pulse_duration_ms);
        }
        else
        {
        }

        if (igniter_status == SECONDARY_IGNITER_STATUS_OK)
        {
            status = SECONDARY_PARACHUTE_STATUS_OK;
        }
        else
        {
            status = SECONDARY_PARACHUTE_STATUS_IGNITER_REJECTED;
        }
    }

    return status;
}

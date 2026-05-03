#include "backup_link_manager.h"

/**
 * @brief Purpose: Initialize backup link manager state to a safe default.
 * @param state Output state structure.
 * @return Link manager initialization status.
 * @failure_behavior Returns `SECONDARY_LINK_MANAGER_STATUS_INVALID_ARGUMENT` for null input.
 * @criticality Mission-critical.
 */
secondary_link_manager_status_t secondary_link_manager_init(
    secondary_link_manager_state_t *const state)
{
    secondary_link_manager_status_t status = SECONDARY_LINK_MANAGER_STATUS_INVALID_ARGUMENT;

    if (state != 0)
    {
        state->heartbeat_received = false;
        state->last_heartbeat_timestamp_ms = 0U;
        state->last_heartbeat_counter = 0U;
        status = SECONDARY_LINK_MANAGER_STATUS_OK;
    }

    return status;
}

/**
 * @brief Purpose: Poll a heartbeat frame and update local heartbeat state.
 * @param state In-out link manager state.
 * @param now_timestamp_ms Current local time in milliseconds.
 * @param timeout_ms Bounded transport timeout in milliseconds.
 * @return Link manager polling status.
 * @failure_behavior Returns an explicit status code if the transport or frame validation fails.
 * @criticality Mission-critical.
 */
secondary_link_manager_status_t secondary_link_manager_poll_heartbeat(
    secondary_link_manager_state_t *const state,
    const uint32_t now_timestamp_ms,
    const uint32_t timeout_ms)
{
    secondary_link_manager_status_t status = SECONDARY_LINK_MANAGER_STATUS_INVALID_ARGUMENT;
    secondary_link_frame_t frame = {0};
    secondary_link_driver_status_t driver_status = SECONDARY_LINK_DRIVER_STATUS_INVALID_ARGUMENT;

    if ((state != 0) && (timeout_ms > 0U))
    {
        driver_status = secondary_link_driver_receive_frame(&frame, timeout_ms);

        if (driver_status == SECONDARY_LINK_DRIVER_STATUS_OK)
        {
            state->heartbeat_received = true;
            state->last_heartbeat_timestamp_ms = now_timestamp_ms;
            state->last_heartbeat_counter = frame.heartbeat_counter;
            status = SECONDARY_LINK_MANAGER_STATUS_OK;
        }
        else
        {
            status = SECONDARY_LINK_MANAGER_STATUS_DRIVER_ERROR;
        }
    }

    return status;
}

/**
 * @brief Purpose: Determine whether the primary heartbeat has become stale.
 * @param state Current link manager state.
 * @param now_timestamp_ms Current local time in milliseconds.
 * @param heartbeat_timeout_ms Allowed heartbeat age in milliseconds.
 * @return `true` when the heartbeat is absent or stale.
 * @failure_behavior Treats invalid inputs as stale to preserve fail-safe behavior.
 * @criticality DAL-A-like.
 */
bool secondary_link_manager_is_primary_stale(
    const secondary_link_manager_state_t *const state,
    const uint32_t now_timestamp_ms,
    const uint32_t heartbeat_timeout_ms)
{
    bool is_stale = true;

    if ((state != 0) && (heartbeat_timeout_ms > 0U) &&
        (state->heartbeat_received == true))
    {
        is_stale = ((now_timestamp_ms - state->last_heartbeat_timestamp_ms) >
                    heartbeat_timeout_ms);
    }

    return is_stale;
}

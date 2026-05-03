#include "sync_handler.h"

/**
 * @brief Purpose: Initialize the application-level synchronization handler.
 * @param handler Output synchronization handler state.
 * @return Synchronization handler initialization status.
 * @failure_behavior Returns an explicit status code for invalid arguments or link init failures.
 * @criticality Mission-critical.
 */
secondary_sync_status_t secondary_sync_handler_init(
    secondary_sync_handler_t *const handler)
{
    secondary_sync_status_t status = SECONDARY_SYNC_STATUS_INVALID_ARGUMENT;

    if (handler != 0)
    {
        if (secondary_link_manager_init(&handler->link_state) ==
            SECONDARY_LINK_MANAGER_STATUS_OK)
        {
            status = SECONDARY_SYNC_STATUS_OK;
        }
        else
        {
            status = SECONDARY_SYNC_STATUS_LINK_ERROR;
        }
    }

    return status;
}

/**
 * @brief Purpose: Poll inter-MCU synchronization traffic for the backup application layer.
 * @param handler In-out synchronization handler state.
 * @param now_timestamp_ms Current local time in milliseconds.
 * @param timeout_ms Bounded UART timeout in milliseconds.
 * @return Synchronization polling status.
 * @failure_behavior Returns an explicit link error status without hidden retries.
 * @criticality Mission-critical.
 */
secondary_sync_status_t secondary_sync_handler_poll(
    secondary_sync_handler_t *const handler,
    const uint32_t now_timestamp_ms,
    const uint32_t timeout_ms)
{
    secondary_sync_status_t status = SECONDARY_SYNC_STATUS_INVALID_ARGUMENT;

    if (handler != 0)
    {
        if (secondary_link_manager_poll_heartbeat(
                &handler->link_state, now_timestamp_ms, timeout_ms) ==
            SECONDARY_LINK_MANAGER_STATUS_OK)
        {
            status = SECONDARY_SYNC_STATUS_OK;
        }
        else
        {
            status = SECONDARY_SYNC_STATUS_LINK_ERROR;
        }
    }

    return status;
}

#ifndef SECONDARY_FC_APP_SYNC_HANDLER_H
#define SECONDARY_FC_APP_SYNC_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "backup_link_manager.h"

typedef struct
{
    secondary_link_manager_state_t link_state;
} secondary_sync_handler_t;

typedef enum
{
    SECONDARY_SYNC_STATUS_OK = 0,
    SECONDARY_SYNC_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_SYNC_STATUS_LINK_ERROR = 2
} secondary_sync_status_t;

/**
 * @brief Purpose: Initialize the application-level synchronization handler.
 * @param handler Output synchronization handler state.
 * @return Synchronization handler initialization status.
 * @failure_behavior Returns an explicit status code for invalid arguments.
 * @criticality Mission-critical.
 */
secondary_sync_status_t secondary_sync_handler_init(
    secondary_sync_handler_t *handler);

/**
 * @brief Purpose: Poll inter-MCU synchronization traffic.
 * @param handler In-out synchronization handler state.
 * @param now_timestamp_ms Current local time in milliseconds.
 * @param timeout_ms Bounded UART timeout in milliseconds.
 * @return Synchronization polling status.
 * @failure_behavior Returns an explicit status code if the link layer reports an error.
 * @criticality Mission-critical.
 */
secondary_sync_status_t secondary_sync_handler_poll(
    secondary_sync_handler_t *handler,
    uint32_t now_timestamp_ms,
    uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif

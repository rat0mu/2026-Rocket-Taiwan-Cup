#ifndef SECONDARY_FC_MODULE_COMM_LINK_LINK_MANAGER_BACKUP_LINK_MANAGER_H
#define SECONDARY_FC_MODULE_COMM_LINK_LINK_MANAGER_BACKUP_LINK_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "backup_link_driver.h"

typedef struct
{
    bool heartbeat_received;
    uint32_t last_heartbeat_timestamp_ms;
    uint16_t last_heartbeat_counter;
} secondary_link_manager_state_t;

typedef enum
{
    SECONDARY_LINK_MANAGER_STATUS_OK = 0,
    SECONDARY_LINK_MANAGER_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_LINK_MANAGER_STATUS_DRIVER_ERROR = 2
} secondary_link_manager_status_t;

/**
 * @brief Purpose: Initialize the backup link manager state.
 * @param state Output state structure.
 * @return Link manager initialization status.
 * @failure_behavior Returns `SECONDARY_LINK_MANAGER_STATUS_INVALID_ARGUMENT` when the
 * state pointer is null.
 * @criticality Mission-critical.
 */
secondary_link_manager_status_t secondary_link_manager_init(
    secondary_link_manager_state_t *state);

/**
 * @brief Purpose: Poll one heartbeat frame from the primary flight computer.
 * @param state In-out state structure.
 * @param now_timestamp_ms Current local time in milliseconds.
 * @param timeout_ms Bounded UART timeout in milliseconds.
 * @return Link manager polling status.
 * @failure_behavior Returns an explicit status code if transport or frame validation fails.
 * @criticality Mission-critical.
 */
secondary_link_manager_status_t secondary_link_manager_poll_heartbeat(
    secondary_link_manager_state_t *state,
    uint32_t now_timestamp_ms,
    uint32_t timeout_ms);

/**
 * @brief Purpose: Determine whether the primary heartbeat is stale.
 * @param state Current link manager state.
 * @param now_timestamp_ms Current local time in milliseconds.
 * @param heartbeat_timeout_ms Allowed heartbeat age in milliseconds.
 * @return `true` when the heartbeat is stale or absent.
 * @failure_behavior Treats invalid state or invalid timeout values as stale.
 * @criticality DAL-A-like.
 */
bool secondary_link_manager_is_primary_stale(
    const secondary_link_manager_state_t *state,
    uint32_t now_timestamp_ms,
    uint32_t heartbeat_timeout_ms);

#ifdef __cplusplus
}
#endif

#endif

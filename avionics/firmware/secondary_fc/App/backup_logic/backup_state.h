#ifndef SECONDARY_FC_APP_BACKUP_LOGIC_BACKUP_STATE_H
#define SECONDARY_FC_APP_BACKUP_LOGIC_BACKUP_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    SECONDARY_BACKUP_MODE_MONITOR_ONLY = 0,
    SECONDARY_BACKUP_MODE_TAKEOVER_ACTIVE = 1
} secondary_backup_mode_t;

typedef struct
{
    bool system_armed;
    bool system_healthy;
    bool flight_valid;
    secondary_backup_mode_t backup_mode;
    uint32_t takeover_timestamp_ms;
} secondary_backup_state_t;

typedef enum
{
    SECONDARY_BACKUP_STATE_STATUS_OK = 0,
    SECONDARY_BACKUP_STATE_STATUS_INVALID_ARGUMENT = 1
} secondary_backup_state_status_t;

/**
 * @brief Purpose: Initialize backup application state to safe monitoring mode.
 * @param state Output backup state structure.
 * @return Backup state initialization status.
 * @failure_behavior Returns `SECONDARY_BACKUP_STATE_STATUS_INVALID_ARGUMENT` for null input.
 * @criticality DAL-A-like.
 */
secondary_backup_state_status_t secondary_backup_state_init(
    secondary_backup_state_t *state);

#ifdef __cplusplus
}
#endif

#endif

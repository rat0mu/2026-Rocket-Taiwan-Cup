#ifndef SECONDARY_FC_ACTUATOR_PARACHUTE_IGNITER_H
#define SECONDARY_FC_ACTUATOR_PARACHUTE_IGNITER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    SECONDARY_IGNITER_CHANNEL_DROGUE_BACKUP = 0,
    SECONDARY_IGNITER_CHANNEL_MAIN_BACKUP = 1
} secondary_igniter_channel_t;

typedef enum
{
    SECONDARY_IGNITER_STATUS_OK = 0,
    SECONDARY_IGNITER_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_IGNITER_STATUS_INHIBITED = 2,
    SECONDARY_IGNITER_STATUS_NOT_CONFIGURED = 3
} secondary_igniter_status_t;

typedef struct
{
    bool system_armed;
    bool deployment_inhibit_active;
    bool drogue_backup_fired;
    bool main_backup_fired;
} secondary_igniter_state_t;

/**
 * @brief Purpose: Initialize igniter state to a fail-safe non-fired condition.
 * @param state Output igniter state.
 * @return Igniter initialization status.
 * @failure_behavior Returns `SECONDARY_IGNITER_STATUS_INVALID_ARGUMENT` for null input.
 * @criticality DAL-A-like.
 */
secondary_igniter_status_t secondary_igniter_init(
    secondary_igniter_state_t *state);

/**
 * @brief Purpose: Request one bounded igniter firing event on the backup computer.
 * @param state In-out igniter state.
 * @param channel Backup igniter channel to fire.
 * @param pulse_duration_ms Requested pulse duration in milliseconds.
 * @return Igniter request status.
 * @failure_behavior Returns `SECONDARY_IGNITER_STATUS_NOT_CONFIGURED` while no verified
 * pyro output mapping exists for the secondary board.
 * @criticality DAL-A-like.
 */
secondary_igniter_status_t secondary_igniter_fire(
    secondary_igniter_state_t *state,
    secondary_igniter_channel_t channel,
    uint32_t pulse_duration_ms);

#ifdef __cplusplus
}
#endif

#endif

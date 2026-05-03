#ifndef SECONDARY_FC_APP_BACKUP_LOGIC_FAILOVER_H
#define SECONDARY_FC_APP_BACKUP_LOGIC_FAILOVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "backup_state.h"

typedef struct
{
    bool relevant_channel_already_fired;
    bool primary_completed_relevant_deploy;
    bool local_deploy_conditions_met;
    bool heartbeat_stale;
} secondary_failover_inputs_t;

typedef enum
{
    SECONDARY_FAILOVER_STATUS_OK = 0,
    SECONDARY_FAILOVER_STATUS_INVALID_ARGUMENT = 1
} secondary_failover_status_t;

/**
 * @brief Purpose: Evaluate whether backup takeover conditions are satisfied.
 * @param state In-out backup application state.
 * @param inputs Evaluated takeover preconditions.
 * @param now_timestamp_ms Current local time in milliseconds.
 * @param out_takeover_allowed Output boolean receiving the decision.
 * @return Failover evaluation status.
 * @failure_behavior Returns an explicit status code for invalid inputs and defaults
 * `out_takeover_allowed` to `false`.
 * @criticality DAL-A-like.
 */
secondary_failover_status_t secondary_failover_evaluate_takeover(
    secondary_backup_state_t *state,
    const secondary_failover_inputs_t *inputs,
    uint32_t now_timestamp_ms,
    bool *out_takeover_allowed);

#ifdef __cplusplus
}
#endif

#endif

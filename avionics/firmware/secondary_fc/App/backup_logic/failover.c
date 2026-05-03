#include "failover.h"

/**
 * @brief Purpose: Evaluate whether backup takeover conditions are satisfied.
 * @param state In-out backup application state.
 * @param inputs Evaluated takeover preconditions.
 * @param now_timestamp_ms Current local time in milliseconds.
 * @param out_takeover_allowed Output boolean receiving the decision.
 * @return Failover evaluation status.
 * @failure_behavior Returns an explicit status code for invalid inputs and defaults
 * the decision to `false`.
 * @criticality DAL-A-like.
 */
secondary_failover_status_t secondary_failover_evaluate_takeover(
    secondary_backup_state_t *const state,
    const secondary_failover_inputs_t *const inputs,
    const uint32_t now_timestamp_ms,
    bool *const out_takeover_allowed)
{
    secondary_failover_status_t status = SECONDARY_FAILOVER_STATUS_INVALID_ARGUMENT;

    if (out_takeover_allowed != 0)
    {
        *out_takeover_allowed = false;
    }

    if ((state != 0) && (inputs != 0) && (out_takeover_allowed != 0))
    {
        *out_takeover_allowed =
            (inputs->heartbeat_stale == true) &&
            (state->system_healthy == true) &&
            (state->system_armed == true) &&
            (state->flight_valid == true) &&
            (inputs->relevant_channel_already_fired == false) &&
            (inputs->primary_completed_relevant_deploy == false) &&
            (inputs->local_deploy_conditions_met == true);

        if (*out_takeover_allowed == true)
        {
            state->backup_mode = SECONDARY_BACKUP_MODE_TAKEOVER_ACTIVE;
            state->takeover_timestamp_ms = now_timestamp_ms;
        }

        status = SECONDARY_FAILOVER_STATUS_OK;
    }

    return status;
}

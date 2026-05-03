#ifndef SECONDARY_FC_ACTUATOR_PARACHUTE_PARACHUTE_H
#define SECONDARY_FC_ACTUATOR_PARACHUTE_PARACHUTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "igniter.h"

typedef enum
{
    SECONDARY_PARACHUTE_EVENT_DROGUE = 0,
    SECONDARY_PARACHUTE_EVENT_MAIN = 1
} secondary_parachute_event_t;

typedef enum
{
    SECONDARY_PARACHUTE_STATUS_OK = 0,
    SECONDARY_PARACHUTE_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_PARACHUTE_STATUS_IGNITER_REJECTED = 2
} secondary_parachute_status_t;

/**
 * @brief Purpose: Dispatch a backup parachute deployment request to the igniter layer.
 * @param igniter_state In-out igniter state.
 * @param event Deployment event to request.
 * @param pulse_duration_ms Requested fixed pulse duration.
 * @return Deployment dispatch status.
 * @failure_behavior Propagates igniter rejection as an explicit deployment error.
 * @criticality DAL-A-like.
 */
secondary_parachute_status_t secondary_parachute_request_deploy(
    secondary_igniter_state_t *igniter_state,
    secondary_parachute_event_t event,
    uint32_t pulse_duration_ms);

#ifdef __cplusplus
}
#endif

#endif

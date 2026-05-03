#include "neo_m9n.h"

/**
 * @brief Purpose: Initialize the backup GPS driver boundary.
 * @param None.
 * @return Driver initialization status.
 * @failure_behavior Returns `SECONDARY_NEO_M9N_STATUS_NOT_IMPLEMENTED` until parser
 * and receiver configuration are integrated.
 * @criticality Mission-critical.
 */
secondary_neo_m9n_status_t secondary_neo_m9n_init(void)
{
    return SECONDARY_NEO_M9N_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief Purpose: Produce a placeholder GPS fix in engineering units.
 * @param out_fix Output structure receiving the fix.
 * @return GPS read status.
 * @failure_behavior Marks the fix invalid and returns an explicit status if
 * sensor access is not yet integrated.
 * @criticality Mission-critical.
 */
secondary_neo_m9n_status_t secondary_neo_m9n_read_fix(
    secondary_neo_m9n_fix_t *const out_fix)
{
    secondary_neo_m9n_status_t status = SECONDARY_NEO_M9N_STATUS_INVALID_ARGUMENT;

    if (out_fix != 0)
    {
        out_fix->fix_valid = false;
        out_fix->latitude_deg = 0.0;
        out_fix->longitude_deg = 0.0;
        out_fix->altitude_m = 0.0F;
        status = SECONDARY_NEO_M9N_STATUS_NOT_IMPLEMENTED;
    }

    return status;
}

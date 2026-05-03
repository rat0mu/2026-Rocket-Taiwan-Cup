#ifndef SECONDARY_FC_DRIVER_NEO_M9N_NEO_M9N_H
#define SECONDARY_FC_DRIVER_NEO_M9N_NEO_M9N_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    SECONDARY_NEO_M9N_STATUS_OK = 0,
    SECONDARY_NEO_M9N_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_NEO_M9N_STATUS_NOT_IMPLEMENTED = 2
} secondary_neo_m9n_status_t;

typedef struct
{
    bool fix_valid;
    double latitude_deg;
    double longitude_deg;
    float altitude_m;
} secondary_neo_m9n_fix_t;

/**
 * @brief Purpose: Initialize the backup GPS driver boundary.
 * @param None.
 * @return Driver initialization status.
 * @failure_behavior Returns an explicit status code without attempting dynamic allocation.
 * @criticality Mission-critical.
 */
secondary_neo_m9n_status_t secondary_neo_m9n_init(void);

/**
 * @brief Purpose: Read a GPS fix placeholder from the NEO-M9N boundary.
 * @param out_fix Output structure receiving the fix.
 * @return GPS read status.
 * @failure_behavior Returns `SECONDARY_NEO_M9N_STATUS_NOT_IMPLEMENTED` until parser
 * integration is complete; output is marked invalid.
 * @criticality Mission-critical.
 */
secondary_neo_m9n_status_t secondary_neo_m9n_read_fix(
    secondary_neo_m9n_fix_t *out_fix);

#ifdef __cplusplus
}
#endif

#endif

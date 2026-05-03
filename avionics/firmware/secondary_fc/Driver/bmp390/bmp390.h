#ifndef SECONDARY_FC_DRIVER_BMP390_BMP390_H
#define SECONDARY_FC_DRIVER_BMP390_BMP390_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    SECONDARY_BMP390_STATUS_OK = 0,
    SECONDARY_BMP390_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_BMP390_STATUS_NOT_IMPLEMENTED = 2
} secondary_bmp390_status_t;

typedef struct
{
    bool sample_valid;
    float pressure_pa;
    float temperature_degC;
} secondary_bmp390_sample_t;

/**
 * @brief Purpose: Initialize the backup BMP390 driver boundary.
 * @param None.
 * @return Driver initialization status.
 * @failure_behavior Returns an explicit status code without attempting dynamic allocation.
 * @criticality Mission-critical.
 */
secondary_bmp390_status_t secondary_bmp390_init(void);

/**
 * @brief Purpose: Read a filtered BMP390 engineering sample placeholder.
 * @param out_sample Output structure receiving the sample.
 * @return BMP390 read status.
 * @failure_behavior Returns `SECONDARY_BMP390_STATUS_NOT_IMPLEMENTED` until sensor
 * register access is integrated; output is marked invalid.
 * @criticality Mission-critical.
 */
secondary_bmp390_status_t secondary_bmp390_read_sample(
    secondary_bmp390_sample_t *out_sample);

#ifdef __cplusplus
}
#endif

#endif

#include "bmp390.h"

/**
 * @brief Purpose: Initialize the backup BMP390 driver boundary.
 * @param None.
 * @return Driver initialization status.
 * @failure_behavior Returns `SECONDARY_BMP390_STATUS_NOT_IMPLEMENTED` until register
 * access and calibration handling are integrated.
 * @criticality Mission-critical.
 */
secondary_bmp390_status_t secondary_bmp390_init(void)
{
    return SECONDARY_BMP390_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief Purpose: Produce a placeholder BMP390 sample in engineering units.
 * @param out_sample Output structure receiving the sample.
 * @return BMP390 read status.
 * @failure_behavior Marks the sample invalid and returns an explicit status if
 * sensor access is not yet integrated.
 * @criticality Mission-critical.
 */
secondary_bmp390_status_t secondary_bmp390_read_sample(
    secondary_bmp390_sample_t *const out_sample)
{
    secondary_bmp390_status_t status = SECONDARY_BMP390_STATUS_INVALID_ARGUMENT;

    if (out_sample != 0)
    {
        out_sample->sample_valid = false;
        out_sample->pressure_pa = 0.0F;
        out_sample->temperature_degC = 0.0F;
        status = SECONDARY_BMP390_STATUS_NOT_IMPLEMENTED;
    }

    return status;
}

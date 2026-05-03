#include "lora.h"

/**
 * @brief Purpose: Initialize the backup LoRa driver boundary.
 * @param None.
 * @return Driver initialization status.
 * @failure_behavior Returns `SECONDARY_LORA_STATUS_NOT_IMPLEMENTED` until radio
 * configuration and framing are integrated.
 * @criticality Non-critical.
 */
secondary_lora_status_t secondary_lora_init(void)
{
    return SECONDARY_LORA_STATUS_NOT_IMPLEMENTED;
}

/**
 * @brief Purpose: Request one bounded LoRa transmission.
 * @param payload Pointer to the bytes to send.
 * @param length Number of bytes to send.
 * @return Transmission status.
 * @failure_behavior Returns an explicit status code and does not retry indefinitely.
 * @criticality Non-critical.
 */
secondary_lora_status_t secondary_lora_send(
    const uint8_t *const payload,
    const uint16_t length)
{
    secondary_lora_status_t status = SECONDARY_LORA_STATUS_INVALID_ARGUMENT;

    if ((payload != 0) && (length > 0U))
    {
        status = SECONDARY_LORA_STATUS_NOT_IMPLEMENTED;
    }

    return status;
}

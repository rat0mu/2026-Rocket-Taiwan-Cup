#ifndef SECONDARY_FC_DRIVER_LORA_LORA_H
#define SECONDARY_FC_DRIVER_LORA_LORA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
    SECONDARY_LORA_STATUS_OK = 0,
    SECONDARY_LORA_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_LORA_STATUS_NOT_IMPLEMENTED = 2
} secondary_lora_status_t;

/**
 * @brief Purpose: Initialize the backup LoRa driver boundary.
 * @param None.
 * @return Driver initialization status.
 * @failure_behavior Returns an explicit status code without attempting dynamic allocation.
 * @criticality Non-critical.
 */
secondary_lora_status_t secondary_lora_init(void);

/**
 * @brief Purpose: Transmit a bounded telemetry payload placeholder.
 * @param payload Pointer to the bytes to send.
 * @param length Number of bytes to send.
 * @return Transmission status.
 * @failure_behavior Returns `SECONDARY_LORA_STATUS_NOT_IMPLEMENTED` until radio framing
 * is integrated; does not retry indefinitely.
 * @criticality Non-critical.
 */
secondary_lora_status_t secondary_lora_send(
    const uint8_t *payload,
    uint16_t length);

#ifdef __cplusplus
}
#endif

#endif

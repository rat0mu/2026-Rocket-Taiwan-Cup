#ifndef SECONDARY_FC_MODULE_COMM_LINK_PROTOCOL_BACKUP_LINK_PROTOCOL_H
#define SECONDARY_FC_MODULE_COMM_LINK_PROTOCOL_BACKUP_LINK_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

enum
{
    SECONDARY_LINK_FRAME_HEADER = 0xA55AU,
    SECONDARY_LINK_MAX_PAYLOAD_BYTES = 32U
};

typedef enum
{
    SECONDARY_LINK_MESSAGE_HEARTBEAT = 1,
    SECONDARY_LINK_MESSAGE_STATE = 2,
    SECONDARY_LINK_MESSAGE_EVENT = 3
} secondary_link_message_type_t;

typedef struct
{
    uint16_t frame_header;
    uint8_t message_type;
    uint8_t payload_length;
    uint32_t timestamp_ms;
    uint16_t heartbeat_counter;
    uint8_t system_state;
    float altitude_m;
    float vertical_velocity_m_s;
    uint8_t gps_fix_valid;
    uint16_t pyro_status_flags;
    uint16_t health_flags;
    uint16_t crc16;
} secondary_link_frame_t;

/**
 * @brief Purpose: Compute a bounded CRC-16 over a fixed buffer.
 * @param data Pointer to the buffer.
 * @param length Number of bytes covered by the CRC.
 * @return CRC-16 value, or zero for invalid arguments.
 * @failure_behavior Returns zero when the input buffer is invalid.
 * @criticality Mission-critical.
 */
uint16_t secondary_link_protocol_crc16(
    const uint8_t *data,
    uint16_t length);

/**
 * @brief Purpose: Validate the static fields of a backup link frame.
 * @param frame Pointer to the frame being checked.
 * @return `true` if the frame is structurally valid, else `false`.
 * @failure_behavior Rejects invalid headers, oversized payload declarations, and null pointers.
 * @criticality Mission-critical.
 */
bool secondary_link_protocol_frame_is_structurally_valid(
    const secondary_link_frame_t *frame);

#ifdef __cplusplus
}
#endif

#endif

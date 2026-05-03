#include "backup_link_protocol.h"

/**
 * @brief Purpose: Compute CRC-16 for a fixed link payload.
 * @param data Pointer to the input bytes.
 * @param length Number of bytes covered by the CRC.
 * @return CRC-16 value, or zero for invalid arguments.
 * @failure_behavior Returns zero if the buffer is invalid and performs only bounded loops.
 * @criticality Mission-critical.
 */
uint16_t secondary_link_protocol_crc16(
    const uint8_t *const data,
    const uint16_t length)
{
    uint16_t crc = 0xFFFFU;
    uint16_t index = 0U;
    uint8_t bit_index = 0U;

    if ((data != 0) && (length > 0U))
    {
        for (index = 0U; index < length; ++index)
        {
            crc = (uint16_t)(crc ^ data[index]);

            for (bit_index = 0U; bit_index < 8U; ++bit_index)
            {
                if ((crc & 1U) != 0U)
                {
                    crc = (uint16_t)((crc >> 1U) ^ 0xA001U);
                }
                else
                {
                    crc = (uint16_t)(crc >> 1U);
                }
            }
        }
    }
    else
    {
        crc = 0U;
    }

    return crc;
}

/**
 * @brief Purpose: Check static structural validity for a backup link frame.
 * @param frame Pointer to the frame being validated.
 * @return `true` when the frame fields are structurally valid.
 * @failure_behavior Rejects null frames, bad headers, and oversized payload declarations.
 * @criticality Mission-critical.
 */
bool secondary_link_protocol_frame_is_structurally_valid(
    const secondary_link_frame_t *const frame)
{
    bool is_valid = false;

    if (frame != 0)
    {
        is_valid = (frame->frame_header == SECONDARY_LINK_FRAME_HEADER) &&
                   (frame->payload_length <= SECONDARY_LINK_MAX_PAYLOAD_BYTES);
    }

    return is_valid;
}

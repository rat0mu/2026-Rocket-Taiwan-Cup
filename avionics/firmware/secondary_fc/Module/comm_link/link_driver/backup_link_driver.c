#include "backup_link_driver.h"

#include <stddef.h>

#include "uart_bus.h"

/**
 * @brief Purpose: Convert a UART transport result into a link driver status.
 * @param uart_status UART layer status.
 * @return Converted link driver status.
 * @failure_behavior Maps transport failures to explicit link driver errors.
 * @criticality Mission-critical.
 */
static secondary_link_driver_status_t secondary_link_driver_status_from_uart(
    const secondary_uart_status_t uart_status)
{
    secondary_link_driver_status_t status = SECONDARY_LINK_DRIVER_STATUS_IO_ERROR;

    if (uart_status == SECONDARY_UART_STATUS_OK)
    {
        status = SECONDARY_LINK_DRIVER_STATUS_OK;
    }
    else if (uart_status == SECONDARY_UART_STATUS_INVALID_ARGUMENT)
    {
        status = SECONDARY_LINK_DRIVER_STATUS_INVALID_ARGUMENT;
    }

    return status;
}

/**
 * @brief Purpose: Initialize the backup link driver boundary.
 * @param None.
 * @return Link driver initialization status.
 * @failure_behavior Returns an explicit status code without hidden side effects.
 * @criticality Mission-critical.
 */
secondary_link_driver_status_t secondary_link_driver_init(void)
{
    return SECONDARY_LINK_DRIVER_STATUS_OK;
}

/**
 * @brief Purpose: Serialize and transmit one backup link frame.
 * @param frame Pointer to the frame to transmit.
 * @param timeout_ms Bounded UART timeout in milliseconds.
 * @return Link driver transmission status.
 * @failure_behavior Rejects invalid frames and returns an explicit transport error on failure.
 * @criticality Mission-critical.
 */
secondary_link_driver_status_t secondary_link_driver_send_frame(
    const secondary_link_frame_t *const frame,
    const uint32_t timeout_ms)
{
    secondary_link_driver_status_t status = SECONDARY_LINK_DRIVER_STATUS_INVALID_ARGUMENT;
    secondary_link_frame_t local_frame = {0};
    size_t crc_offset = 0U;

    if ((frame != 0) && (timeout_ms > 0U) &&
        secondary_link_protocol_frame_is_structurally_valid(frame))
    {
        local_frame = *frame;
        crc_offset = offsetof(secondary_link_frame_t, crc16);
        local_frame.crc16 = secondary_link_protocol_crc16(
            (const uint8_t *)&local_frame, (uint16_t)crc_offset);

        status = secondary_link_driver_status_from_uart(secondary_uart_bus_transmit(
            SECONDARY_UART_PORT_BACKUP_LINK,
            (const uint8_t *)&local_frame,
            (uint16_t)sizeof(local_frame),
            timeout_ms));
    }

    return status;
}

/**
 * @brief Purpose: Receive and validate one backup link frame.
 * @param out_frame Output frame buffer.
 * @param timeout_ms Bounded UART timeout in milliseconds.
 * @return Link driver reception status.
 * @failure_behavior Returns an explicit frame or transport error without hidden retries.
 * @criticality Mission-critical.
 */
secondary_link_driver_status_t secondary_link_driver_receive_frame(
    secondary_link_frame_t *const out_frame,
    const uint32_t timeout_ms)
{
    secondary_link_driver_status_t status = SECONDARY_LINK_DRIVER_STATUS_INVALID_ARGUMENT;
    uint16_t expected_crc = 0U;
    const size_t crc_offset = offsetof(secondary_link_frame_t, crc16);

    if ((out_frame != 0) && (timeout_ms > 0U))
    {
        status = secondary_link_driver_status_from_uart(secondary_uart_bus_receive(
            SECONDARY_UART_PORT_BACKUP_LINK,
            (uint8_t *)out_frame,
            (uint16_t)sizeof(*out_frame),
            timeout_ms));
    }

    if (status == SECONDARY_LINK_DRIVER_STATUS_OK)
    {
        expected_crc = secondary_link_protocol_crc16(
            (const uint8_t *)out_frame, (uint16_t)crc_offset);

        if ((!secondary_link_protocol_frame_is_structurally_valid(out_frame)) ||
            (expected_crc != out_frame->crc16))
        {
            status = SECONDARY_LINK_DRIVER_STATUS_FRAME_ERROR;
        }
    }

    return status;
}

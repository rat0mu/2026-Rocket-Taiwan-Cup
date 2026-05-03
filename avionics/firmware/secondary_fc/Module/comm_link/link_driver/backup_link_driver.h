#ifndef SECONDARY_FC_MODULE_COMM_LINK_LINK_DRIVER_BACKUP_LINK_DRIVER_H
#define SECONDARY_FC_MODULE_COMM_LINK_LINK_DRIVER_BACKUP_LINK_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "backup_link_protocol.h"

typedef enum
{
    SECONDARY_LINK_DRIVER_STATUS_OK = 0,
    SECONDARY_LINK_DRIVER_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_LINK_DRIVER_STATUS_IO_ERROR = 2,
    SECONDARY_LINK_DRIVER_STATUS_FRAME_ERROR = 3
} secondary_link_driver_status_t;

/**
 * @brief Purpose: Initialize the backup link driver boundary.
 * @param None.
 * @return Link driver initialization status.
 * @failure_behavior Returns an explicit status code without hidden retries.
 * @criticality Mission-critical.
 */
secondary_link_driver_status_t secondary_link_driver_init(void);

/**
 * @brief Purpose: Send one fixed-size backup link frame over the inter-MCU UART.
 * @param frame Pointer to the frame to transmit.
 * @param timeout_ms Bounded transport timeout in milliseconds.
 * @return Link driver transmission status.
 * @failure_behavior Returns a status code if the frame is invalid or the transport fails.
 * @criticality Mission-critical.
 */
secondary_link_driver_status_t secondary_link_driver_send_frame(
    const secondary_link_frame_t *frame,
    uint32_t timeout_ms);

/**
 * @brief Purpose: Receive one fixed-size backup link frame over the inter-MCU UART.
 * @param out_frame Output frame buffer.
 * @param timeout_ms Bounded transport timeout in milliseconds.
 * @return Link driver reception status.
 * @failure_behavior Returns a status code if the transport fails or the frame is invalid.
 * @criticality Mission-critical.
 */
secondary_link_driver_status_t secondary_link_driver_receive_frame(
    secondary_link_frame_t *out_frame,
    uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif

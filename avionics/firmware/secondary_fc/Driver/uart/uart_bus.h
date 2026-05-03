#ifndef SECONDARY_FC_DRIVER_UART_UART_BUS_H
#define SECONDARY_FC_DRIVER_UART_UART_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board_handles.h"

typedef enum
{
    SECONDARY_UART_PORT_GPS = 0,
    SECONDARY_UART_PORT_BACKUP_LINK = 1,
    SECONDARY_UART_PORT_LORA = 2,
    SECONDARY_UART_PORT_COUNT
} secondary_uart_port_id_t;

typedef enum
{
    SECONDARY_UART_STATUS_OK = 0,
    SECONDARY_UART_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_UART_STATUS_HAL_ERROR = 2
} secondary_uart_status_t;

/**
 * @brief Purpose: Resolve the UART handle associated with a logical secondary UART port.
 * @param port_id Logical UART port identifier.
 * @param out_handle Output pointer that receives the HAL UART handle.
 * @return UART handle resolution status.
 * @failure_behavior Returns `SECONDARY_UART_STATUS_INVALID_ARGUMENT` for invalid inputs.
 * @criticality Mission-critical.
 */
secondary_uart_status_t secondary_uart_bus_get_handle(
    secondary_uart_port_id_t port_id,
    UART_HandleTypeDef **out_handle);

/**
 * @brief Purpose: Transmit a bounded block of data over a UART port.
 * @param port_id Logical UART port identifier.
 * @param tx_data Pointer to the bytes to transmit.
 * @param length Number of bytes to transmit.
 * @param timeout_ms Bounded HAL timeout in milliseconds.
 * @return UART transmission status.
 * @failure_behavior Returns an explicit status code and does not retry indefinitely.
 * @criticality Mission-critical.
 */
secondary_uart_status_t secondary_uart_bus_transmit(
    secondary_uart_port_id_t port_id,
    const uint8_t *tx_data,
    uint16_t length,
    uint32_t timeout_ms);

/**
 * @brief Purpose: Receive a bounded block of data from a UART port.
 * @param port_id Logical UART port identifier.
 * @param rx_data Pointer to the receive buffer.
 * @param length Number of bytes to receive.
 * @param timeout_ms Bounded HAL timeout in milliseconds.
 * @return UART reception status.
 * @failure_behavior Returns an explicit status code and does not retry indefinitely.
 * @criticality Mission-critical.
 */
secondary_uart_status_t secondary_uart_bus_receive(
    secondary_uart_port_id_t port_id,
    uint8_t *rx_data,
    uint16_t length,
    uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif

#include "uart_bus.h"

/**
 * @brief Purpose: Convert a HAL UART result into a driver status code.
 * @param hal_status HAL return status.
 * @return Converted UART driver status.
 * @failure_behavior Maps all non-`HAL_OK` results to `SECONDARY_UART_STATUS_HAL_ERROR`.
 * @criticality Mission-critical.
 */
static secondary_uart_status_t secondary_uart_status_from_hal(
    const HAL_StatusTypeDef hal_status)
{
    secondary_uart_status_t status = SECONDARY_UART_STATUS_HAL_ERROR;

    if (hal_status == HAL_OK)
    {
        status = SECONDARY_UART_STATUS_OK;
    }

    return status;
}

/**
 * @brief Purpose: Resolve the HAL UART handle for a logical secondary port ID.
 * @param port_id Logical UART port identifier.
 * @param out_handle Output pointer receiving the resolved handle.
 * @return UART handle resolution status.
 * @failure_behavior Returns `SECONDARY_UART_STATUS_INVALID_ARGUMENT` for null outputs
 * or unsupported port identifiers.
 * @criticality Mission-critical.
 */
secondary_uart_status_t secondary_uart_bus_get_handle(
    const secondary_uart_port_id_t port_id,
    UART_HandleTypeDef **const out_handle)
{
    secondary_uart_status_t status = SECONDARY_UART_STATUS_INVALID_ARGUMENT;

    if (out_handle != 0)
    {
        if (port_id == SECONDARY_UART_PORT_GPS)
        {
            *out_handle = &g_huart1;
            status = SECONDARY_UART_STATUS_OK;
        }
        else if (port_id == SECONDARY_UART_PORT_BACKUP_LINK)
        {
            *out_handle = &g_huart2;
            status = SECONDARY_UART_STATUS_OK;
        }
        else if (port_id == SECONDARY_UART_PORT_LORA)
        {
            *out_handle = &g_huart3;
            status = SECONDARY_UART_STATUS_OK;
        }
        else
        {
        }
    }

    return status;
}

/**
 * @brief Purpose: Perform a bounded UART transmission on a logical secondary port.
 * @param port_id Logical UART port identifier.
 * @param tx_data Pointer to the transmit buffer.
 * @param length Number of bytes to transmit.
 * @param timeout_ms Bounded timeout in milliseconds.
 * @return UART transmission status.
 * @failure_behavior Returns an explicit status code and does not retry indefinitely.
 * @criticality Mission-critical.
 */
secondary_uart_status_t secondary_uart_bus_transmit(
    const secondary_uart_port_id_t port_id,
    const uint8_t *const tx_data,
    const uint16_t length,
    const uint32_t timeout_ms)
{
    secondary_uart_status_t status = SECONDARY_UART_STATUS_INVALID_ARGUMENT;
    UART_HandleTypeDef *handle = 0;

    if ((tx_data != 0) && (length > 0U) && (timeout_ms > 0U))
    {
        status = secondary_uart_bus_get_handle(port_id, &handle);
    }

    if (status == SECONDARY_UART_STATUS_OK)
    {
        status = secondary_uart_status_from_hal(HAL_UART_Transmit(
            handle, (uint8_t *)tx_data, length, timeout_ms));
    }

    return status;
}

/**
 * @brief Purpose: Perform a bounded UART reception on a logical secondary port.
 * @param port_id Logical UART port identifier.
 * @param rx_data Pointer to the receive buffer.
 * @param length Number of bytes to receive.
 * @param timeout_ms Bounded timeout in milliseconds.
 * @return UART reception status.
 * @failure_behavior Returns an explicit status code and does not retry indefinitely.
 * @criticality Mission-critical.
 */
secondary_uart_status_t secondary_uart_bus_receive(
    const secondary_uart_port_id_t port_id,
    uint8_t *const rx_data,
    const uint16_t length,
    const uint32_t timeout_ms)
{
    secondary_uart_status_t status = SECONDARY_UART_STATUS_INVALID_ARGUMENT;
    UART_HandleTypeDef *handle = 0;

    if ((rx_data != 0) && (length > 0U) && (timeout_ms > 0U))
    {
        status = secondary_uart_bus_get_handle(port_id, &handle);
    }

    if (status == SECONDARY_UART_STATUS_OK)
    {
        status = secondary_uart_status_from_hal(
            HAL_UART_Receive(handle, rx_data, length, timeout_ms));
    }

    return status;
}

#ifndef SECONDARY_FC_DRIVER_SPI_SPI_BUS_H
#define SECONDARY_FC_DRIVER_SPI_SPI_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board_handles.h"

typedef enum
{
    SECONDARY_SPI_BUS_IMU = 0,
    SECONDARY_SPI_BUS_STORAGE = 1,
    SECONDARY_SPI_BUS_COUNT
} secondary_spi_bus_id_t;

typedef enum
{
    SECONDARY_SPI_STATUS_OK = 0,
    SECONDARY_SPI_STATUS_INVALID_ARGUMENT = 1,
    SECONDARY_SPI_STATUS_HAL_ERROR = 2
} secondary_spi_status_t;

/**
 * @brief Purpose: Resolve the SPI handle associated with a secondary board SPI bus.
 * @param bus_id Logical SPI bus identifier.
 * @param out_handle Output pointer that receives the HAL SPI handle.
 * @return SPI bus resolution status.
 * @failure_behavior Returns `SECONDARY_SPI_STATUS_INVALID_ARGUMENT` and leaves
 * `out_handle` unchanged when the inputs are invalid.
 * @criticality Mission-critical.
 */
secondary_spi_status_t secondary_spi_bus_get_handle(
    secondary_spi_bus_id_t bus_id,
    SPI_HandleTypeDef **out_handle);

/**
 * @brief Purpose: Perform a bounded full-duplex SPI transaction.
 * @param bus_id Logical SPI bus identifier.
 * @param tx_data Pointer to the transmit buffer, or `0` when sending dummy bytes.
 * @param rx_data Pointer to the receive buffer, or `0` when received bytes are discarded.
 * @param length Number of bytes to exchange.
 * @param timeout_ms Bounded HAL timeout in milliseconds.
 * @return SPI transaction status.
 * @failure_behavior Returns an explicit status code and does not retry indefinitely.
 * @criticality Mission-critical.
 */
secondary_spi_status_t secondary_spi_bus_transfer(
    secondary_spi_bus_id_t bus_id,
    const uint8_t *tx_data,
    uint8_t *rx_data,
    uint16_t length,
    uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif

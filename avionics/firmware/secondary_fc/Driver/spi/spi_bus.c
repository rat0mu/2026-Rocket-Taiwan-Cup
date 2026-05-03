#include "spi_bus.h"

/**
 * @brief Purpose: Convert a HAL SPI result into a driver status code.
 * @param hal_status HAL return status.
 * @return Converted SPI driver status.
 * @failure_behavior Maps all non-`HAL_OK` results to `SECONDARY_SPI_STATUS_HAL_ERROR`.
 * @criticality Mission-critical.
 */
static secondary_spi_status_t secondary_spi_status_from_hal(
    const HAL_StatusTypeDef hal_status)
{
    secondary_spi_status_t status = SECONDARY_SPI_STATUS_HAL_ERROR;

    if (hal_status == HAL_OK)
    {
        status = SECONDARY_SPI_STATUS_OK;
    }

    return status;
}

/**
 * @brief Purpose: Resolve the HAL SPI handle for a logical secondary bus ID.
 * @param bus_id Logical SPI bus identifier.
 * @param out_handle Output pointer receiving the resolved handle.
 * @return SPI handle resolution status.
 * @failure_behavior Returns `SECONDARY_SPI_STATUS_INVALID_ARGUMENT` for null outputs
 * or unsupported bus identifiers.
 * @criticality Mission-critical.
 */
secondary_spi_status_t secondary_spi_bus_get_handle(
    const secondary_spi_bus_id_t bus_id,
    SPI_HandleTypeDef **const out_handle)
{
    secondary_spi_status_t status = SECONDARY_SPI_STATUS_INVALID_ARGUMENT;

    if (out_handle != 0)
    {
        if (bus_id == SECONDARY_SPI_BUS_IMU)
        {
            *out_handle = &g_hspi1;
            status = SECONDARY_SPI_STATUS_OK;
        }
        else if (bus_id == SECONDARY_SPI_BUS_STORAGE)
        {
            *out_handle = &g_hspi2;
            status = SECONDARY_SPI_STATUS_OK;
        }
        else
        {
        }
    }

    return status;
}

/**
 * @brief Purpose: Perform a bounded SPI transfer on a logical secondary bus.
 * @param bus_id Logical SPI bus identifier.
 * @param tx_data Optional transmit buffer pointer.
 * @param rx_data Optional receive buffer pointer.
 * @param length Number of bytes to transfer.
 * @param timeout_ms Bounded timeout in milliseconds.
 * @return SPI transfer status.
 * @failure_behavior Returns an explicit status code and limits internal chunking
 * to a fixed buffer size.
 * @criticality Mission-critical.
 */
secondary_spi_status_t secondary_spi_bus_transfer(
    const secondary_spi_bus_id_t bus_id,
    const uint8_t *const tx_data,
    uint8_t *const rx_data,
    const uint16_t length,
    const uint32_t timeout_ms)
{
    secondary_spi_status_t status = SECONDARY_SPI_STATUS_INVALID_ARGUMENT;
    SPI_HandleTypeDef *handle = 0;
    static uint8_t g_secondary_spi_dummy_tx[32];
    static uint8_t g_secondary_spi_dummy_rx[32];
    uint16_t remaining_length = 0U;
    uint16_t chunk_length = 0U;
    uint16_t offset = 0U;

    if ((length > 0U) && (timeout_ms > 0U))
    {
        status = secondary_spi_bus_get_handle(bus_id, &handle);
    }

    if (status == SECONDARY_SPI_STATUS_OK)
    {
        remaining_length = length;

        while ((remaining_length > 0U) && (status == SECONDARY_SPI_STATUS_OK))
        {
            chunk_length = (remaining_length > (uint16_t)sizeof(g_secondary_spi_dummy_tx))
                               ? (uint16_t)sizeof(g_secondary_spi_dummy_tx)
                               : remaining_length;

            status = secondary_spi_status_from_hal(HAL_SPI_TransmitReceive(
                handle,
                (uint8_t *)((tx_data != 0) ? &tx_data[offset] : g_secondary_spi_dummy_tx),
                (uint8_t *)((rx_data != 0) ? &rx_data[offset] : g_secondary_spi_dummy_rx),
                chunk_length,
                timeout_ms));

            offset = (uint16_t)(offset + chunk_length);
            remaining_length = (uint16_t)(remaining_length - chunk_length);
        }
    }

    return status;
}

#include "board_support.h"

#include "board_handles.h"

/**
 * @brief Purpose: Convert a HAL return code into a secondary BSP status value.
 * @param hal_status HAL status returned by a low-level initialization call.
 * @return Converted secondary BSP status.
 * @failure_behavior Maps any non-`HAL_OK` result to `SECONDARY_BSP_STATUS_HAL_ERROR`.
 * @criticality DAL-A-like.
 */
static secondary_bsp_status_t secondary_bsp_status_from_hal(
    const HAL_StatusTypeDef hal_status)
{
    secondary_bsp_status_t status = SECONDARY_BSP_STATUS_HAL_ERROR;

    if (hal_status == HAL_OK)
    {
        status = SECONDARY_BSP_STATUS_OK;
    }

    return status;
}

/**
 * @brief Purpose: Configure the STM32F103 system clock tree for the backup computer.
 * @param None.
 * @return Clock initialization status.
 * @failure_behavior Returns `SECONDARY_BSP_STATUS_HAL_ERROR` if oscillator or bus clock
 * configuration fails; callers must inhibit mission activity.
 * @criticality DAL-A-like.
 */
static secondary_bsp_status_t secondary_bsp_clock_init(void)
{
    RCC_OscInitTypeDef rcc_osc_init = {0};
    RCC_ClkInitTypeDef rcc_clk_init = {0};
    secondary_bsp_status_t status = SECONDARY_BSP_STATUS_OK;

    rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    rcc_osc_init.HSEState = RCC_HSE_ON;
    rcc_osc_init.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    rcc_osc_init.HSIState = RCC_HSI_ON;
    rcc_osc_init.PLL.PLLState = RCC_PLL_ON;
    rcc_osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    rcc_osc_init.PLL.PLLMUL = RCC_PLL_MUL9;

    status = secondary_bsp_status_from_hal(HAL_RCC_OscConfig(&rcc_osc_init));

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        rcc_clk_init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
        rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
        rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
        rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

        status = secondary_bsp_status_from_hal(
            HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_2));
    }

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        HAL_RCC_EnableCSS();
    }

    return status;
}

/**
 * @brief Purpose: Initialize all board-level GPIO to deterministic safe defaults.
 * @param None.
 * @return GPIO initialization status.
 * @failure_behavior Returns `SECONDARY_BSP_STATUS_HAL_ERROR` if static validation fails;
 * GPIO writes are still kept in safe low states before configuration.
 * @criticality DAL-A-like.
 */
static secondary_bsp_status_t secondary_bsp_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOA, CS_IMU_GYRO_Pin | CS_IMU_ACCEL_Pin | CS_BARO_Pin,
                      GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,
                      CS_SD_Pin | LORA_RESET_Pin | GPS_RESET_Pin | LORA_M0_Pin |
                          LORA_M1_Pin,
                      GPIO_PIN_RESET);

    gpio_init.Pin = INT1_IMU_ACCEL_Pin | INT3_IMU_GYRO_Pin;
    gpio_init.Mode = GPIO_MODE_IT_RISING;
    gpio_init.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &gpio_init);

    gpio_init.Pin = INT_GPS_Pin | INT_BARO_Pin;
    gpio_init.Mode = GPIO_MODE_IT_RISING;
    gpio_init.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    gpio_init.Pin = CS_IMU_GYRO_Pin | CS_IMU_ACCEL_Pin | CS_BARO_Pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    gpio_init.Pin = CS_SD_Pin | LORA_RESET_Pin | GPS_RESET_Pin | LORA_M0_Pin |
                    LORA_M1_Pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    gpio_init.Pin = LORA_AUX_Pin;
    gpio_init.Mode = GPIO_MODE_IT_RISING;
    gpio_init.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LORA_AUX_GPIO_Port, &gpio_init);

    return SECONDARY_BSP_STATUS_OK;
}

/**
 * @brief Purpose: Initialize the SPI buses used by the secondary flight computer.
 * @param None.
 * @return SPI initialization status.
 * @failure_behavior Returns `SECONDARY_BSP_STATUS_HAL_ERROR` if any SPI controller
 * fails to initialize; callers must avoid sensor access.
 * @criticality DAL-A-like.
 */
static secondary_bsp_status_t secondary_bsp_spi_init(void)
{
    secondary_bsp_status_t status = SECONDARY_BSP_STATUS_OK;

    g_hspi1.Instance = SPI1;
    g_hspi1.Init.Mode = SPI_MODE_MASTER;
    g_hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    g_hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    g_hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    g_hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    g_hspi1.Init.NSS = SPI_NSS_SOFT;
    g_hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    g_hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    g_hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    g_hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    g_hspi1.Init.CRCPolynomial = 10U;

    status = secondary_bsp_status_from_hal(HAL_SPI_Init(&g_hspi1));

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        g_hspi2.Instance = SPI2;
        g_hspi2.Init.Mode = SPI_MODE_MASTER;
        g_hspi2.Init.Direction = SPI_DIRECTION_2LINES;
        g_hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
        g_hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
        g_hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
        g_hspi2.Init.NSS = SPI_NSS_SOFT;
        g_hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
        g_hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
        g_hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
        g_hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        g_hspi2.Init.CRCPolynomial = 10U;

        status = secondary_bsp_status_from_hal(HAL_SPI_Init(&g_hspi2));
    }

    return status;
}

/**
 * @brief Purpose: Initialize the timer used for GPS PPS capture.
 * @param None.
 * @return Timer initialization status.
 * @failure_behavior Returns `SECONDARY_BSP_STATUS_HAL_ERROR` if timer setup fails;
 * callers must treat PPS timing as unavailable.
 * @criticality Mission-critical.
 */
static secondary_bsp_status_t secondary_bsp_timer_init(void)
{
    TIM_MasterConfigTypeDef master_config = {0};
    TIM_IC_InitTypeDef input_capture_config = {0};
    secondary_bsp_status_t status = SECONDARY_BSP_STATUS_OK;

    g_htim2.Instance = TIM2;
    g_htim2.Init.Prescaler = 0U;
    g_htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_htim2.Init.Period = 65535U;
    g_htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    g_htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = secondary_bsp_status_from_hal(HAL_TIM_IC_Init(&g_htim2));

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        master_config.MasterOutputTrigger = TIM_TRGO_RESET;
        master_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

        status = secondary_bsp_status_from_hal(
            HAL_TIMEx_MasterConfigSynchronization(&g_htim2, &master_config));
    }

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        input_capture_config.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
        input_capture_config.ICSelection = TIM_ICSELECTION_DIRECTTI;
        input_capture_config.ICPrescaler = TIM_ICPSC_DIV1;
        input_capture_config.ICFilter = 0U;

        status = secondary_bsp_status_from_hal(HAL_TIM_IC_ConfigChannel(
            &g_htim2, &input_capture_config, TIM_CHANNEL_2));
    }

    return status;
}

/**
 * @brief Purpose: Initialize the UART interfaces used by the secondary computer.
 * @param None.
 * @return UART initialization status.
 * @failure_behavior Returns `SECONDARY_BSP_STATUS_HAL_ERROR` if any UART controller
 * fails to initialize; callers must inhibit dependent communications.
 * @criticality DAL-A-like.
 */
static secondary_bsp_status_t secondary_bsp_uart_init(void)
{
    secondary_bsp_status_t status = SECONDARY_BSP_STATUS_OK;

    g_huart1.Instance = USART1;
    g_huart1.Init.BaudRate = 115200U;
    g_huart1.Init.WordLength = UART_WORDLENGTH_8B;
    g_huart1.Init.StopBits = UART_STOPBITS_1;
    g_huart1.Init.Parity = UART_PARITY_NONE;
    g_huart1.Init.Mode = UART_MODE_TX_RX;
    g_huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    status = secondary_bsp_status_from_hal(HAL_UART_Init(&g_huart1));

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        g_huart2.Instance = USART2;
        g_huart2.Init.BaudRate = 115200U;
        g_huart2.Init.WordLength = UART_WORDLENGTH_8B;
        g_huart2.Init.StopBits = UART_STOPBITS_1;
        g_huart2.Init.Parity = UART_PARITY_NONE;
        g_huart2.Init.Mode = UART_MODE_TX_RX;
        g_huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        g_huart2.Init.OverSampling = UART_OVERSAMPLING_16;

        status = secondary_bsp_status_from_hal(HAL_UART_Init(&g_huart2));
    }

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        g_huart3.Instance = USART3;
        g_huart3.Init.BaudRate = 115200U;
        g_huart3.Init.WordLength = UART_WORDLENGTH_8B;
        g_huart3.Init.StopBits = UART_STOPBITS_1;
        g_huart3.Init.Parity = UART_PARITY_NONE;
        g_huart3.Init.Mode = UART_MODE_TX_RX;
        g_huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        g_huart3.Init.OverSampling = UART_OVERSAMPLING_16;

        status = secondary_bsp_status_from_hal(HAL_UART_Init(&g_huart3));
    }

    return status;
}

secondary_bsp_status_t secondary_bsp_init(void)
{
    secondary_bsp_status_t status = SECONDARY_BSP_STATUS_OK;

    HAL_Init();

    status = secondary_bsp_clock_init();

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        status = secondary_bsp_gpio_init();
    }

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        status = secondary_bsp_spi_init();
    }

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        status = secondary_bsp_timer_init();
    }

    if (status == SECONDARY_BSP_STATUS_OK)
    {
        status = secondary_bsp_uart_init();
    }

    return status;
}

/**
 * @brief Purpose: Enter the board-level fail-safe error trap.
 * @param None.
 * @return None.
 * @failure_behavior Disables interrupts and loops forever to prevent unsafe
 * continuation after unrecoverable initialization or runtime faults.
 * @criticality DAL-A-like.
 */
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}

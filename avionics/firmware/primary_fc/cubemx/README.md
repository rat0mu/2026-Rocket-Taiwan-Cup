# Primary Flight Computer CubeMX Layout

This directory stores the original STM32CubeMX/IAR project used as the board-support source for the Primary Flight Computer (PFC).

The raw project snapshot is kept under [`raw/`](./raw/) and should be treated as generated source material. Do not place mission logic in that tree.

## Source Files

The following raw files are the primary configuration and generated entry points:

- [`raw/Documents.ioc`](./raw/Documents.ioc): STM32CubeMX board configuration source of truth for the STM32F405VG target. This file defines the MCU, pinout, peripheral instances, clock tree, and CubeMX generation settings.
- [`raw/.mxproject`](./raw/.mxproject): CubeMX project metadata. This records how CubeMX previously generated the `Core/`, `Drivers/`, and toolchain files.

## Raw/Core

Files under [`raw/Core/Inc`](./raw/Core/Inc/) and [`raw/Core/Src`](./raw/Core/Src/) are the generated application skeleton and low-level initialization code.

- [`raw/Core/Inc/main.h`](./raw/Core/Inc/main.h): Board-level pin labels and shared generated declarations. Use this as the source for a future `BSP/board_pins.h`.
- [`raw/Core/Inc/stm32f4xx_hal_conf.h`](./raw/Core/Inc/stm32f4xx_hal_conf.h): HAL module configuration for STM32F4. This controls which HAL modules are enabled and how the HAL is built.
- [`raw/Core/Inc/stm32f4xx_it.h`](./raw/Core/Inc/stm32f4xx_it.h): Interrupt handler declarations for the generated startup/IRQ layer.
- [`raw/Core/Src/main.c`](./raw/Core/Src/main.c): Generated single-project entry point. Its value is the `SystemClock_Config()` and `MX_*_Init()` functions. Do not use it as the final mission application entry point.
- [`raw/Core/Src/stm32f4xx_hal_msp.c`](./raw/Core/Src/stm32f4xx_hal_msp.c): HAL MSP layer for clocks, alternate-function GPIO, DMA, and peripheral hardware binding. This is board-support code and belongs conceptually in `BSP/`.
- [`raw/Core/Src/stm32f4xx_it.c`](./raw/Core/Src/stm32f4xx_it.c): Interrupt service routine skeleton for Cortex-M and peripheral handlers. This maps to the runtime core layer.
- [`raw/Core/Src/system_stm32f4xx.c`](./raw/Core/Src/system_stm32f4xx.c): CMSIS system startup code, including `SystemInit()` and system clock state.

## Raw/Drivers

The [`raw/Drivers`](./raw/Drivers/) directory contains third-party vendor dependencies, not project-specific drivers.

- [`raw/Drivers/STM32F4xx_HAL_Driver`](./raw/Drivers/STM32F4xx_HAL_Driver/): ST HAL implementation for GPIO, SPI, UART, SDIO, ADC, timers, and related support modules used by the generated project.
- [`raw/Drivers/CMSIS`](./raw/Drivers/CMSIS/): ARM CMSIS core headers and ST device headers for STM32F4. This includes the device register definitions and core support files required by the HAL and startup code.

These vendor directories should not be mixed with the repository's own [`Driver/`](../Driver/) directory, which is reserved for project-owned bus wrappers and device drivers.

## Raw/EWARM

The [`raw/EWARM`](./raw/EWARM/) directory contains the IAR Embedded Workbench project and toolchain-specific startup/linker assets.

- [`raw/EWARM/Documents.ewp`](./raw/EWARM/Documents.ewp): IAR project definition. This describes which files, include paths, and build options the generated project uses.
- [`raw/EWARM/Documents.ewd`](./raw/EWARM/Documents.ewd): IAR debugger configuration for the generated project.
- [`raw/EWARM/Project.eww`](./raw/EWARM/Project.eww): IAR workspace file.
- [`raw/EWARM/startup_stm32f405xx.s`](./raw/EWARM/startup_stm32f405xx.s): Startup assembly file containing the vector table and reset entry.
- [`raw/EWARM/stm32f405xx_flash.icf`](./raw/EWARM/stm32f405xx_flash.icf): IAR linker script for flash execution.
- [`raw/EWARM/stm32f405xx_sram.icf`](./raw/EWARM/stm32f405xx_sram.icf): IAR linker script for SRAM execution.

## Intended Mapping Into `primary_fc`

The raw CubeMX output is not the final firmware architecture. It should feed the existing repository layout:

- `raw/Documents.ioc` stays as the hardware configuration source of truth under `cubemx/raw/`.
- `raw/Core/Src/system_stm32f4xx.c`, `raw/Core/Src/stm32f4xx_it.c`, `raw/Core/Inc/stm32f4xx_it.h`, and `raw/EWARM/startup_stm32f405xx.s` map conceptually to [`Core/`](../Core/).
- `raw/Core/Inc/main.h` and `raw/Core/Src/stm32f4xx_hal_msp.c` map conceptually to [`BSP/`](../BSP/).
- The `MX_*_Init()` functions in `raw/Core/Src/main.c` should be split into board-support source files such as `BSP/board_gpio.c`, `BSP/board_spi.c`, `BSP/board_uart.c`, `BSP/board_adc.c`, `BSP/board_sdio.c`, and `BSP/board_timer.c`.
- `raw/Drivers/*` remain vendor dependencies and should not replace project-owned code in [`Driver/`](../Driver/).
- Application logic, state estimation, telemetry, storage, and pyro logic remain in [`App/`](../App/), [`Service/`](../Service/), [`Module/`](../Module/), and [`Actuator/`](../Actuator/).

## Working Rule

When hardware settings change, update `Documents.ioc`, regenerate the raw project, and then selectively merge the board-support changes into the formal `primary_fc` structure. Avoid editing the raw generated files as if they were the final firmware implementation.

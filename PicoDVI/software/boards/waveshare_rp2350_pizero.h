// pico_cmake_set PICO_PLATFORM=rp2350

#ifndef WAVESHARE_RP2350_PIZERO_H
#define WAVESHARE_RP2350_PIZERO_H

// For board detection
#define WAVESHARE_RP2350_PIZERO
#define WAVESHARE_RP2350_PIZERO_16MB

// --- RP2350 VARIANT ---
#define PICO_RP2350A 0

// --- BOARD SPECIFIC ---
#define WAVESHARE_RP2350_PIZERO_PSRAM_CS_PIN 47

// -- for Sparkfun PSRAM library
#define SFE_RP2350_XIP_CSI_PIN WAVESHARE_RP2350_PIZERO_PSRAM_CS_PIN

// --- UART ---
#ifndef PICO_DEFAULT_UART
#define PICO_DEFAULT_UART 0
#endif
#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 0
#endif
#ifndef PICO_DEFAULT_UART_RX_PIN
#define PICO_DEFAULT_UART_RX_PIN 1
#endif

// --- LED ---
#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 25
#endif
// no PICO_DEFAULT_WS2812_PIN

// --- I2C ---
#ifndef PICO_DEFAULT_I2C
#define PICO_DEFAULT_I2C 0
#endif
#ifndef PICO_DEFAULT_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SDA_PIN 4
#endif
#ifndef PICO_DEFAULT_I2C_SCL_PIN
#define PICO_DEFAULT_I2C_SCL_PIN 5
#endif

// --- SPI ---
#ifndef PICO_DEFAULT_SPI
#define PICO_DEFAULT_SPI 0
#endif
#ifndef PICO_DEFAULT_SPI_SCK_PIN
#define PICO_DEFAULT_SPI_SCK_PIN 18
#endif
#ifndef PICO_DEFAULT_SPI_TX_PIN
#define PICO_DEFAULT_SPI_TX_PIN 19
#endif
#ifndef PICO_DEFAULT_SPI_RX_PIN
#define PICO_DEFAULT_SPI_RX_PIN 16
#endif
#ifndef PICO_DEFAULT_SPI_CSN_PIN
#define PICO_DEFAULT_SPI_CSN_PIN 17
#endif

// --- FLASH ---

#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 2
#endif

// pico_cmake_set_default PICO_FLASH_SIZE_BYTES = (16 * 1024 * 1024)
#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (16 * 1024 * 1024)
#endif

// Drive high to force power supply into PWM mode (lower ripple on 3V3 at light loads)
#define PICO_SMPS_MODE_PIN 23

// The GPIO Pin used to read VBUS to determine if the device is battery powered.
// #ifndef PICO_VBUS_PIN
// #define PICO_VBUS_PIN 24
// #endif

// The GPIO Pin used to monitor VSYS. Typically you would use this with ADC.
// There is an example in adc/read_vsys in pico-examples.
// #ifndef PICO_VSYS_PIN
// #define PICO_VSYS_PIN 29
// #endif

// pico_cmake_set_default PICO_RP2350_A2_SUPPORTED = 1
#ifndef PICO_RP2350_A2_SUPPORTED
#define PICO_RP2350_A2_SUPPORTED 1
#endif

//----------------------------------------------------------------------------

#define RP2350_PIZERO_SDCART_SCK_GPIO       30
#define RP2350_PIZERO_SDCART_MOSI_GPIO      31
#define RP2350_PIZERO_SDCART_MISO_GPIO      40
#define RP2350_PIZERO_SDCART_CS_GPIO        43
#define RP2350_PIZERO_SDCART_SPI            spi1

#define RP2350_PIZERO_HDMI_D0_PLUS          36
#define RP2350_PIZERO_HDMI_D0_MINUS         37
#define RP2350_PIZERO_HDMI_CLK_PLUS         38
#define RP2350_PIZERO_HDMI_CLK_MINUS        39
#define RP2350_PIZERO_HDMI_D2_PLUS          32
#define RP2350_PIZERO_HDMI_D2_MINUS         33
#define RP2350_PIZERO_HDMI_D1_PLUS          34
#define RP2350_PIZERO_HDMI_D1_MINUS         35

// #ifndef DVI_DEFAULT_SERIAL_CONFIG
#define DVI_DEFAULT_SERIAL_CONFIG           waveshare_rp2350_pizero_cfg
// #endif

#endif // WAVESHARE_RP2350_PIZERO_H

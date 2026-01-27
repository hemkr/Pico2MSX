#ifndef BOARD_OLIMEXPC_H
#define BOARD_OLIMEXPC_H

// Board: Olimex RP2350-PC
// Configuration for picomsx emulator

#define BOARD_NAME "Olimex RP2350-PC"

// VGA/Video DMA channels
#define VGA_DMA_CHANNEL 2 // requires 2 channels
#define AUD_DMA_CHANNEL 4 // requires 1 or 3 channels
#define PSR_DMA_CHANNEL 7 // requires 2 channels (PSRAM)

// Speaker / Audio (I2S on Olimex)
// Using I2S data pin for compatibility with AudioPlaySystem
#define AUDIO_PIN       26  // PWM_AUDIO-L (can also use GPIO27 for stereo)

// TFT (if used instead of HDMI)
#define TFT_SPIREG      spi1
#define TFT_SPIDREQ     DREQ_SPI1_TX
#define TFT_SCLK        30
#define TFT_MOSI        31
#define TFT_MISO        28
#define TFT_DC          3
#define TFT_CS          46  // 255 for LORES ST7789 (NO CS)
#define TFT_RST         22  // 255 for ILI/ST if connected to 3.3V
#define TFT_BACKLIGHT   255 // hardwired to 3.3v

// Buttons
// WARNING: GPIO0-2 are used for UART and USB on Olimex
// GPIO4-5 may conflict with PSRAM SPI
// Disable user buttons to avoid GPIO conflicts
// #define PIN_KEY_USER1   0   // BUTTON1 - CONFLICTS WITH UART0_TX
// #define PIN_KEY_USER2   4   // BUTTON2 - CONFLICTS WITH PSRAM_MISO
// #define PIN_KEY_USER3   5   // BUTTON3 - CONFLICTS WITH PSRAM_CS

// SD Card (same as Fruit Jam)
#define SD_SCLK         34
#define SD_MOSI         35
#define SD_MISO         36 
#define SD_CS           39
#define SD_DETECT       33
#define SD_SPIREG       spi0  // Second SPI bus

// PSRAM (Olimex has QSPI flash on different pins)
// Using SPI0 alternative pins
#define PSRAM_PIN_SCK   6   // SPIO_SCK
#define PSRAM_PIN_MOSI  7   // SPIO_TX(MOSI)
#define PSRAM_PIN_MISO  4   // SPIO_RX(MISO)
#define PSRAM_PIN_CS    5   // SPIO_Csn
#define PSRAM_ASYNC     1
#define PSRAM_SCLK      6
#define PSRAM_MOSI      7
#define PSRAM_MISO      4
#define PSRAM_CS        5
#define PSRAM_SPIREG    spi0
// Override global default from platform_config.h
#ifdef PSRAM_CHIP_SELECT
#undef PSRAM_CHIP_SELECT
#endif
#define PSRAM_CHIP_SELECT (8u)  // QMI_CS1n

// HDMI HSTX TMDS pairs
// Olimex RP2350-PC has different HDMI routing than Fruit Jam
// CKP is on GPIO15 (not GPIO13)
// D0P is on GPIO13 (not GPIO15)
// This is similar to Adafruit Metro RP2350 routing
#define PIN_CKP         (15u)  // Clock pair P (GPIO15) - HDMI_CK-
// V2_inverted: Try using + pins instead of - pins (12 instead of 13, etc)
#define PIN_D0P         (12u)  // Data 0 pair P (GPIO12) - HDMI_D0+ 
#define PIN_D1P         (16u)  // Data 1 pair P (GPIO16) - HDMI_D2+
#define PIN_D2P         (18u)  // Data 2 pair P (GPIO18) - HDMI_D1+

// USB Host (PIO USB)
// Default mapping assumes D+=GP1, D-=GP2. Some Olimex revisions may swap DP/DM.
#ifndef USB_DPDM_INVERT
#define PIN_USB_HOST_DP     (1u)   // USB D+ on GPIO1
#define PIN_USB_HOST_DM     (2u)   // USB D- on GPIO2
#else
#define PIN_USB_HOST_DP     (2u)   // USB D+ on GPIO2 (inverted)
#define PIN_USB_HOST_DM     (1u)   // USB D- on GPIO1 (inverted)
#endif
// Olimex RP2350-PC does not have a USB 5V switch on a GPIO by default.
// Leave VBUS control undefined; provide external 5V (powered hub or jumper).
// #define PIN_USB_HOST_VBUS   (11u)
// If you've added a GPIO-controlled 5V switch to your hardware, uncomment the
// line above and wire it accordingly so firmware can enable VBUS.

// Additional Olimex features
#define PIN_USER_LED    25  // User LED

// I2S Audio pins (for future enhanced audio support)
#define PIN_I2S_AUDIO_L 26  // PWM_AUDIO-L
#define PIN_I2S_AUDIO_R 27  // PWM_AUDIO-R
#define PIN_CODEC_MCLK  23  // CODEC_MCLK
#define PIN_CODEC_LRCK  29  // CODEC_LRCK
#define PIN_CODEC_SCLK  30  // CODEC_SCLK
#define PIN_CODEC_SDIN  31  // CODEC_SDIN

#endif // BOARD_OLIMEXPC_H

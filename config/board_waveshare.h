#ifndef BOARD_WAVESHARE_H
#define BOARD_WAVESHARE_H

// Board: Waveshare RP2350 HDMI (PiZero form factor)
// 16MB Flash + 16MB PSRAM (GPIO47)
// HDMI via PicoDVI on GPIO32-39 (actual hardware pins)
// USB Host via PIO-USB on GPIO28/29
// SD Card on SPI1 (SDIO pins: GPIO30/31/40/43)

#define BOARD_NAME "Waveshare RP2350 HDMI"

// Board identification for PicoDVI library
#define WAVESHARE_RP2350_PIZERO
#define WAVESHARE_RP2350_PIZERO_16MB

// HDMI output method: PicoDVI (not HSTX)
#define USE_LIBDVI          1
#define HDMI_OVER_HSTX      0

// HDMI pins (actual hardware GPIO32-39 on Waveshare)
// These are the physical HDMI connector pins on the board
#define HDMI_D0_PLUS        36
#define HDMI_D0_MINUS       37
#define HDMI_CLK_PLUS       38
#define HDMI_CLK_MINUS      39
#define HDMI_D2_PLUS        32
#define HDMI_D2_MINUS       33
#define HDMI_D1_PLUS        34
#define HDMI_D1_MINUS       35

// DDC/CEC pins (for reference)
#define DVI_SDA             44
#define DVI_SCL             45
#define DVI_CEC             46

// USB Host (PIO-USB)
#define PIN_USB_HOST_DP     28
#define PIN_USB_HOST_DM     29  // USB D- adjacent to D+
#define HAS_USBPIO          1
#define PIO_USB_DP_PIN_DEFAULT  28

// SD Card (SPI1 on SDIO pins - Waveshare routing)
#define SD_SPIREG           spi1
#define SD_SCLK             30
#define SD_MOSI             31
#define SD_MISO             40
#define SD_CS               43
#define SD_DETECT           255  // No card detect

// PSRAM (16MB on GPIO47)
#define HAS_PSRAM           1
#define PSRAM_CS_PIN        47
#define SFE_RP2350_XIP_CSI_PIN  PSRAM_CS_PIN

// Audio (GPIO21 PWM)
#define HAS_SND             1
#define AUDIO_PIN           21
#define AUD_DMA_CHANNEL     4

// DMA channels
#define VGA_DMA_CHANNEL     2  // PicoDVI requires 2 channels

// TFT (not used on this board, but needed for compilation)
#define TFT_SPIREG          spi1
#define TFT_SPIDREQ         DREQ_SPI1_TX
#define TFT_SCLK            255
#define TFT_MOSI            255
#define TFT_MISO            255
#define TFT_DC              255
#define TFT_CS              255
#define TFT_RST             255
#define TFT_BACKLIGHT       255

// UART1 (GPIO4=TX / GPIO5=RX - confirmed working)
// These are configured via CMakeLists.txt but documented here
// #define UART_TX 4
// #define UART_RX 5

#endif // BOARD_WAVESHARE_H

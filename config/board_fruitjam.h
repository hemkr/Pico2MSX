#ifndef BOARD_FRUITJAM_H
#define BOARD_FRUITJAM_H

// Board: Adafruit Feather RP2350 (Fruit Jam)
// Configuration for picomsx emulator

#define BOARD_NAME "Adafruit Feather RP2350 (Fruit Jam)"

// VGA/Video DMA channels
#define VGA_DMA_CHANNEL 2 // requires 2 channels
#define AUD_DMA_CHANNEL 4 // requires 1 or 3 channels
#define PSR_DMA_CHANNEL 7 // requires 2 channels (PSRAM)

// Speaker / Audio
#define AUDIO_PIN       21

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
#define PIN_KEY_USER1   0   // BUTTON1
#define PIN_KEY_USER2   4   // BUTTON2

// SD Card
#define SD_SCLK         34
#define SD_MOSI         35
#define SD_MISO         36 
#define SD_CS           39
#define SD_DETECT       33
#define SD_SPIREG       spi0  // Second SPI bus and DMA not conflicting with USB

// PSRAM (if available)
#define PSRAM_PIN_SCK   10
#define PSRAM_PIN_MOSI  11
#define PSRAM_PIN_MISO  8 
#define PSRAM_PIN_CS    9
#define PSRAM_ASYNC     1
#define PSRAM_SCLK      10
#define PSRAM_MOSI      11
#define PSRAM_MISO      8 
#define PSRAM_CS        9
#define PSRAM_SPIREG    spi1
#define PSRAM_CHIP_SELECT (47u)

// HDMI HSTX TMDS pairs
// Fruit Jam uses odd-side P pins (standard Pico 2 routing)
#define PIN_CKP         (13u)  // Clock pair P (GPIO13)
#define PIN_D0P         (15u)  // Data 0 pair P (GPIO15)
#define PIN_D1P         (17u)  // Data 1 pair P (GPIO17)
#define PIN_D2P         (19u)  // Data 2 pair P (GPIO19)

// USB Host (PIO USB)
#define PIN_USB_HOST_DP     (1u)   // USB D+ on GPIO1
#define PIN_USB_HOST_DM     (2u)   // USB D- on GPIO2
#define PIN_USB_HOST_VBUS   (11u)  // USB VBUS control on GPIO11

#endif // BOARD_FRUITJAM_H

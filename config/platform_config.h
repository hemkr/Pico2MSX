#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#define PICOHYPERPET   1
#define KEYLAYOUT      KLAYOUT_ES

#define INVX           1
#define HAS_SND        1
#define USE_VGA        1
// If using Adafruit Metro RP2350 with HDMI, enable the alternate HSTX pinout
// #define HDMI_PINOUT_METRO 1
// USB Host support for keyboard
// Use native USB on boards that wire DP/DM to the RP2350 USB pins (e.g., Olimex).
// Enable PIO-USB only on boards that route USB via GPIOs (e.g., FruitJam).
#define HAS_USBHOST    1
#if defined(BOARD_FRUITJAM) || defined(BOARD_WAVESHARE)
#define HAS_USBPIO     1
#endif

#ifndef PSRAM_CHIP_SELECT
#define PSRAM_CHIP_SELECT (47u)
#endif
//#define ILI9341        1
//#define ST7789         1
//#define SWAP_JOYSTICK  1
//#define LOHRES         1
//#define ROTATE_SCREEN  1
//#define FLIP_SCREEN  1
//#define HAS_PSRAM      1

#ifdef HAS_SND

#define SOUNDRATE 22050                           // sound rate [Hz]

//#define AUDIO_8BIT     1
#define AUDIO_1DMA      1


#ifdef AUDIO_8BIT
typedef uint8_t  audio_sample;
#else
typedef short  audio_sample;
#endif

#endif

#include "program_config.h"

#endif

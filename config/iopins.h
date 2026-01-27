#ifndef IOPINS_H
#define IOPINS_H

#include "platform_config.h"

// Board-specific pin definitions
// Select board configuration based on BOARD_* compile definitions
// Default: BOARD_FRUITJAM if not specified

#if defined(BOARD_OLIMEXPC)
    #include "board_olimexpc.h"
    #ifndef BOARD_SELECTED
        #define BOARD_SELECTED "Olimex RP2350-PC"
    #endif
#elif defined(BOARD_WAVESHARE)
    #include "board_waveshare.h"
    #ifndef BOARD_SELECTED
        #define BOARD_SELECTED "Waveshare RP2350 HDMI"
    #endif
#else
    #include "board_fruitjam.h"
    #ifndef BOARD_SELECTED
        #define BOARD_SELECTED "Adafruit Feather RP2350 (Fruit Jam)"
    #endif
#endif

// Ensure all required pins are defined
#ifndef USE_LIBDVI
// Only validate HSTX pins if not using PicoDVI
#ifndef PIN_CKP
    #error "HDMI Clock pin (PIN_CKP) not defined for this board"
#endif

// Sanity checks for HDMI pins with HSTX: must be within GPIO12..19 and in adjacent pairs
#if (PIN_CKP < 12) || (PIN_CKP > 19) || (PIN_D0P < 12) || (PIN_D0P > 19) || (PIN_D1P < 12) || (PIN_D1P > 19) || (PIN_D2P < 12) || (PIN_D2P > 19)
#error "HDMI TMDS P pins must be within GPIO12..19 for this HSTX driver (check board_*.h)"
#endif

#if (((PIN_CKP ^ 1) != (PIN_CKP + 1) && (PIN_CKP ^ 1) != (PIN_CKP - 1)) || \
     ((PIN_D0P ^ 1) != (PIN_D0P + 1) && (PIN_D0P ^ 1) != (PIN_D0P - 1)) || \
     ((PIN_D1P ^ 1) != (PIN_D1P + 1) && (PIN_D1P ^ 1) != (PIN_D1P - 1)) || \
     ((PIN_D2P ^ 1) != (PIN_D2P + 1) && (PIN_D2P ^ 1) != (PIN_D2P - 1)))
#error "HDMI TMDS P pins must have their N partner on the adjacent GPIO (P^1)."
#endif
#endif // USE_LIBDVI

#ifndef PIN_USB_HOST_DP
    #error "USB Host D+ pin not defined for this board"
#endif

#endif // IOPINS_H

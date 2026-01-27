#ifndef DISPLAY_BACKEND_H
#define DISPLAY_BACKEND_H

#include <stdint.h>
#include <stddef.h>

// Common display backend interface
// Abstracts HSTX (FruitJam/Olimex) vs PicoDVI (Waveshare) backends

#ifdef __cplusplus
extern "C" {
#endif

// Initialize display backend
void display_backend_init(uint16_t width, uint16_t height);

// Get framebuffer pointer
// - For PicoDVI (Waveshare): 8bpp RGB332 buffer (uint8_t*)
// - For HSTX (FruitJam/Olimex): RGB565 buffer (exposed as uint8_t* for compatibility)
uint8_t* display_backend_get_framebuffer(void);

// Get framebuffer dimensions
uint16_t display_backend_get_width(void);
uint16_t display_backend_get_height(void);
uint16_t display_backend_get_stride(void);  // Words per line

// Wait for vsync (optional, for frame timing)
void display_backend_vsync(void);

#ifdef __cplusplus
}
#endif

#endif // DISPLAY_BACKEND_H

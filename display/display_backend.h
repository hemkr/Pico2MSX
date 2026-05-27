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

// ---------------------------------------------------------------------------
// Audio API (PicoDVI backend only – routes PCM into the HDMI audio stream)
// ---------------------------------------------------------------------------

// Register the application fill-callback and pre-fill the ring buffer.
// `callback` receives a mono int16_t buffer of `samplesize` samples.
// Call this once before the main loop (replaces multicore_launch_core1 for audio).
void display_backend_audio_begin(void (*callback)(short *stream, int len),
                                 int samplesize);

// Push `count` mono int16_t samples into the audio ring buffer.
// Returns the number of samples actually written (may be less if ring is full).
uint32_t display_backend_write_audio(const int16_t *samples, uint32_t count);

// Returns the number of free sample slots in the audio ring buffer.
uint32_t display_backend_get_free_audio(void);

#ifdef __cplusplus
}
#endif

#endif // DISPLAY_BACKEND_H

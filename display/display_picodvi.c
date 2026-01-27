// PicoDVI display backend for Waveshare RP2350 HDMI
// Implements HDMI output via PIO instead of HSTX

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "dvi.h"
#include "dvi_serialiser.h"
#include "dvi_timing.h"
#include "common_dvi_pin_configs.h"
#include "tmds_encode.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "display_backend.h"

// DVI instance
static struct dvi_inst dvi0;

// Framebuffer (8bpp RGB332 to match existing VGA paths)
static uint8_t *framebuffer = NULL;
static uint16_t fb_width = 0;
static uint16_t fb_height = 0;
static uint16_t fb_stride = 0; // bytes per line

// Core1 scanline callback - feeds lines to DVI encoder
static void __not_in_flash_func(core1_scanline_callback)(uint scanline_id) {
    // Discard any scanline pointers passed back
    uint8_t *bufptr;
    while (queue_try_remove_u32(&dvi0.q_colour_free, &bufptr))
        ;

    // Advance sequentially through scanlines (decoupled from v_ctr value)
    static uint next_line = 2; // 0 and 1 were queued before start
    bufptr = &framebuffer[fb_stride * next_line];
    queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
    next_line = (next_line + 1) % fb_height;
    (void)scanline_id;
}

// Core1 main loop - runs DVI encoder
static void __not_in_flash_func(dvi_core1_main)(void) {
    // Use DMA IRQ 0 for DVI to avoid conflicts with audio I2S which uses IRQ 1
    dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
    dvi_start(&dvi0);
    // Use 8bpp path (RGB332): matches existing framebuffer writes
    dvi_scanbuf_main_8bpp(&dvi0);
}

void display_backend_init(uint16_t width, uint16_t height) {
    // Boost core voltage and system clock to meet TMDS encode throughput
    // VESA 640x480@60 requires 252 MHz bit clock; PicoDVI typically runs sysclk at this rate
    vreg_set_voltage(VREG_VOLTAGE_1_20);
    sleep_ms(10);
    set_sys_clock_khz(dvi_timing_640x480p_60hz.bit_clk_khz, true);
    // Re-init default UART after clock change to avoid garbled output
    setup_default_uart();

    printf("[PicoDVI] Initializing display backend...\n");
    printf("[PicoDVI] Resolution: %dx%d\n", width, height);
    
    fb_width = width;
    fb_height = height;
    // For 640x480 timing, the 8bpp scanbuf path consumes half-res scanlines of 320 pixels.
    // Ensure stride is at least 320 bytes so the encoder doesn't read past row bounds
    // when emulation width is narrower (e.g., 256x240).
    fb_stride = (width < 320) ? 320 : width;  // bytes per line
    
    // Allocate framebuffer (8bpp RGB332)
    size_t fb_size = (size_t)fb_stride * fb_height;
    framebuffer = (uint8_t*)malloc(fb_size);
    if (!framebuffer) {
        printf("[PicoDVI] ERROR: Failed to allocate framebuffer!\n");
        return;
    }
    
    // Clear framebuffer to black
    memset(framebuffer, 0x00, fb_size);
    // Draw simple color bars test pattern (helps verify HDMI signal)
    {
        const uint8_t bars[8] = {
            0xFF, // white
            0xFC, // yellow (R=7,G=7,B=0)
            0xE3, // magenta (R=7,G=0,B=3)
            0x1F, // cyan (R=0,G=7,B=3)
            0xE0, // red
            0x1C, // green
            0x03, // blue
            0x00  // black
        };
        uint band_h = fb_height / 8u;
        for (uint b = 0; b < 8; ++b) {
            uint y0 = b * band_h;
            uint y1 = (b == 7) ? fb_height : y0 + band_h;
            for (uint y = y0; y < y1; ++y) {
                uint8_t *row = &framebuffer[y * fb_stride];
                // Fill the entire stride so bars span the full active width (320 half-res → 640 output)
                memset(row, bars[b], fb_stride);
            }
        }
    }
    printf("[PicoDVI] Framebuffer allocated at %p (%zu bytes)\n", framebuffer, fb_size);
    
    // Set up PIO for DVI on GPIO16+ (base)
    // PicoDVI expects the TMDS pairs on consecutive GPIO pins starting from base
    // For Waveshare, we need to route to GPIO32-39 via pio_set_gpio_base
    // The example uses GPIO16 as base and then routes via PIO
    pio_set_gpio_base(DVI_DEFAULT_SERIAL_CONFIG.pio, 16);
    
    // Configure DVI instance
    dvi0.timing = &dvi_timing_640x480p_60hz;  // Standard VGA timing
    dvi0.ser_cfg = DVI_DEFAULT_SERIAL_CONFIG;
    dvi0.scanline_callback = core1_scanline_callback;
    dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());
    
    printf("[PicoDVI] DVI timing: %dx%d @ 60Hz\n",
           dvi0.timing->h_active_pixels,
           dvi0.timing->v_active_lines);
    printf("[PicoDVI] PIO%d TMDS pins: %u,%u,%u CLK:%u invert:%d\n",
        (DVI_DEFAULT_SERIAL_CONFIG.pio == pio0) ? 0 : 1,
        DVI_DEFAULT_SERIAL_CONFIG.pins_tmds[0],
        DVI_DEFAULT_SERIAL_CONFIG.pins_tmds[1],
        DVI_DEFAULT_SERIAL_CONFIG.pins_tmds[2],
        DVI_DEFAULT_SERIAL_CONFIG.pins_clk,
        DVI_DEFAULT_SERIAL_CONFIG.invert_diffpairs);
    
    // Push first two scanlines to start the pipeline
    uint8_t *bufptr = framebuffer;
    queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
    bufptr += fb_stride;
    queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
    
    // Launch Core1 to run DVI encoder
    printf("[PicoDVI] Launching Core1 for DVI encoding...\n");
    multicore_launch_core1(dvi_core1_main);
    
    printf("[PicoDVI] Initialization complete.\n");
}

uint8_t* display_backend_get_framebuffer(void) {
    return framebuffer;
}

uint16_t display_backend_get_width(void) {
    return fb_width;
}

uint16_t display_backend_get_height(void) {
    return fb_height;
}

uint16_t display_backend_get_stride(void) {
    return fb_stride;
}

void display_backend_vsync(void) {
    // PicoDVI handles vsync internally; no action needed
    // Could add frame timing here if needed
}

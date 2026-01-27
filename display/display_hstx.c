// HSTX display backend for FruitJam/Olimex boards
// Wraps existing hdmi_framebuffer.cpp HSTX driver

#include "display_backend.h"
#include "hdmi_framebuffer.h"
#include <stdio.h>

static hdmi_framebuffer_obj_t fb_obj;
static uint16_t *framebuffer = NULL;

void display_backend_init(uint16_t width, uint16_t height) {
    printf("[HSTX] Initializing display backend...\n");
    printf("[HSTX] Resolution: %dx%d\n", width, height);
    
    // Initialize HSTX framebuffer (existing code)
    // color_depth = 16 for RGB565
    hdmi_framebuffer(&fb_obj, width, height, 16);
    
    // Store framebuffer pointer as uint16_t* for RGB565 access
    framebuffer = (uint16_t*)fb_obj.framebuffer;
    
    printf("[HSTX] Framebuffer at %p\n", framebuffer);
    printf("[HSTX] Initialization complete.\n");
}

uint8_t* display_backend_get_framebuffer(void) {
    // Expose as byte pointer for compatibility with generic callers
    return (uint8_t*)framebuffer;
}

uint16_t display_backend_get_width(void) {
    return fb_obj.width;
}

uint16_t display_backend_get_height(void) {
    return fb_obj.height;
}

uint16_t display_backend_get_stride(void) {
    // HSTX pitch is in 32-bit words; convert to 16-bit words
    return fb_obj.pitch * 2;
}

void display_backend_vsync(void) {
    hdmi_framebuffer_vsync();
}

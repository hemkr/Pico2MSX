#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/vreg.h"
#include "pico/sem.h"

#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"
#include "sprite.h"

#include "../AUDIO/DING.h"

// TMDS bit clock 252 MHz
// DVDD 1.2V (1.1V seems ok too)
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

#define LED_PIN 16

struct dvi_inst dvi0;
uint16_t framebuf[FRAME_WIDTH * FRAME_HEIGHT];

// ---------------------------------------------------------------------------
// Audio Related

#define AUDIO_BUFFER_SIZE   256
audio_sample_t      audio_buffer[AUDIO_BUFFER_SIZE];
struct repeating_timer audio_timer;
const unsigned short *waveBuffer = &DING_WAV[2];
size_t waveBufferLen = DING_WAV[0] - 2;

bool audio_timer_callback(struct repeating_timer *t) {
    int size = get_write_size(&dvi0.audio_ring, false);
    audio_sample_t *audio_ptr = get_write_pointer(&dvi0.audio_ring);
    audio_sample_t sample;
    static uint sample_count = 0;
    for (int cnt = 0; cnt < size; cnt++) {
        // sample.channels[0] = sine[sample_count % 32];
        // sample.channels[1] = sine[sample_count % 32];
        sample.channels[0] = waveBuffer[sample_count % waveBufferLen];
        sample.channels[1] = waveBuffer[sample_count % waveBufferLen];
        *audio_ptr++ = sample;
        sample_count++;
    }
    increase_write_pointer(&dvi0.audio_ring, size);

    return true;
}

// ---------------------------------------------------------------------------

void core1_main() {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	dvi_start(&dvi0);
	dvi_scanbuf_main_16bpp(&dvi0);
	__builtin_unreachable();
}

void core1_scanline_callback() {
	// Discard any scanline pointers passed back
	uint16_t *bufptr;
	while (queue_try_remove_u32(&dvi0.q_colour_free, &bufptr))
		;
	// // Note first two scanlines are pushed before DVI start
	static uint scanline = 2;
	bufptr = &framebuf[FRAME_WIDTH * scanline];
	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
	scanline = (scanline + 1) % FRAME_HEIGHT;
}

int main() {
	vreg_set_voltage(VREG_VSEL);
	sleep_ms(10);
#ifdef RUN_FROM_CRYSTAL
	set_sys_clock_khz(12000, true);
#else
	// Run system at TMDS bit clock
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);
#endif

	setup_default_uart();

	pio_set_gpio_base(DVI_DEFAULT_SERIAL_CONFIG.pio,16);

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	printf("Configuring DVI\n");

	dvi0.timing = &DVI_TIMING;
	dvi0.ser_cfg = DVI_DEFAULT_SERIAL_CONFIG;
	dvi0.scanline_callback = core1_scanline_callback;
	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

    // HDMI Audio related
    dvi_audio_sample_buffer_set(&dvi0, audio_buffer, AUDIO_BUFFER_SIZE);
    dvi_set_audio_freq(&dvi0, 44100, 28000, 6272);
    add_repeating_timer_ms(2, audio_timer_callback, NULL, &audio_timer);

	// Once we've given core 1 the framebuffer, it will just keep on displaying
	// it without any intervention from core 0
	sprite_fill16(framebuf, 0xffff, FRAME_WIDTH * FRAME_HEIGHT);
	uint16_t *bufptr = framebuf;
	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
	bufptr += FRAME_WIDTH;
	queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

	printf("Core 1 start\n");
	multicore_launch_core1(core1_main);

	printf("Start rendering\n");

	for (int y = 0; y < FRAME_HEIGHT; ++y) {
		for (int x = 0; x < FRAME_WIDTH; ++x) {
			const float scale = FRAME_HEIGHT / 2;
			float cr = ((float)x - FRAME_WIDTH / 2) / scale - 0.5f;
			float ci = ((float)y - FRAME_HEIGHT / 2) / scale;
			float zr = cr;
			float zi = ci;
			int iters;
			const int max_iters = 255;
			for (iters = 0; iters < max_iters; ++iters) {
				if (zr * zr + zi * zi > 4.f)
					break;
				float zrtemp = zr * zr - zi * zi + cr;
				zi = 2.f * zr * zi + ci;
				zr = zrtemp;
			}
			framebuf[y * FRAME_WIDTH + x] = ((max_iters - iters) >> 2) * 0x41 >> 1;
		}
	}
	while (1)
		__wfe();
	__builtin_unreachable();
}


#include "pico.h"
#include "pico/stdlib.h"

extern "C" {
  #include "iopins.h"  
  #include "emuapi.h"  
}
#include "keyboard_osd.h"

extern "C" {
#include "fmsx.h"
}
#include <stdio.h>

#include <stdio.h>
#include "pico_dsp.h"

volatile bool vbl=true;

bool repeating_timer_callback(struct repeating_timer *t) {
    if (vbl) {
        vbl = false;
    } else {
        vbl = true;
    }   
    return true;
}

PICO_DSP tft;
static int skip=0;

#include "hardware/clocks.h"
#include "hardware/vreg.h"

#include "hdmi_framebuffer.h"

int main(void) {
//    vreg_set_voltage(VREG_VOLTAGE_1_05);
//    set_sys_clock_khz(125000, true);    
//    set_sys_clock_khz(150000, true);    
//    set_sys_clock_khz(133000, true);    
//    set_sys_clock_khz(200000, true);    
//    set_sys_clock_khz(210000, true);    
//    set_sys_clock_khz(230000, true);    
//    set_sys_clock_khz(225000, truxe);    
//    set_sys_clock_khz(250000, true);  

#ifdef HAS_USBPIO
    // PIO USB requires multiple of 48 MHz for USB timing
    // 132 MHz = 48 MHz * 2.75, good compromise between USB and HDMI
    // HDMI: 132/5 = 26.4 MHz pixel clock (close enough to VGA 25.175 MHz for capture devices)
    set_sys_clock_khz(132000, true);
    *((uint32_t *)(0x40010000+0x58)) = 5 << 16; // HSTX clock/5 = 26.4MHz
#else
    // For HDMI: sysclk=250MHz, then HSTX gets sysclk/2=125MHz via CLK_HSTX_DIV
    // This matches the configuration used by all other emulators in this project
    set_sys_clock_khz(250000, true);
    *((uint32_t *)(0x40010000+0x58)) = 2 << 16; //CLK_HSTX_DIV = 2 << 16; // HSTX clock/2
#endif

/*
    volatile uint32_t *qmi_m0_timing=(uint32_t *)0x400d000c;
    vreg_disable_voltage_limit();
    vreg_set_voltage(VREG_VOLTAGE_1_40);
    sleep_ms(10);
    *qmi_m0_timing = 0x60007204;
    set_sys_clock_khz(120000, false);
    *qmi_m0_timing = 0x60007303;
*/

     emu_init();
   



    char * filename;
#ifdef FILEBROWSER
    while (true) {      
        if (menuActive()) {
            uint16_t bClick = emu_DebounceLocalKeys();
            int action = handleMenu(bClick);
            filename = menuSelection();   
            if (action == ACTION_RUN) {
              break;    
            }
            tft.waitSync();
        }
    }
#endif
    emu_start();
    emu_Init(filename);
    tft.startRefresh();
    struct repeating_timer timer;
    add_repeating_timer_ms(25, repeating_timer_callback, NULL, &timer);
    while (true) {
        uint16_t bClick = emu_DebounceLocalKeys();
        emu_Input(bClick);  
        emu_Step();               
    }
}

static unsigned short palette16[PALETTE_SIZE];
void emu_SetPaletteEntry(unsigned char r, unsigned char g, unsigned char b, int index)
{
    if (index<PALETTE_SIZE) {
        palette16[index]  = RGBVAL16(r,g,b);        
    }
}

void emu_DrawLinePal16(unsigned char * VBuf, int width, int height, int line) 
{
    if (skip == 0) {
         tft.writeLinePal(width,height,line, VBuf, palette16);
    }
}

void emu_DrawLine16(unsigned short * VBuf, int width, int height, int line)
{
    if (skip == 0) {
        tft.writeLine(width,height,line, VBuf);
    }
}

int emu_IsVga(void)
{
    return (tft.getMode() == MODE_VGA_320x240?1:0);
}

void emu_DrawVsync(void)
{
    skip += 1;
    skip &= VID_FRAME_SKIP;
#ifdef HAS_USBPIO
#else
#ifdef USE_VGA
    tft.waitSync();            
#else                      
    volatile bool vb=vbl;
    while (vbl==vb) {};
#endif
#endif    
}

/*
void emu_DrawLine8(unsigned char * VBuf, int width, int height, int line) 
{
    if (skip == 0) {
#ifdef USE_VGA                        
      tft.writeLine(width,height,line, VBuf);      
#endif      
    }
} 

void emu_DrawLine16(unsigned short * VBuf, int width, int height, int line) 
{
    if (skip == 0) {
#ifdef USE_VGA        
        tft.writeLine16(width,height,line, VBuf);
#else
        tft.writeLine(width,height,line, VBuf);
#endif        
    }
}  

void emu_DrawScreen(unsigned char * VBuf, int width, int height, int stride) 
{
    if (skip == 0) {
#ifdef USE_VGA                
        tft.writeScreen(width,height-TFT_VBUFFER_YCROP,stride, VBuf+(TFT_VBUFFER_YCROP/2)*stride, palette8);
#else
        tft.writeScreen(width,height-TFT_VBUFFER_YCROP,stride, VBuf+(TFT_VBUFFER_YCROP/2)*stride, palette16);
#endif
    }
}

int emu_FrameSkip(void)
{
    return skip;
}

void * emu_LineBuffer(int line)
{
    return (void*)tft.getLineBuffer(line);    
}
*/


#ifdef HAS_SND

// Simple PCM ring buffer to feed I2S from fMSX Sound.c (WriteAudio/GetFreeAudio)
extern "C" {
    // Declarations expected by Sound.c
    unsigned int GetFreeAudio(void);
    unsigned int WriteAudio(short *buf, unsigned int n);
}

#define AUDIO_BUFFER_LEN  (256) // frames pulled per callback
#define PCM_RING_SIZE     (8192) // samples in ring buffer (tune as needed)

static short pcm_ring[PCM_RING_SIZE];
static volatile unsigned int pcm_w = 0; // write index
static volatile unsigned int pcm_r = 0; // read index

static inline unsigned int pcm_count(void) {
    unsigned int w = pcm_w, r = pcm_r;
    return (w >= r) ? (w - r) : (PCM_RING_SIZE - (r - w));
}

static inline unsigned int pcm_free(void) {
    return PCM_RING_SIZE - 1 - pcm_count();
}

extern "C" unsigned int GetFreeAudio(void) {
    return pcm_free();
}

extern "C" unsigned int WriteAudio(short *buf, unsigned int n) {
    unsigned int free = pcm_free();
    if (n > free) n = free;
    for (unsigned int i = 0; i < n; ++i) {
        pcm_ring[pcm_w] = buf[i];
        pcm_w = (pcm_w + 1) % PCM_RING_SIZE;
    }
    return n;
}

static void msx_audio_fill(short *stream, int len) {
    // len is number of samples requested
    for (int i = 0; i < len; ++i) {
        if (pcm_count() == 0) {
            stream[i] = 0; // underrun: output silence
        } else {
            stream[i] = pcm_ring[pcm_r];
            pcm_r = (pcm_r + 1) % PCM_RING_SIZE;
        }
    }
}

void emu_sndInit() {
    tft.begin_audio(AUDIO_BUFFER_LEN*2, msx_audio_fill);
}

void emu_sndPlaySound(int, int, int) { /* Unused by fMSX path */ }
void emu_sndPlayBuzz(int, int) { /* Unused by fMSX path */ }

#endif



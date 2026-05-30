/*
  RP2350 flash driver
*/

#ifndef _FLASH_T_H_
#define _FLASH_T_H_


#include "pico.h"
#include "pico/stdlib.h"
#include "platform_config.h"

#define FLASH_SECTOR_SIZE       4096

// Flash storage top address (end of usable flash for ROM storage).
// Pico2 has 4MB flash, Waveshare RP2350 PiZero has 16MB flash.
#ifdef BOARD_WAVESHARE
#define HW_FLASH_STORAGE_TOP   (0x1000000) // Waveshare = 16MB flash
#else
#define HW_FLASH_STORAGE_TOP   (0x400000)  // Pico2 = 4MB flash
#endif

#ifdef FLASH_STORE_START
#define HW_FLASH_STORAGE_BASE  (FLASH_STORE_START)
#else
#define HW_FLASH_STORAGE_BASE  (0x200000) // Start at 2MB default
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern unsigned char * flash_start;
extern unsigned char * flash_end;
extern int flash_load(const char * filename);
extern int flash_load_bswap(const char * filename);
extern int flash_verify(unsigned char * buf,int size);

#ifdef __cplusplus
}
#endif


#endif


#ifndef _PROGRAM_CONFIG_H_
#define _PROGRAM_CONFIG_H_

#define FLASH_STORE_START 0x100000  
#define KEYBOARD_ACTIVATED 1

// Default MSX model to boot:
// 0 = MSX1, 1 = MSX2, 2 = MSX2+
#ifndef DEFAULT_MSX_VERSION
#define DEFAULT_MSX_VERSION 0
#endif

#endif

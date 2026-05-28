# Pico2MSX (PicoMSX for Raspberry Pi Pico 2 (RP2350) Platforms)

Adaptation of MCUME-MSX for Waveshare rp2350 pizero

Description:

* MSX: https://minibots.wordpress.com/2025/10/26/emulador-de-msx-con-adafruit-fruit-jam/
* MSX2: https://minibots.wordpress.com/2025/11/01/emulador-de-msx2-con-olimex-rp2350pc/
* MSX2+: https://minibots.wordpress.com/2025/11/05/emulador-de-msx2-con-olimex-rp2350pc-2/

More retrocomputing projects here:
https://minibots.wordpress.com/retroinformatica/

## Features

* **MSX-1** emulation (fMSX core)
* **HDMI output**: 640×480@60Hz via HSTX
* **USB keyboard**: USB Host via PIO USB (Adafruit Feather RP2350)
* **HDMI Audio Support** 
* **Storage**: FAT32 microSD for ROMs and disk images
* **PSRAM**: External SPI memory support

## Recommended Hardware

### Supported Boards

This project officially supports two boards:

#### 1. Waveshare rp2350 pizero

## Software Requirements

* **Pico SDK** (included in this project)
* **CMake** >= 3.12
* **Ninja** or Make
* **arm-none-eabi-gcc** (ARM compiler)

## Building

### Option 1: Build Scripts (Recommended)

Quick builds by board and MSX model.

* MSX1: `./build_waveshare.sh` 
Useful options:

* Force model via define: `DEFAULT_MSX_VERSION={0|1|2}`

### Option 2: Manual Build

```bash
mkdir build && cd build

# For WaveShare:
cmake -G Ninja -DBOARD_TYPE=WAVESHARE -DPICO_EXTRAS_PATH=../pico-extras ..

ninja
```

### 2. Configure with CMake

```bash
cmake -G Ninja ..
```

### 3. Build

```bash
ninja
```

The resulting file will be located at:
`build/picomsx.uf2`

### 4. Flash to Pico 2

1. Hold the BOOTSEL button while connecting the Pico 2 via USB
2. It will appear as a storage device named `RPI-RP2`
3. Copy the `picomsx.uf2` file to the drive
4. The Pico 2 will automatically reboot into the emulator

## Configuration

### platform_config.h File

Location:
`config/platform_config.h`

Important options:

* `#define HAS_USBHOST`: Enables USB keyboard support
* `#define HAS_USBPIO`: Uses PIO USB instead of the native USB port

### System Clock

The emulator uses **132 MHz** when PIO USB is enabled:

* Allows USB operation (multiple of 48 MHz: 132 = 48 × 2.75)
* HDMI pixel clock of 26.4 MHz (HSTX divider /5)
* Compatible with **HDMI monitors and capture devices**

Without PIO USB: 250 MHz for better video performance.

### Keyboard Layout

Default: **US**

To change it, create a `kbd.cfg` file in the SD card root with:

```txt
keyboard=us
```

Supported layouts:
* `us`: United States of America
* `uk`: United Kingdom (default)
* `be`: Belgian

## Usage

### MSX ROMs

Place the MSX ROMs in the root of the microSD card:

* `MSX.ROM` (MSX-1 BIOS)
* `MSX2.ROM` (optional, MSX-2 BIOS)
* `.ROM` game/program files

### Disks

`.DSK` files in MSX disk format.

### Auto-Boot to BASIC

If there are no files on the SD card or the browser is disabled, the emulator boots directly into MSX BASIC.

### Special Keys

#### Modifiers

* **Left/Right Shift**: Both supported
* **Left/Right Control**: Both supported
* **Caps Lock**: Toggle (persistent)

#### Function Keys

* **F1-F5**: MSX function keys
* **Arrow Keys**: Navigation
* **ESC**: Escape
* **TAB**: Tab
* **Backspace**: Delete (mapped to MSX DEL)

#### Symbols (US Layout)

Number row with Shift:

* Shift+1 = !
* Shift+2 = "
* Shift+3 = £
* Shift+4 = $
* Shift+5 = %
* Shift+6 = ^
* Shift+7 = &
* Shift+8 = *
* Shift+9 = (
* Shift+0 = )

### BASIC Function Bar

The bottom MSX BASIC function bar changes dynamically:

* **Without Shift**: "color auto goto list run"
* **With Shift**: "color cload cont list run"

## Known Limitations

* The **Fn** key on keyboards cannot be mapped (it does not send standard HID codes)
* The symbol layout is optimized for **UK** keyboards; other layouts may require adjustments
* A **microSD card** is required for ROMs (no built-in ROMs included)

## File Structure

```text
picomsx_pico2_standalone/
├── CMakeLists.txt          # Build configuration
├── pico_sdk_import.cmake   # SDK import
├── tusb_config.h           # TinyUSB configuration
├── ffconf.h                # FatFS configuration
├── README.md               # This file
├── pico-sdk/               # Raspberry Pi Pico SDK
├── pico-extras/            # Additional libraries (I2S audio)
├── pico-pio-usb/           # USB host via PIO
├── picomsx/                # MSX emulator core (fMSX)
│   ├── picomsx.cpp         # Initialization and main loop
│   ├── fmsx.c              # fMSX core
│   ├── Z80.c               # Z80 CPU
│   ├── V9938.c             # MSX VDP
│   ├── AY8910.c            # Sound chip
│   ├── YM2413.c            # MSX-MUSIC FM
│   └── CAMBIOS.md          # Change documentation
├── config/                 # Hardware configuration
│   ├── platform_config.h   # Build flags
│   └── iopins.h            # Pin mapping
├── display/                # Display layer
│   ├── emuapi.cpp          # Emulator API
│   ├── hdmi_framebuffer.cpp # HDMI/HSTX framebuffer
│   └── pico_dsp.cpp        # Audio processing
├── usb_kbd/                # USB keyboard driver
│   └── hid_app.c           # HID processing
├── fatfs/                  # FAT filesystem
├── sd_driver/              # SD card driver
├── psram/                  # SPI PSRAM driver
└── flash/                  # Internal flash access
```

## Credits

* **fMSX**: Marat Fayzullin (MSX emulator core)
* **MCUME**: Jean-Marc Harvengt (original Pico port)
* **Pico-PIO-USB**: sekigon-gonnoc (PIO USB host)
* **RP2350 Port**: Adaptation for Pico 2 with HDMI and USB host

## License

This project combines multiple components with different licenses:

* fMSX: fMSX License (see picomsx/)
* Pico SDK: BSD License
* TinyUSB: MIT License
* Pico-PIO-USB: MIT License

See the LICENSE files in each subdirectory for more details.

## Changelog

See `picomsx/CAMBIOS.md` for a detailed list of all modifications.

## Version

**v0.2** (May 28, 2026)

* Support for gamepads including PS3 controllers and 2-player mode.
* SD card clock was set too high causing file loading failures
* Performance/speed issues
* Boot issues caused by I2C
* Keyboard arrow key issues
* BASIC screen red color issue
* HDMI audio not working
* PIO-USB detection issues
* Stuttering when changing keyboard directions
* Changed keyboard layout to US and fixed non-working keys

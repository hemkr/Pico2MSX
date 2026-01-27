# Pico2MSX (PicoMSX para plataformas Raspberry Pi Pico 2 (RP2350))

Adaptación del MCUME-MSX para Olimex RP2350pc y Adafruit Fruit Jam

Descripción en:
- MSX: [https://minibots.wordpress.com/2025/10/26/emulador-de-msx-con-adafruit-fruit-jam/]
- MSX2: [https://minibots.wordpress.com/2025/11/01/emulador-de-msx2-con-olimex-rp2350pc/]
- MSX2+: [https://minibots.wordpress.com/2025/11/05/emulador-de-msx2-con-olimex-rp2350pc-2/]

Note: I am working on polishing and commenting the code.

Más sobre retroinformática aquí: [https://minibots.wordpress.com/retroinformatica/]

## Características

- **MSX-1** emulado (fMSX core)
- **Salida HDMI**: 640×480@60Hz mediante HSTX
- **Teclado USB**: Host USB vía PIO USB (Adafruit Feather RP2350)
- **Audio I2S**: DAC externo PCM5102
- **Almacenamiento**: MicroSD con FAT32 para ROMs y discos
- **PSRAM**: Soporte para memoria externa SPI

## Hardware recomendado

### Placas soportadas

Este proyecto soporta dos placas oficialmente:

#### 1. Adafruit Feather RP2350 (Fruit Jam)
- **HDMI**: CKP=GPIO13, D0P=GPIO15, D1P=GPIO17, D2P=GPIO19
- **Audio PWM**: GPIO 21
- **USB host (PIO USB)**: D+=GPIO1, D-=GPIO2, VBUS=GPIO11
- **PSRAM**: GPIO 0, 8, 9, 10 (SPI0)
- **MicroSD**: Conexión SPI estándar

#### 2. Olimex RP2350-PC
- **HDMI**: CKP=GPIO15, D0P=GPIO12, D1P=GPIO16, D2P=GPIO18
- **Audio PWM**: GPIO 26 (PWM_AUDIO-L)
- **Audio I2S** (opcional): BCLK=GPIO23, DATA=GPIO29, LRCLK=GPIO30, MCLK=GPIO31
- **USB host (PIO USB)**: D+=GPIO1, D-=GPIO2, VBUS: sin control por GPIO (ver nota)
- **PSRAM**: GPIO 4, 5, 6, 7 (SPI0 alt.)
- **MicroSD**: Conexión SPI estándar

## Requisitos de software

- **Pico SDK** (incluido en este proyecto)
- **CMake** >= 3.12
- **Ninja** o Make
- **arm-none-eabi-gcc** (compilador ARM)

## Compilación

### Opción 1: Scripts de compilación (recomendado)

Compilaciones rápidas por placa y modelo MSX.

Variantes Fruit Jam (Adafruit Feather RP2350):
- MSX1: `./build_msx_fruitjam.sh` → genera `build/picomsx_fruitjam_msx1.uf2`
- MSX2: `./build_msx2_fruitjam.sh` → genera `build/picomsx_fruitjam_msx2.uf2`
- MSX2+: usar el script general con flag: `./build_fruitjam.sh --msx2p` → `build/picomsx_fruitjam_msx2p.uf2`

Variantes Olimex RP2350-PC:
- MSX1: `./build_msx_olimex.sh` → genera `build/picomsx_olimexpc_msx1.uf2`
- MSX2: `./build_msx2_olimex.sh` → genera `build/picomsx_olimexpc_msx2.uf2`
- MSX2+: usar el script general con flag: `./build_olimexpc.sh --msx2p` → `build/picomsx_olimexpc_msx2p.uf2`

Opciones útiles:
- Forzar modelo vía define: `DEFAULT_MSX_VERSION={0|1|2}`
- Probar inversión D+/D- (Olimex): `./build_olimexpc.sh --swap-dpdm`

### Opción 2: Compilación manual

```bash
mkdir build && cd build

# Para Fruit Jam:
cmake -G Ninja -DBOARD_TYPE=FRUITJAM -DPICO_EXTRAS_PATH=../pico-extras ..

# Para Olimex:
cmake -G Ninja -DBOARD_TYPE=OLIMEXPC -DPICO_EXTRAS_PATH=../pico-extras ..

ninja
```

### 2. Configurar con CMake

```bash
cmake -G Ninja ..
```

### 3. Compilar

```bash
ninja
```

El archivo resultante estará en: `build/picomsx.uf2`

### 4. Flashear en el Pico 2

1. Mantén pulsado el botón BOOTSEL al conectar el Pico 2 por USB
2. Aparecerá como unidad de almacenamiento `RPI-RP2`
3. Copia el archivo `picomsx.uf2` a la unidad
4. El Pico 2 se reiniciará automáticamente con el emulador

## Configuración

### Archivo platform_config.h

Ubicación: `config/platform_config.h`

Opciones importantes:
- `#define HAS_USBHOST`: Habilita teclado USB
- `#define HAS_USBPIO`: Usa PIO USB en lugar del puerto USB nativo

### Clock del sistema

El emulador usa **132 MHz** cuando PIO USB está activo:
- Permite USB (múltiplo de 48 MHz: 132 = 48 × 2.75)
- HDMI pixel clock de 26.4 MHz (divisor HSTX /5)
- Compatible con **monitores y capturadoras HDMI**

Sin PIO USB: 250 MHz para mejor rendimiento de vídeo.

### Layout de teclado

Por defecto: **UK** (británico)

Para cambiarlo, crea un archivo `kbd.cfg` en la raíz de la SD con:
```
keyboard=uk
```

Layouts soportados:
- `uk`: Reino Unido (por defecto)
- `be`: Belga

## Uso

### ROMs MSX

Coloca las ROMs MSX en la raíz de la microSD:
- `MSX.ROM` (BIOS MSX-1)
- `MSX2.ROM` (opcional, BIOS MSX-2)
- Archivos `.ROM` de juegos/programas

### Discos

Archivos `.DSK` en formato MSX

### Auto-boot a BASIC

Si no hay archivos en la SD o navegador deshabilitado, el emulador arranca directamente a MSX BASIC.

### Teclas especiales

#### Modificadores
- **Shift izquierdo/derecho**: Ambos funcionan
- **Control izquierdo/derecho**: Ambos funcionan
- **Caps Lock**: Toggle (persistente)

#### Función
- **F1-F5**: Teclas de función MSX
- **Flechas**: Navegación
- **ESC**: Escape
- **TAB**: Tabulador
- **Backspace**: Borrar (mapeado a DEL MSX)

#### Símbolos (UK layout)
Fila numérica con Shift:
- Shift+1 = !
- Shift+2 = "
- Shift+3 = £
- Shift+4 = $
- Shift+5 = %
- Shift+6 = ^
- Shift+7 = &
- Shift+8 = *
- Shift+9 = (
- Shift+0 = )

### Barra de funciones BASIC

La barra inferior del MSX BASIC cambia dinámicamente:
- **Sin Shift**: "color auto goto list run"
- **Con Shift**: "color cload cont list run"

## Limitaciones conocidas

- La tecla **Fn** de los teclados no se puede mapear (no envía código HID estándar)
- El layout de símbolos está optimizado para **UK**; otros layouts pueden necesitar ajustes
- Requiere **tarjeta microSD** para ROMs (no hay ROMs internas)

## Resolución de problemas

### No aparece señal HDMI

- Verifica que uses un **monitor o TV compatible con 640×480@60Hz**
- Algunas capturadoras HDMI antiguas pueden no funcionar (prueba con 144 MHz si es el caso)
- Comprueba las conexiones HSTX

### El teclado USB no funciona

- Verifica las conexiones GPIO1 (D+) y GPIO2 (D-)
- Asegúrate de que `HAS_USBPIO` está definido en `platform_config.h`
- Prueba con otro teclado USB (algunos pueden ser incompatibles)
- En la placa **Olimex RP2350-PC** no hay un switch de 5V controlado por GPIO para VBUS. Debes alimentar el puerto USB del dispositivo con 5V mediante un **hub USB alimentado** o un **puente de 5V a VBUS**. En el arranque, el firmware mostrará: "USB host VBUS: no GPIO control defined. Ensure 5V is provided externally."

Si el teclado no es detectado en Olimex, prueba también a compilar con inversión de líneas D+/D-:

- `./build_olimexpc.sh --swap-dpdm` → genera un UF2 con el mapeo DP/DM invertido para depuración.

Si el host no arranca, conecta un hub alimentado y observa por UART mensajes como:

- `USB Host (PIO) setup: DP=GP1, DM=GP2`
- `Init USB (PIO HCD)... ok`
- `Keyboard found` al enchufar el teclado

### Audio sin sonido

- Verifica las conexiones I2S (GPIO24, GPIO26-27)
- Comprueba que el DAC PCM5102 está correctamente alimentado
- El audio requiere DAC externo (no hay salida de audio por HDMI)

## Estructura de archivos

```
picomsx_pico2_standalone/
├── CMakeLists.txt          # Configuración de compilación
├── pico_sdk_import.cmake   # Importación del SDK
├── tusb_config.h           # Configuración TinyUSB
├── ffconf.h                # Configuración FatFS
├── README.md               # Este archivo
├── pico-sdk/               # Raspberry Pi Pico SDK
├── pico-extras/            # Librerías adicionales (audio I2S)
├── pico-pio-usb/           # USB host vía PIO
├── picomsx/                # Core del emulador MSX (fMSX)
│   ├── picomsx.cpp         # Inicialización y loop principal
│   ├── fmsx.c              # Núcleo fMSX
│   ├── Z80.c               # CPU Z80
│   ├── V9938.c             # VDP MSX
│   ├── AY8910.c            # Chip de sonido
│   ├── YM2413.c            # FM MSX-MUSIC
│   └── CAMBIOS.md          # Documentación de cambios
├── config/                 # Configuración de hardware
│   ├── platform_config.h   # Flags de compilación
│   └── iopins.h            # Mapeo de pines
├── display/                # Capa de visualización
│   ├── emuapi.cpp          # API del emulador
│   ├── hdmi_framebuffer.cpp # Framebuffer HDMI/HSTX
│   └── pico_dsp.cpp        # Procesado de audio
├── usb_kbd/                # Driver de teclado USB
│   └── hid_app.c           # Procesado HID
├── fatfs/                  # Sistema de archivos FAT
├── sd_driver/              # Driver SD card
├── psram/                  # Driver PSRAM SPI
└── flash/                  # Acceso a flash interna
```

## Créditos

- **fMSX**: Marat Fayzullin (core del emulador MSX)
- **MCUME**: Jean-Marc Harvengt (port original a Pico)
- **Pico-PIO-USB**: sekigon-gonnoc (USB host vía PIO)
- **Port RP2350**: Adaptación para Pico 2 con HDMI y USB host

## Licencia

Este proyecto combina múltiples componentes con diferentes licencias:
- fMSX: Licencia fMSX (ver picomsx/)
- Pico SDK: Licencia BSD
- TinyUSB: Licencia MIT
- Pico-PIO-USB: Licencia MIT

Consulta los archivos LICENSE en cada subdirectorio para más detalles.

## Changelog

Ver `picomsx/CAMBIOS.md` para el listado detallado de todas las modificaciones realizadas.

## Versión

**v1.0** (26 de octubre de 2025)
- Primera versión estable con HDMI y USB host
- Clock: 132 MHz
- Compatible con monitores y capturadoras HDMI

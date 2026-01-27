# FruitJam (Adafruit Feather RP2350): HDMI, USB Host (PIO-USB) y UART0 en PicoMSX

Este documento explica, con detalle orientado a entrenamiento de IA, cómo este software configura y utiliza HDMI (HSTX/DVI), el USB Host (mediante PIO-USB) y la UART0 en la plataforma FruitJam (Adafruit Feather RP2350).

Se incluyen:
- Mapeos de pines exactos (hardware).
- Rutas de código y funciones relevantes (software).
- Flags de compilación y enlaces CMake implicados.
- Flujo de inicialización y llamadas en tiempo de ejecución.
- Logs esperados y problemas típicos.

Referencias de archivo clave:
- `config/board_fruitjam.h` — Mapeo de pines de FruitJam.
- `config/platform_config.h` — Flags de plataforma (HAS_USBHOST, HAS_USBPIO, layout teclado, etc.).
- `CMakeLists.txt` — Selección de fuentes y librerías según BOARD_TYPE.
- `display/emuapi.cpp` — Inicialización de sistema, USB Host y almacenamiento SD; enrutado de teclado.
- `display/hdmi_framebuffer.cpp`, `display/pico_dsp.cpp` — Salida de vídeo HSTX (HDMI/DVI) y modos.
- `usb_kbd/hid_app.c`, `usb_kbd/kbd.h` — Traducción HID teclado y envío de eventos.

---

## 1) Selección de placa y flags de plataforma

- La placa se selecciona en CMake con `-DBOARD_TYPE=FRUITJAM`.
- CMake activa el backend USB correspondiente y define constantes de compilación.
- En `config/platform_config.h`:
  - `#define HAS_USBHOST 1` habilita el soporte de host USB.
  - Condicional a FruitJam: `#define HAS_USBPIO 1` — se usa TinyUSB con controlador PIO (no el USB nativo del RP2350B).
  - `#define KEYLAYOUT KLAYOUT_ES` fija el layout de teclado a Español por defecto.

Fragmento relevante:
- `CMakeLists.txt`
  - Incluye `usb_kbd/hid_app.c` y el driver `hcd_pio_usb.c` de TinyUSB para FruitJam.
  - Enlaza `pico_pio_usb` sólo cuando `BOARD_TYPE=FRUITJAM`.
  - Define `PIO_USB_DP_PIN_DEFAULT=2` (véase notas de pines de USB más abajo).
  - UART para debug (stdio UART habilitado, USB stdio deshabilitado):
    - `pico_enable_stdio_usb(picomsx FALSE)`
    - `pico_enable_stdio_uart(picomsx TRUE)`
    - Define pines de UART por compile defs (TX=GP44, RX=GP45, ver §3).

---

## 2) HDMI (HSTX / DVI) en FruitJam

Hardware de pines (definido en `config/board_fruitjam.h`):
- CK pair P: `PIN_CKP = GPIO13`
- D0 pair P: `PIN_D0P = GPIO15`
- D1 pair P: `PIN_D1P = GPIO17`
- D2 pair P: `PIN_D2P = GPIO19`

Notas:
- FruitJam usa los pares P (lado impar) estándar del Pico 2 para la señalización TMDS (HSTX).
- El audio PWM está en `GPIO21` (`AUDIO_PIN 21`), no forma parte del TMDS pero es relevante para sincronización/recursos.

Software (módulos):
- `display/hdmi_framebuffer.cpp` y `display/pico_dsp.cpp` implementan la salida de vídeo usando el bloque HSTX del RP2350.
- La abstracción de pantalla está en `display/pico_dsp.h/.cpp` vía el objeto `PICO_DSP tft;`.
- La selección de modo se hace en `display/emuapi.cpp` mediante `tft.begin(...)`.
  - Con `USE_VGA 1` (activado en `platform_config.h`), el proyecto usa el camino de salida “VGA” del driver, que internamente está adaptado para HSTX/DVI-HDMI en esta plataforma.
  - En FruitJam, típicamente se llama a `tft.begin(MODE_VGA_320x240)` durante `emu_init()` según el estado y preferencias de arranque.

Flujo simplificado:
1. `emu_init()` decide el modo de vídeo y llama `tft.begin(...)` (VGA 320×240 por defecto en esta build).
2. `pico_dsp` configura el HSTX y los pares TMDS conforme a los pines del board.
3. `hdmi_framebuffer` prepara el frame buffer y la temporización.

Edge cases:
- Existe un `#define HDMI_PINOUT_METRO` comentado en `platform_config.h` para variantes de pinout HDMI (ej. Metro RP2350). FruitJam usa el pinout por defecto, por lo que no se define.

---

## 3) UART0 (debug stdio) en FruitJam

Decisiones de diseño:
- Para mantener libre GPIO0/1 (usados por USB PIO-USB), la UART0 se enruta a pines alternativos.
- En `CMakeLists.txt` (bloque FruitJam) se definen:
  - `PICO_DEFAULT_UART=0`
  - `PICO_DEFAULT_UART_TX_PIN=44`
  - `PICO_DEFAULT_UART_RX_PIN=45`
- Se desactiva stdio por USB y se activa stdio por UART:
  - `pico_enable_stdio_usb(picomsx FALSE)`
  - `pico_enable_stdio_uart(picomsx TRUE)`
- Velocidad por defecto: 115200 bps (por `stdio_init_all()` del SDK salvo override explícito). En ramas con USB nativo (Olimex) se fuerza un init temprano de UART para logging; en FruitJam no es necesario el init manual porque se usa PIO-USB, pero los printf salen por UART0 igualmente.

Conexión física típica para debug:
- TX (placa) → RX (adaptador) en GPIO44
- RX (placa) ← TX (adaptador) en GPIO45
- GND común

Ejemplo de logs al arrancar (`display/emuapi.cpp`):
```
=== picomsx starting ===
Board: Adafruit Feather RP2350 (Fruit Jam)
USB Host (PIO) setup: DP=GP1, DM=GP2
Enabling USB host VBUS power on GP11
Init USB (PIO HCD)... ok
USB D+/D- on GP1 and GP2
TinyUSB Host HID Controller Example
[USB] Polling for 2 seconds to detect connected devices...
[USB] Initial enumeration window complete
SD initialized, files found: N
```

---

## 4) USB Host en FruitJam (PIO-USB con TinyUSB)

Hardware de pines (`config/board_fruitjam.h`):
- `PIN_USB_HOST_DP = GPIO1`
- `PIN_USB_HOST_DM = GPIO2`
- `PIN_USB_HOST_VBUS = GPIO11` (control del switch de 5V para el puerto host)

Notas sobre pines y defines:
- CMake define `PIO_USB_DP_PIN_DEFAULT=2`. Este define interno es consumido por el stack PIO-USB; sin embargo, el código en `emuapi.cpp` configura explícitamente el pinout en tiempo de ejecución a partir de `PIN_USB_HOST_DP/DM`, por lo que prevalece la configuración dinámica.
- Requisito de hardware: VBUS a 5V para el conector host. En FruitJam se controla por GPIO11; en placas sin control por GPIO se requiere un hub alimentado.

Software (módulos y flujo):
- Backend TinyUSB host con controlador PIO (`raspberrypi/pio_usb/hcd_pio_usb.c`).
- En CMake (cuando `BOARD_TYPE=FRUITJAM`):
  - Se añade el driver PIO-USB a las fuentes.
  - Se enlaza la librería `pico_pio_usb` tras crear el target.
- Inicialización en `display/emuapi.cpp::emu_init()` (ruta sin `USE_USB_OLIMEXPC`):
  1. Construye `pio_usb_configuration_t pio_cfg` con el pinout DP/DM real:
     - Detecta automáticamente si el orden es DPDM o DMDP: `PIO_USB_PINOUT_DPDM` vs `PIO_USB_PINOUT_DMDP`.
     - Asigna `pio_cfg.pin_dp = PIN_USB_HOST_DP`.
  2. Si existe `PIN_USB_HOST_VBUS`, activa el switch de 5V: `gpio_put(PIN_USB_HOST_VBUS, 1)`.
  3. Configura el HCD de TinyUSB para PIO: `tuh_configure(..., TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg)`.
  4. Llama `tuh_init(BOARD_TUH_RHPORT)` para inicializar el host.
  5. Da una ventana de enumeración inicial de 2s con `tuh_task()` en un bucle (100 iteraciones × 20ms).
- Procesamiento HID teclado:
  - `usb_kbd/hid_app.c` traduce reportes HID a códigos del emulador: tablas `conv_table_es` (ES por defecto), `conv_table_uk/be`.
  - Los eventos se envían a `kbd_signal_raw_key(...)` en `display/emuapi.cpp`, que decide si las teclas van al menú (navegación/selección) o al emulador (BASIC). Las F1–F12 se envían siempre al emulador fuera del menú.

Ejecución periódica (polling):
- Durante la ventana de arranque se pulsa `tuh_task()` para forzar enumeración.
- En ejecución normal, los callbacks de TinyUSB (interrupciones/EP IN) alimentan `tuh_hid_report_received_cb` y `hid_app_task()`; si se porta el bucle principal fuera del framework actual, garantizar llamadas regulares a `tuh_task()`.

Mensajes de diagnóstico (ejemplos):
- "USB Host (PIO) setup: DP=GP1, DM=GP2"
- "Enabling USB host VBUS power on GP11"
- "Init USB (PIO HCD)... ok"
- "TinyUSB Host HID Controller Example"
- "[USB] Polling for 2 seconds to detect connected devices..."

Opcional: invertir D+/D-
- CMake expone la opción `-DUSB_DPDM_INVERT=ON` que define `USB_DPDM_INVERT=1` si el cableado físico estuviese invertido.

---

## 5) Resumen de pines (FruitJam)

Desde `config/board_fruitjam.h`:

- HDMI (HSTX/TMDS):
  - CKP = GP13, D0P = GP15, D1P = GP17, D2P = GP19
- USB Host (PIO-USB):
  - D+ = GP1, D- = GP2, VBUS = GP11
- UART0 (stdio):
  - TX = GP44, RX = GP45 (definidos vía CMake)
- Audio PWM:
  - AUDIO_PIN = GP21
- SD (SPI0):
  - SCLK = GP34, MOSI = GP35, MISO = GP36, CS = GP39, DETECT = GP33

---

## 6) Calidad, errores típicos y troubleshooting

- USB host no enumera:
  - Verificar VBUS: en FruitJam debe estar activo (GPIO11 a nivel alto). Usar hub alimentado si no hay 5V.
  - Mantener la ventana de enumeración inicial; si se modifica el arranque, asegurar llamadas regulares a `tuh_task()`.
  - Layout de pines DP/DM: usar `USB_DPDM_INVERT=ON` si el arnés invierte las líneas.
- UART sin salida:
  - Confirmar conexión a 115200 8N1, TX→RX, RX→TX y masa común.
  - Asegurar que no se activó stdio USB (está desactivado por CMake en FruitJam).
- Vídeo HDMI sin señal:
  - Confirmar conexiones a los pares P (GPIO13/15/17/19) y modo seleccionado por `tft.begin(...)`.
  - Evitar conflictos de DMA: HDMI, audio y PSRAM usan múltiples canales (ver defines en `board_fruitjam.h`).

---

## 7) Extractos de código clave

- CMake (enlace y defines relevantes):
  - Añade `hcd_pio_usb.c` y enlaza `pico_pio_usb` sólo en FruitJam.
  - `target_compile_definitions(picomsx PRIVATE PIO_USB_DP_PIN_DEFAULT=2)`
  - UART stdio: `pico_enable_stdio_usb(FALSE)`, `pico_enable_stdio_uart(TRUE)` y pines 44/45.
- Inicialización USB (PIO-USB): `display/emuapi.cpp::emu_init()`
  - Configura `pio_usb_configuration_t`, enciende VBUS, `tuh_configure`, `tuh_init`, bucle `tuh_task()` por 2s.
- Teclado HID:
  - `usb_kbd/hid_app.c::process_kbd_report` + `conv_table_es` → `kbd_signal_raw_key(...)` (en `display/emuapi.cpp`).
- Vídeo:
  - `display/hdmi_framebuffer.cpp`, `display/pico_dsp.cpp` y llamadas `tft.begin(...)` desde `emu_init()`.

---

## 8) Contrato funcional (resumen)

- Entrada:
  - Teclado USB HID conectado a puerto host (DP=GP1, DM=GP2, VBUS=GP11).
  - Configuración de build con `-DBOARD_TYPE=FRUITJAM`.
- Salida:
  - Señal de vídeo por HSTX (HDMI/DVI) en CKP/D0P/D1P/D2P.
  - Logs por UART0 en GP44/45 a 115200 bps.
- Éxito:
  - Enumeración USB durante la ventana inicial y respuesta a teclas (F1–F12 hacia emulador fuera del menú).
  - Imagen estable 320×240.
  - Logs visibles por UART desde el arranque.
- Errores manejados:
  - Falta de VBUS → mensaje con recomendación de hub alimentado.
  - SD no lista → reintentos de `f_mount` y mensaje de fallo.

---

## 9) Cómo reproducir (build rápido)

- Compilar para FruitJam (MSX2 por defecto, ejemplo):
  - Script: `./build_msx2_fruitjam.sh`
  - Genera `build/picomsx_fruitjam.uf2` (o `picomsx.uf2`).
- Flashear por BOOTSEL y conectar UART a 115200 para ver logs.

---

Última actualización: 2 de noviembre de 2025

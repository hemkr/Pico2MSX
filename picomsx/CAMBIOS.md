# CAMBIOS respecto a la versión original (RP2350 Pico 2)

Fecha: 2025-10-26

Este documento resume las modificaciones realizadas para que picomsx funcione en RP2350 (Pico 2) con salida HDMI (HSTX) y teclado USB vía host PIO USB.

## Vídeo (HDMI / HSTX)
- Ajuste de reloj del sistema y del HSTX para que aparezca señal HDMI:
  - HDMI "solo": sysclk a 250 MHz y divisor de HSTX /2 configurado ANTES de inicializar HDMI.
  - Con USB PIO activo: sysclk a **132 MHz** (48 MHz × 2.75) con divisor HSTX **/5** → **pixel clock 26.4 MHz**
    - Esta configuración es compatible tanto con monitores como con **capturadoras HDMI**
    - El pixel clock de 26.4 MHz está suficientemente cerca del estándar VGA 25.175 MHz
- Resultado: vídeo estable 640×480@60Hz con HSTX, compatible con monitores y capturadoras.

Archivos implicados:
- Inicialización de plataforma (configuración de reloj y HSTX) en el arranque del target picomsx.

## Auto‑boot a MSX BASIC
- Si no hay ficheros en la SD (o no hay navegador), forzamos auto‑arranque a MSX BASIC.

Archivo implicado:
- `display/emuapi.cpp` (detección de ficheros; flag `autorun`).

## USB Host (PIO USB) y teclado
- Integración de Pico‑PIO‑USB para usar host USB en pines externos (Feather RP2350: D+ GP1 / D− GP2, VBUS GP11):
  - Añadido el subdirectorio/librería pico‑pio‑usb al build.
  - Inclusión de `lib/tinyusb/src/portable/raspberrypi/pio_usb/hcd_pio_usb.c` para el backend HCD PIO.
  - Enlace con `pico_pio_usb` (no `tinyusb_pico_pio_usb`).
  - Ajuste condicional de reloj a 120 MHz cuando PIO USB está activo.
- Configuración de TinyUSB host en `tusb_config.h` acorde al uso de host + PIO USB.

Archivos implicados:
- `CMakeLists.txt` (inclusiones, fuentes y librerías).
- `tusb_config.h` (config de host y PIO USB).
- `display/emuapi.cpp` (inicialización del host; VBUS; selección de layout por fichero `kbd.cfg`).

## Entrada de teclado: mapeo y comportamiento
- Capa HID (USB) mejorada en `usb_kbd/hid_app.c`:
  - Detección de ambos Shift y ambos Control; construcción de un byte de flags.
  - Envío de eventos también cuando SOLO cambian los modificadores (p. ej., mantener Shift sin pulsar otra tecla). Esto permite que el BASIC cambie la barra inferior dinámicamente.
- Puente hacia el núcleo MSX en `display/emuapi.cpp` y `picomsx/fmsx.c`:
  - Acepta códigos no ASCII (flechas, F1–F5, ESC, Backspace, etc.).
  - Backspace USB (código interno 0x14) convertido a 0x7F (MSX).
  - Flechas mapeadas a la matriz MSX (fila/bit correctos).
  - F1–F5, TAB y ESC añadidos.
  - Propagación y aplicación de modificadores cada frame:
    - SHIFT y CTRL activan sus bits en la matriz (`KeyMap[6]`).
    - CAPS LOCK implementado como toggle (persistente) que actúa como SHIFT.
    - Importante: ahora la barra inferior cambia a “color cload cont list run” mientras se mantiene Shift.
- Tabla de símbolos/puntuación y números con Shift:
  - Correcciones sucesivas para UK layout (por defecto).
  - Fila numérica con Shift: símbolos ajustados; corrección final de Shift+9 → '(' y Shift+0 → ')'.

Archivos implicados:
- `usb_kbd/hid_app.c` (procesado del reporte HID; flags; eventos de modificadores).
- `usb_kbd/kbd.h` (constantes de flags/códigos especiales).
- `display/emuapi.cpp` (ruta de entrada y joystick por teclado opcional).
- `picomsx/fmsx.c` (KeyMap, mapeos especiales, CAPS toggle, aplicación de modificadores por frame, traducción de backspace y flechas, F1–F5, TAB, ESC, signos).

## Layout de teclado
- `uk` por defecto; se puede seleccionar en `kbd.cfg` (línea `keyboard=uk` o `keyboard=be`).
- Nota: la tecla Fn de muchos teclados no envía código HID (no mapeable).

## Otros
- Se añadieron y documentaron utilidades de auto‑run (archivo `autorun.txt`) para ejecuciones automáticas.
- Se mantuvieron advertencias del compilador no críticas (funciones no usadas en el core fMSX original) para minimizar cambios invasivos.

## Resumen de efectos visibles
- HDMI operativo con HSTX y Feather RP2350.
- Teclado USB (host PIO USB) funcional con flechas, Backspace, TAB, ESC, F1–F5, Shift/Control y CAPS.
- Barra inferior del BASIC cambia mientras se mantiene Shift.
- Fila numérica con símbolos correctos (incluido Shift+9/0 → '()').

## Limitaciones conocidas
- La tecla Fn no se puede mapear (no emite usage HID propio en la mayoría de teclados).
- El layout de símbolos está optimizado para UK; otros layouts pueden requerir ajustes puntuales.

## Pistas de mantenimiento
- Si se añaden nuevos layouts, extender `conv_table_*` y permitir su selección en `kbd.cfg`.
- Para nuevos mapeos de teclas especiales, añadir casos en `picomsx/fmsx.c` (sección de `hk >= 1000`).
- Si se cambia el reloj del sistema por necesidades de audio u otras, revisar la condición que fija 132 MHz con PIO USB activo.
- **Importante para HDMI**: El pixel clock de 26.4 MHz (132 MHz / 5) es crítico para compatibilidad con capturadoras. Valores probados:
  - 120 MHz: funciona en monitores, NO en capturadoras
  - 132 MHz: funciona en monitores Y capturadoras ✓
  - 144 MHz: puede funcionar (no probado completamente)

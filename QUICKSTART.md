# PicoMSX Pico 2 - Inicio Rápido

# PicoMSX Pico 2 - Inicio Rápido

## Compilación según placa y modelo

Atajos recomendados por placa/modelo:

### Adafruit Feather RP2350 (Fruit Jam)
- MSX1: `./build_msx_fruitjam.sh` → `build/picomsx_fruitjam_msx1.uf2`
- MSX2: `./build_msx2_fruitjam.sh` → `build/picomsx_fruitjam_msx2.uf2`
- MSX2+: `./build_fruitjam.sh --msx2p` → `build/picomsx_fruitjam_msx2p.uf2`

### Olimex RP2350-PC
- MSX1: `./build_msx_olimex.sh` → `build/picomsx_olimexpc_msx1.uf2`
- MSX2: `./build_msx2_olimex.sh` → `build/picomsx_olimexpc_msx2.uf2`
- MSX2+: `./build_olimexpc.sh --msx2p` → `build/picomsx_olimexpc_msx2p.uf2`

Notas:
- También puedes forzar el modelo con `DEFAULT_MSX_VERSION={0|1|2}`.
- Para depurar USB en Olimex, existe `--swap-dpdm` para invertir D+/D- en el firmware.

## Flashear en tu placa

1. Mantén pulsado **BOOTSEL** mientras conectas la placa por USB
2. Aparecerá como unidad `RPI-RP2`
3. Copia el archivo `.uf2` correspondiente a tu placa
4. La placa se reiniciará automáticamente

## Conexiones hardware

### Común a ambas placas

#### USB Keyboard (PIO USB)
- **D+ (Data Plus)**: GPIO 1
- **D- (Data Minus)**: GPIO 2
- **VBUS**:
   - Fruit Jam: GPIO 11 (controlado por firmware)
   - Olimex RP2350-PC: sin control por GPIO → aportar 5V externo (hub alimentado o jumper)
   - En el arranque el firmware imprime el estado de USB (placa, DP/DM, VBUS) por UART

#### HDMI
- HSTX integrado en RP2350
- Pixel clock: 26.4 MHz (132 MHz sysclk / 5)
- Resolución: 640×480@60Hz

#### MicroSD
- Conexión SPI estándar
- Formato: FAT32
- Coloca ROMs MSX (.ROM) y discos (.DSK) en la raíz

### Diferencias específicas

#### Adafruit Feather RP2350
- **HDMI pins**: CKP=GPIO13, D0P=GPIO15
- **Audio PWM**: GPIO 21

#### Olimex RP2350-PC
- **HDMI pins**: CKP=GPIO15, D0P=GPIO12, D1P=GPIO16, D2P=GPIO18
- **Audio PWM**: GPIO 26
- **Audio I2S** opcional: BCLK=GPIO23, DATA=GPIO29, LRCLK=GPIO30, MCLK=GPIO31
 - **VBUS USB host**: requiere 5V externo (no conmutado por GPIO)

## Primera ejecución

1. Si no hay archivos en la SD, arranca directamente a **MSX BASIC**
2. Con teclado USB conectado, ya puedes usar el emulador
3. La barra inferior cambia con **Shift** entre:
   - Sin Shift: `color auto goto list run`
   - Con Shift: `color cload cont list run`

## Teclas especiales

- **F1-F5**: Funciones MSX
- **Flechas**: Navegación
- **ESC**: Escape
- **TAB**: Tabulador
- **Backspace**: Borrar
- **Caps Lock**: Toggle (persistente)

## Símbolos UK Layout

| Tecla | Sin Shift | Con Shift |
|-------|-----------|-----------|
| 1     | 1         | !         |
| 2     | 2         | "         |
| 3     | 3         | £         |
| 4     | 4         | $         |
| 5     | 5         | %         |
| 6     | 6         | ^         |
| 7     | 7         | &         |
| 8     | 8         | *         |
| 9     | 9         | (         |
| 0     | 0         | )         |

## Resolución HDMI

- **640×480 @ 60Hz**
- Compatible con monitores y capturadoras HDMI
- Pixel clock: 26.4 MHz (132 MHz sysclk, divisor HSTX /5)

## Problemas comunes

### No compila
- Verifica que tienes instalado: `cmake`, `ninja`, `arm-none-eabi-gcc`
- Asegúrate de tener Pico SDK disponible

### No aparece señal HDMI
- Verifica que el monitor soporte 640×480@60Hz
- Comprueba las conexiones HSTX

### Teclado USB no funciona
- Verifica conexiones GPIO1 (D+), GPIO2 (D-)
- Prueba con otro teclado USB
- Comprueba que `HAS_USBPIO` está habilitado en `config/platform_config.h`
- En Olimex RP2350-PC, usa un **hub USB alimentado** o alimenta VBUS con 5V.
- Si persiste, prueba firmware con D+/D- invertidos: `./build_olimexpc.sh --swap-dpdm` y flashea el UF2 resultante.
- Observa el log UART al inicio: debe mostrar inicialización TinyUSB y, al conectar, "Keyboard found".

## Más información

Consulta `README.md` para documentación completa y `picomsx/CAMBIOS.md` 
para detalles técnicos de todas las modificaciones realizadas.

## Versión

**v1.0** - 26 de octubre de 2025
- Primera versión estable
- HDMI 640×480@60Hz
- USB host PIO completo
- Clock: 132 MHz

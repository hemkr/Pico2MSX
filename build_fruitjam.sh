#!/bin/bash
# Script de compilación para Adafruit Feather RP2350 (Fruit Jam)

set -e

BOARD="FRUITJAM"
BOARD_NAME="Adafruit Feather RP2350 (Fruit Jam)"

# Parámetros
#  --msx1 / --msx2 / --msx2p    -> fija DEFAULT_MSX_VERSION (0/1/2)
#  DEFAULT_MSX_VERSION=<0|1|2>  -> alternativa vía variable de entorno o CLI
MSX_VERSION_ARG=""
OUT_SUFFIX="fruitjam"

for arg in "$@"; do
    case "$arg" in
        --msx1)
            MSX_VERSION_ARG="-DDEFAULT_MSX_VERSION=0";
            OUT_SUFFIX="fruitjam_msx1";
            shift;;
        --msx2)
            MSX_VERSION_ARG="-DDEFAULT_MSX_VERSION=1";
            OUT_SUFFIX="fruitjam_msx2";
            shift;;
        --msx2p|--msx2plus)
            MSX_VERSION_ARG="-DDEFAULT_MSX_VERSION=2";
            OUT_SUFFIX="fruitjam_msx2p";
            shift;;
        DEFAULT_MSX_VERSION=*)
            # Permite pasar DEFAULT_MSX_VERSION=1 directamente
            VAL="${arg#*=}"
            if [[ "$VAL" =~ ^[0-2]$ ]]; then
                MSX_VERSION_ARG="-DDEFAULT_MSX_VERSION=${VAL}"
                case "$VAL" in
                    0) OUT_SUFFIX="fruitjam_msx1";;
                    1) OUT_SUFFIX="fruitjam_msx2";;
                    2) OUT_SUFFIX="fruitjam_msx2p";;
                esac
            else
                echo "Valor DEFAULT_MSX_VERSION inválido: $VAL (use 0,1,2)"
                exit 2
            fi
            shift;;
    esac
done

echo "=========================================="
echo "  PicoMSX para $BOARD_NAME"
echo "=========================================="
echo "Modelo por defecto: ${MSX_VERSION_ARG:-(por defecto en código)}"
echo ""

# Crear directorio build si no existe
if [ ! -d "build" ]; then
        echo "Creando directorio build..."
        mkdir build
fi

cd build

# Obtener ruta absoluta del proyecto
PROJECT_DIR="$(cd .. && pwd)"

# Configurar con CMake
echo "Configurando proyecto con CMake para $BOARD_NAME..."
cmake -G Ninja \
    -DBOARD_TYPE=FRUITJAM \
    -DPICO_EXTRAS_PATH="${PROJECT_DIR}/pico-extras" \
    ${MSX_VERSION_ARG} \
    ..

echo "Compilando..."
ninja

if [ $? -eq 0 ]; then
        echo ""
        echo "=========================================="
        echo "  ✓ Compilación exitosa"
        echo "=========================================="
        echo ""
        echo "Archivo generado: build/picomsx.uf2"
        ls -lh picomsx.uf2
    
        # Copiar a archivo específico de placa/modelo
        cp picomsx.uf2 "picomsx_${OUT_SUFFIX}.uf2"
        echo "Copia guardada: build/picomsx_${OUT_SUFFIX}.uf2"
        echo ""
        echo "Placa objetivo: $BOARD_NAME"
        echo "Modelo por defecto: ${MSX_VERSION_ARG:-(por defecto en código)}"
        echo ""
        echo "Para flashear:"
        echo "  1. Mantén BOOTSEL y conecta el Pico 2"
        echo "  2. Copia picomsx.uf2 a la unidad RPI-RP2"
        echo ""
else
        echo ""
        echo "=========================================="
        echo "  ✗ Error en la compilación"
        echo "=========================================="
        exit 1
fi

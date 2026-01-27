#!/bin/bash
# Script de compilación rápida para PicoMSX

set -e

echo "=========================================="
echo "  PicoMSX para Raspberry Pi Pico 2"
echo "=========================================="
echo ""

# Crear directorio build si no existe
if [ ! -d "build" ]; then
    echo "Creando directorio build..."
    mkdir build
fi

cd build

# Configurar con CMake si no está configurado
if [ ! -f "build.ninja" ] && [ ! -f "Makefile" ]; then
    echo "Configurando proyecto con CMake..."
    cmake -G Ninja ..
fi

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

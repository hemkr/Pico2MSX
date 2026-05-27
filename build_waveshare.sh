#!/usr/bin/env bash
set -euo pipefail

# Build script for Waveshare RP2350 HDMI board (MSX default)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

printf "==========================================\n"
printf "  PicoMSX para Waveshare RP2350 (HDMI)\n"
printf "==========================================\n"
printf "Modelo por defecto: -DDEFAULT_MSX_VERSION=0\n\n"

cmake -B "$BUILD_DIR" -S "$SCRIPT_DIR" \
  -DBOARD_TYPE=WAVESHARE \
  -DDEFAULT_MSX_VERSION=1

printf "Compilando...\n"
cmake --build "$BUILD_DIR" -j

printf "\n==========================================\n"
printf "  \xE2\x9C\x93 Compilación exitosa\n"
printf "==========================================\n\n"

if [ -f "$BUILD_DIR/picomsx.uf2" ]; then
  ls -lh "$BUILD_DIR/picomsx.uf2"
  cp -f "$BUILD_DIR/picomsx.uf2" "$BUILD_DIR/picomsx_waveshare_msx1.uf2"
  printf "Copia guardada: build/picomsx_waveshare_msx1.uf2\n\n"
fi

printf "Placa objetivo: Waveshare RP2350 HDMI (PIO-USB)\n"
printf "Modelo por defecto: -DDEFAULT_MSX_VERSION=0\n\n"
printf "Para flashear:\n"
printf "  1. Mantén BOOTSEL y conecta el Pico 2\n"
printf "  2. Copia picomsx.uf2 a la unidad RPI-RP2\n"}

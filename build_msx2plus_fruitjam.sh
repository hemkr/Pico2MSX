#!/bin/bash
# Wrapper de compilación para MSX2+ en Adafruit Feather RP2350 (Fruit Jam)
# Delegamos en build_fruitjam.sh fijando el modelo por defecto a MSX2+

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

"${SCRIPT_DIR}/build_fruitjam.sh" --msx2plus "$@"

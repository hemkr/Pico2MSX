#!/bin/bash
# Wrapper de compilación para MSX2+ en Olimex RP2350-PC
# Delegamos en build_olimexpc.sh fijando el modelo por defecto a MSX2+

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

"${SCRIPT_DIR}/build_olimexpc.sh" --msx2plus "$@"

#!/bin/bash
# Atajo: Compila Olimex RP2350-PC con MSX1 por defecto
# Puedes añadir --swap-dpdm para invertir D+/D-
set -e
DIR="$(cd "$(dirname "$0")" && pwd)"
exec bash -lc "cd '$DIR' && ./build_olimexpc.sh --msx1 $*"
#!/bin/bash
# Build wrapper: Olimex RP2350-PC with MSX1 as default model
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
"${SCRIPT_DIR}/build_olimexpc.sh" --msx1 "$@"

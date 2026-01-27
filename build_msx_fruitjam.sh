#!/bin/bash
# Atajo: Compila FruitJam con MSX1 por defecto
set -e
DIR="$(cd "$(dirname "$0")" && pwd)"
exec bash -lc "cd '$DIR' && ./build_fruitjam.sh --msx1 $*"
#!/bin/bash
# Build wrapper: FruitJam with MSX1 as default model
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
"${SCRIPT_DIR}/build_fruitjam.sh" --msx1 "$@"

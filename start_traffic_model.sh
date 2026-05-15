#!/usr/bin/env bash
set -euo pipefail

INPUT_FILE="${1:-examples/equal_10_10.txt}"
OUTPUT_CSV="${2:-examples/results/equal/output.csv}"
MODEL_NAME="${3:-equal}"
SEED="${4:-67}"
OUTPUT_DIR="${5:-examples/results/equal/plots}"

./cpp/build/trafficmodel "${INPUT_FILE}" "${OUTPUT_CSV}" "${SEED}"
python3 python/analyze_traffic.py "${OUTPUT_CSV}" --model "${MODEL_NAME}" --out-dir "${OUTPUT_DIR}"

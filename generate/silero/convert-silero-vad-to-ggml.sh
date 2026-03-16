#!/bin/bash

# generate/silero/convert-silero-vad-to-ggml.sh
# Anastasia Shchupak, 16 March 2026
#
# Shell script that installs an original Silero VAD v6.2.0 model and converts it to GGML binary format,
# using the python script convert-silero-vad-to-ggml.py.
#
# Usage: convert-silero-vad-to-ggml.sh <silero-vad-version>
# Example: convert-silero-vad-to-ggml.sh v6.2.0

set -e  # stop if any command fails

# check if a version is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <silero-vad-version>"
  echo "Example: $0 v6.2.0"
  exit 1
fi

VERSION="$1"

echo "Creating virtual environment..."
python3 -m venv venv

echo "Activating virtual environment..."
source venv/bin/activate

echo "Installing silero-vad and dependencies..."
pip install "silero-vad==$VERSION" numpy

echo "Converting Silero VAD model..."
python3 convert-silero-vad-to-ggml.py --output ggml-silero.bin

echo "Deactivating virtual environment..."
deactivate

echo "Done. Silero VAD $VERSION is installed and converted to GGML binary format."
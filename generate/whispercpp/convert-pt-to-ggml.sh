#!/bin/bash

# generate/whispercpp/convert-pt-to-ggml.sh
# Anastasia Shchupak, 16 March 2026
#
# Shell script that clones OpenAI Whisper repository, downloads an original OpenAI Whisper PyTorch model,
# and converts it to GGML binary format, using the Python script convert-pt-to-ggml.py.
#
# Usage: convert-pt-to-ggml.sh <whisper-model-name>
# Example: convert-pt-to-ggml.sh tiny

set -e  # stop if any command fails

# OpenAI Whisper models available for conversion
models="tiny tiny.en base base.en small small.en medium medium.en large-v1 large-v2 large-v3 large-v3-turbo"

list_models() {
    printf "\nAvailable models:\n "
    for model in $models; do
        printf " %s" "$model"
    done
    printf "\n\n"
    printf ".en = english-only\n\n"
}

# check if a model name is provided
if [ -z "$1" ]; then
    printf "Usage: %s <whisper-model-name>\n" "$0"
    printf "Example: %s tiny\n" "$0"
    list_models
    exit 1
fi

MODEL_NAME="$1"

# validate model name
if ! echo "$models" | tr ' ' '\n' | grep -qx "$MODEL_NAME"; then
    printf "Invalid model: %s\n" "$MODEL_NAME"
    list_models
    exit 1
fi

echo "Creating virtual environment..."
python3 -m venv venv

echo "Activating virtual environment..."
source venv/bin/activate

if [ -d "whisper" ]; then
  echo "OpenAI whisper repository already cloned, skipping."
else
  echo "Cloning OpenAI whisper repository..."
  git clone https://github.com/openai/whisper
fi

echo "Installing openai-whisper and dependencies..."
pip install openai-whisper

echo "Downloading original PyTorch model..."
python3 -c "import whisper; whisper.load_model('${MODEL_NAME}', download_root='./')"

echo "Converting Whisper model to GGML format..."
python3 convert-pt-to-ggml.py ./"${MODEL_NAME}".pt ./whisper/ ./

echo "Deactivating virtual environment..."
deactivate

mv ggml-model.bin ggml-"${MODEL_NAME}".bin

echo "Done. Whisper model '${MODEL_NAME}' is downloaded and converted to GGML binary format."
generate/whispercpp/README.TXT
Anastasia Shchupak, 16 March 2026

This folder documents how Whisper models in GGML format are obtained for use with Praat's speech recognition.
Unlike the Silero VAD model, Whisper models are not embedded in Praat. They are downloaded separately by the user and
stored in the Praat preferences folder.


Files in this folder
--------------------

README.txt
    This file.

convert-pt-to-ggml.py
    Python script from whisper.cpp that converts original OpenAI Whisper PyTorch models (.pt files) to GGML binary format.
    Included here to document how GGML models can be produced from the original OpenAI models. Requires Python 3
    with the openai-whisper package, and a clone of the OpenAI Whisper repository (for tokenizer and mel filters).

convert-pt-to-ggml.sh
    Shell script that converts an OpenAI Whisper PyTorch model (.pt file) to GGML binary format,
    using the Python script convert-pt-to-ggml.py. If the .pt file is not already present in the current folder,
    the script downloads the original model from OpenAI. If a .pt file with the matching name is already present
    (e.g. a fine-tuned version), it is used as-is. The script also clones the OpenAI Whisper repository
    if not already present, as it is needed by the conversion script for tokenizer and mel filters.

download-ggml-model.sh
    Shell script from whisper.cpp that downloads pre-converted GGML models from HuggingFace
    (https://huggingface.co/ggerganov/whisper.cpp). An alternative to running the Python conversion.


How to obtain Whisper models in GGML format
-------------------------------------------

Option A. Download pre-converted model:

    ./download-ggml-model.sh <whisper-model-name>

    Run the script without arguments to see the full list of available models, which includes quantized variants
    not available via Option B.

Option B. Convert from PyTorch model:

    ./convert-pt-to-ggml.sh <whisper-model-name>

    Available models include: tiny, tiny.en, base, base.en, small, small.en, medium, medium.en,
    large-v1, large-v2, large-v3, large-v3-turbo. Run the script without arguments to see the full list.
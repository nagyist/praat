generate/silero/README.TXT
Anastasia Shchupak, 16 March 2026

This folder documents how the file external/whispercpp/ggml-silero-vad-model-data.h was generated. This C header
contains the embedded Silero VAD (Voice Activity Detection) v6.2.0 model weights used by Praat's speech recognition.


Files in this folder
--------------------

README.txt
    This file.

ggml-silero-v6.2.0.bin
    The Silero VAD model (version 6.2.0) in GGML binary format. This is the file from which the C header was generated.

GENERATE.sh
    Shell script that converts included ggml-silero-v6.2.0.bin to the C header
    ggml-silero-vad-model-data.h, which is to be put to the external/whispercpp folder.

convert-silero-vad-to-ggml.py
    Slightly adapted (to match the output file name) Python script from whisper.cpp that converts
    the original Silero VAD PyTorch model to GGML binary format. Included here to document one of the ways
    to obtain ggml-silero-v6.2.0.bin. Requires Python 3 with the silero-vad and numpy packages.

convert-silero-vad-to-ggml.sh
    Shell script that installs an original Silero VAD v6.2.0 model and converts it to GGML binary format,
    using the python script convert-silero-vad-to-ggml.py.

download-vad-model.sh
    Shell script from whisper.cpp that downloads pre-converted GGML models from HuggingFace
    (https://huggingface.co/ggml-org/whisper-vad). An alternative to running the Python conversion.


1. How to obtain ggml-silero-vad-model-data.h from the included .bin file
-------------------------------------------------------------------------

Run the script GENERATE.sh. This produces ggml-silero-vad-model-data.h in the current folder,
which then needs to be copied to the external/whispercpp folder.


2. How to obtain the .bin file
------------------------------

If you need to recreate ggml-silero-v6.2.0.bin from scratch, then you have two options.

Option A. Download pre-converted model:

    ./download-vad-model.sh silero-v6.2.0

Option B. Convert from PyTorch model:

    ./convert-silero-vad-to-ggml.sh v6.2.0



#!/bin/bash
# Generates C header ggml-silero-vad-model-data.h
# from the Silero VAD model in GGML binary format:

xxd -i -n ggml_silero_bin ggml-silero-v6.2.0.bin > ggml-silero-vad-model-data.h

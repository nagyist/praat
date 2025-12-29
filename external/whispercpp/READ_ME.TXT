To make GGML and Whisper-cpp compatible with Praat,
we add the following to the top of `ggml.h`:

#define WHISPER_VERSION  "1.8.2"
#define GGML_VERSION  "0.9.4"
#define GGML_COMMIT  "unknown"
#define GGML_USE_CPU
#define GGML_CPU_GENERIC
#if ! defined (_GNU_SOURCE)
	#define _GNU_SOURCE
#endif

This works because `ggml.h` is included, directly or indirectly,
at the very top of all `ggml` and `whisper` files (last checked 20251227).

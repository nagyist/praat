praat/external/whispercpp/READ_ME.TXT
Anastasia Shchupak, 12 March 2026

This file describes the adaptations to the Whisper.cpp current sources
that are needed to make them compatible with Praat.
Last maintenance release of Whisper.cpp as of 12 March 2026 was v1.8.3 (Jan 15, 2025).
But source code in this edition was taken from 5 March 2026, commit hash is 30c5194c9691e4e9a98b3dea9f19727397d3f46e.


1. Selecting files and flattening the file structure.
-----------------------------------------------------

The Whisper.cpp sources are distributed over multiple folders, which are in different deep
branches of the whisper.cpp root folder. We use only a subset of files and flatten this hierarchy as shown below.

cp whisper.cpp/src/*.c          praat/external/whispercpp/
cp whisper.cpp/src/*.cpp        praat/external/whispercpp/
cp whisper.cpp/src/*.h          praat/external/whispercpp/

cp whisper.cpp/ggml/src/*.c     praat/external/whispercpp/
cp whisper.cpp/ggml/src/*.cpp   praat/external/whispercpp/
cp whisper.cpp/ggml/src/*.h     praat/external/whispercpp/

cp whisper.cpp/ggml/src/ggml-cpu/*.c        praat/external/whispercpp/
cp whisper.cpp/ggml/src/ggml-cpu/*.cpp      praat/external/whispercpp/
cp whisper.cpp/ggml/src/ggml-cpu/*.h        praat/external/whispercpp/
cp whisper.cpp/ggml/src/ggml-cpu/amx/amx.h  praat/external/whispercpp/

cp whisper.cpp/include/*.h      praat/external/whispercpp/
cp whisper.cpp/ggml/include/*.h praat/external/whispercpp/

cd praat/external/whispercpp/
mv ggml.cpp         ggml-cpp.cpp
mv ggml-cpu.cpp     ggml-cpu-cpp.cpp


2. File modifications.
----------------------
2.1. ggml.h
-----------
To make GGML and Whisper-cpp compatible with Praat, we add the following to the top of `ggml.h`:

#define WHISPER_VERSION  "1.8.3"
#define GGML_VERSION  "0.9.7"
#define GGML_COMMIT  "unknown"
#define GGML_USE_CPU
#define GGML_CPU_GENERIC
#if ! defined (_GNU_SOURCE)
	#define _GNU_SOURCE
#endif

This works because `ggml.h` is included, directly or indirectly,
at the very top of all `ggml` and `whisper` files (last checked 20260312), except:
ggml-common.h, ggml-backend-dl.cpp, ggml-backend-dl.h, unary-ops.cpp, and unary-ops.h.

2.2. whisper.h and whisper.cpp
--------------
2.2.1.
------
To use silero model for speech detection from memory rather than from the external binary file,
we add the following two variables to the `struct whisper_full_params` in whisper.h (in the section for VAD):
```
        const void * vad_model_data;		      // Pointer to in-memory model data
        size_t       vad_model_data_size;         // Size of in-memory model data
```
And we initialize them in whisper.cpp in whisper_full_default_params() (also in the section for VAD):
```
		/*.vad_model_data		       =*/ nullptr,
		/*.vad_model_data_size		   =*/ 0,
```

2.2.2.
------
Then we declare a function which is responsible for loading the silero model from internal memory.
We place it among other WHISPER_API in the section "Voice Activity Detection (VAD)"
right after function whisper_vad_init_from_file_with_params() in whisper.h:

	WHISPER_API struct whisper_vad_context * whisper_vad_init_from_memory_with_params(const void * data, size_t size, struct whisper_vad_context_params params);

And we define it in whisper.cpp:
```
struct whisper_vad_context * whisper_vad_init_from_memory_with_params (
		const void * data, size_t size,
		whisper_vad_context_params params) {
	WHISPER_LOG_INFO("%s: loading VAD model from memory\n", __func__);
	struct SileroVadStream {
		const void * data;
		size_t size;
		size_t pos;
	};
	SileroVadStream stream {
		data,
		size,
		0
	};
	whisper_model_loader loader {};
	loader.context = &stream;

	loader.read = [](void * ctx, void * output, size_t read_size) -> size_t {
		auto * s = (SileroVadStream *)ctx;
		size_t available = s->size - s->pos;
		size_t to_read = std::min(read_size, available);
		memcpy(output, (const unsigned char *)s->data + s->pos, to_read);
		s->pos += to_read;
		return to_read;
	};
	loader.eof = [](void * ctx) -> bool {
		auto * s = (SileroVadStream *)ctx;
		return s->pos >= s->size;
	};
	loader.close = [](void * ctx) { };
	return whisper_vad_init_with_params(&loader, params);
}
```

2.2.3.
------
In the same section "Voice Activity Detection (VAD)" in whisper.h, we also declare 5 functions which are meant to
provide an interface for accessing information about VAD segments.
```
	WHISPER_API int whisper_full_n_vad_segments(struct whisper_context * ctx);
	WHISPER_API int64_t whisper_full_get_vad_segment_orig_start(struct whisper_context * ctx, int i_vad_segment);
	WHISPER_API int64_t whisper_full_get_vad_segment_orig_end(struct whisper_context * ctx, int i_vad_segment);
	WHISPER_API int64_t whisper_full_get_vad_segment_vad_start(struct whisper_context * ctx, int i_vad_segment);
	WHISPER_API int64_t whisper_full_get_vad_segment_vad_end(struct whisper_context * ctx, int i_vad_segment);
```
And we define these functions in whisper.cpp:
```
int whisper_full_n_vad_segments(struct whisper_context * ctx) {
	if (!ctx->state->has_vad_segments) {
		return 0;
	}
	return static_cast<int>(ctx->state->vad_segments.size());
}

int64_t whisper_full_get_vad_segment_orig_start(struct whisper_context * ctx, int i_vad_segment) {
	return ctx->state->vad_segments[i_vad_segment].orig_start;
}

int64_t whisper_full_get_vad_segment_orig_end(struct whisper_context * ctx, int i_vad_segment) {
	return ctx->state->vad_segments[i_vad_segment].orig_end;
}

int64_t whisper_full_get_vad_segment_vad_start(struct whisper_context * ctx, int i_vad_segment) {
	return ctx->state->vad_segments[i_vad_segment].vad_start;
}

int64_t whisper_full_get_vad_segment_vad_end(struct whisper_context * ctx, int i_vad_segment) {
	return ctx->state->vad_segments[i_vad_segment].vad_end;
}
```

2.2.4.
------
In whisper.cpp, function whisper_vad() is modified to include the reading from memory. This line:
```
		struct whisper_vad_context * vctx = whisper_vad_init_from_file_with_params(params.vad_model_path, vad_ctx_params);
```
is changed to these lines:
```
		struct whisper_vad_context * vctx = nullptr;
		if (params.vad_model_data && params.vad_model_data_size) {
			vctx = whisper_vad_init_from_memory_with_params((void*)params.vad_model_data, params.vad_model_data_size, vad_ctx_params);
		} else {
			vctx = whisper_vad_init_from_file_with_params(params.vad_model_path, vad_ctx_params);
		}
```


3. Bringing Silero-VAD model to Praat source code.
--------------------------------------------------
First, we downloaded the ggml Silero model from the original whisper.cpp repository:
```
whisper.cpp/models/download-vad-model.sh silero-v6.2.0
```
The result is `ggml-silero-v6.2.0.bin`, which is a C-compatible binary file, which can be loaded by whisper.cpp.

We then converted this binary to a C header using `xxd` and copied it to external/whispercpp directory:
```
xxd -i whisper.cpp/models/ggml-silero-v6.2.0.bin > praat/external/whispercpp/ggml-silero-vad-model-data.h
```

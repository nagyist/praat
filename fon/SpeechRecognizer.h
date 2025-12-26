#ifndef _SpeechRecognizer_h_
#define _SpeechRecognizer_h_

#include "Data.h"
#include "Sound.h"

struct whisper_context;
struct whisper_full_params;

struct autoWhisperContext {
	whisper_context *ptr;

	autoWhisperContext (whisper_context *p = nullptr) : ptr(p) {}
	~autoWhisperContext ();

	autoWhisperContext (const autoWhisperContext&) = delete;
	autoWhisperContext& operator= (const autoWhisperContext&) = delete;

	autoWhisperContext (autoWhisperContext&& other) noexcept : ptr(other.ptr) {
		other.ptr = nullptr;
	}
	autoWhisperContext& operator= (autoWhisperContext&& other) noexcept;

	[[nodiscard]]
	whisper_context * get () const {
		return ptr;
	}
};

#include "SpeechRecognizer_def.h"

// singletons to access lists of models and languages from everywhere
constSTRVEC theSpeechRecognizerModelNames ();
constSTRVEC theSpeechRecognizerLanguageNames ();

// default model parameters to access from everywhere (except TextGridArea_prefs.h)
inline conststring32 theSpeechRecognizerDefaultModelName = U"ggml-base.bin";
inline conststring32 theSpeechRecognizerDefaultLanguageName = U"Autodetect language";

// class SpeechRecognizer functions
autoSpeechRecognizer SpeechRecognizer_create (conststring32 modelName, conststring32 languageName);
autostring32 SpeechRecognizer_recognize (SpeechRecognizer me, constSound sound);

#endif
/* End of file SpeechRecognizer.h */
#ifndef _SpeechRecognizer_h_
#define _SpeechRecognizer_h_
/* SpeechRecognizer.h
 *
 * Copyright (C) 2025 Anastasia Shchupak
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

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

/* End of file SpeechRecognizer.h */
#endif

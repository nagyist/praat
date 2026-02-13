/* SpeechRecognizer.cpp
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

#include "SpeechRecognizer.h"
#include "Sound.h"
#include "whisper.h"
#include "melder.h"

#include "oo_DESTROY.h"
#include "SpeechRecognizer_def.h"
#include "oo_COPY.h"
#include "SpeechRecognizer_def.h"
#include "oo_EQUAL.h"
#include "SpeechRecognizer_def.h"
#include "oo_CAN_WRITE_AS_ENCODING.h"
#include "SpeechRecognizer_def.h"
#include "oo_WRITE_TEXT.h"
#include "SpeechRecognizer_def.h"
#include "oo_WRITE_BINARY.h"
#include "SpeechRecognizer_def.h"
#include "oo_READ_TEXT.h"
#include "SpeechRecognizer_def.h"
#include "oo_READ_BINARY.h"
#include "SpeechRecognizer_def.h"
#include "oo_DESCRIPTION.h"
#include "SpeechRecognizer_def.h"

autoWhisperContext :: ~autoWhisperContext () {
	if (ptr)
		whisper_free (ptr);
}

autoWhisperContext& autoWhisperContext :: operator= (autoWhisperContext&& other) noexcept {
	if (this != & other) {
		if (ptr)
			whisper_free (ptr);
		ptr = other.ptr;
		other.ptr = nullptr;
	}
	return *this;
}

Thing_implement (SpeechRecognizer, Daata, 0);

void structSpeechRecognizer :: v1_info () {
	SpeechRecognizer_Parent :: v1_info ();
	MelderInfo_writeLine (U"Model: ", our d_modelName.get());
	MelderInfo_writeLine (U"Language: ", our d_languageName.get());
}

static conststring32 theWhisperModelsFolder ();
autoSpeechRecognizer SpeechRecognizer_create (conststring32 modelName, conststring32 languageName) {
	try {
		autoSpeechRecognizer me = Thing_new (SpeechRecognizer);
		my d_modelName = Melder_dup (modelName);
		my d_languageName = Melder_dup (languageName);

		/*
			Check if selected model and language are compatible.
		*/
		if (str32str (modelName, U".en.bin")) {
			Melder_require (str32str (languageName, U"Autodetect") || str32str (languageName, U"English"),
					U"Model ", modelName, U" cannot be used for ", languageName, U" transcription. "
					U"Either select a multilingual model (the model name does not include .en) "
					U"or select \"Autodetect language\"/\"English\" from the language list."
			);
		}

		/*
			Create Whisper context.
		*/
		whisper_context_params contextParams = whisper_context_default_params();
		contextParams.use_gpu = false;
		contextParams.flash_attn = false;

		conststring32 modelPath = Melder_cat (theWhisperModelsFolder(), U"/", modelName);
		conststring8 utf8ModelPath = Melder_peek32to8 (modelPath);

		whisper_context *ctx = whisper_init_from_file_with_params (utf8ModelPath, contextParams);
		if (! ctx)
			Melder_throw (U"Cannot create Whisper context from: ", modelPath, U". Model file not found?");

		my whisperContext = autoWhisperContext (ctx);

		return me;
	} catch (MelderError) {
		Melder_throw (U"SpeechRecognizer not created.");
	}
}

bool isSentenceEndingPunctuation(conststring32 word) {
	if (! word || word [0] == U'\0')
		return false;

	size_t word_length = Melder_length (word);
	char32 last_char = word [word_length - 1];
	return last_char == U'.' || last_char == U'!' || last_char == U'?' ||
		last_char == U'。' || last_char == U'！' || last_char == U'？';
}

WhisperTranscription SpeechRecognizer_recognize (SpeechRecognizer me, constSound sound) {
	try {
		//TRACE
		trace (U"Sound xmin = ", sound -> xmin, U", sound xmax = ", sound -> xmax);

		/*
			Resample the sound to 16kHz if needed.
		*/
		autoSound resampled = Sound_resample (sound, 16000.0, 50);
		sound = resampled.get();

		/*
			Convert the sound to float32 for Whisper.
		*/
		std::vector <float> samples32;
		samples32. reserve (integer_to_uinteger_a (sound -> nx));
		for (integer i = 1; i <= sound -> nx; ++ i)
			samples32. push_back (static_cast <float> (sound -> z [1] [i]));

		/*
			Set up Whisper parameters for word segmentation.
		*/
		whisper_full_params params = whisper_full_default_params (WHISPER_SAMPLING_GREEDY);
		params.max_len = 1;				// maximum length of a segment (in symbols)
		params.token_timestamps = true;	// must be true with max_len=1, otherwise segfault
		params.split_on_word = true;	// with max_len=1, it ensures that the minimal segment is a word (and not a symbol)
		if (whisper_is_multilingual (my whisperContext.get ())) {
			if (my d_languageName && ! str32str (my d_languageName.get(), U"Autodetect")) {
				autostring32 name = Melder_dup (my d_languageName.get());
				name [0] = Melder_toLowerCase (name [0]);   // e.g. "Dutch" -> "dutch"
				params. language = whisper_lang_str (whisper_lang_id (Melder_peek32to8 (name.get())));
			} else {
				params. language = "auto";
			}
		}

		/*
			Run Whisper, collect all word segments, construct sentences and the full transcription string.
		*/
		if (whisper_full (my whisperContext.get(), params, samples32.data(), static_cast <int> (sound -> nx)) != 0)
			Melder_throw (U"Whisper failed to process audio");
		const int n_words = whisper_full_n_segments (my whisperContext.get());
		autoMelderString full_text;
		autoMelderString sentence_text;
		autovector <WhisperSegment> words = newvectorzero <WhisperSegment> (0);
		autovector <WhisperSegment> sentences = newvectorzero <WhisperSegment> (0);

		double sentence_tmin = 0.0;
		bool isFirstWordInSentence = true;

		for (int i = 0; i < n_words; ++ i) {
			/*
				Extract word segment information.
			*/
			double word_tmin = whisper_full_get_segment_t0 (my whisperContext.get(), i) / 100.0;
			double word_tmax = whisper_full_get_segment_t1 (my whisperContext.get(), i) / 100.0;
			if (word_tmax == word_tmin) // skip segments of the length 0
				continue;
			autostring32 word_text = Melder_8to32 (whisper_full_get_segment_text (my whisperContext.get(), i));

			/*
				Mark sentence start.
			*/
			if (isFirstWordInSentence) {
				sentence_tmin = word_tmin;
				isFirstWordInSentence = false;
			}

			/*
				Add word to the sentence and to the full transcription text.
			*/
			MelderString_append (& sentence_text, word_text.get());
			MelderString_append (& full_text, word_text.get());

			/*
				Create and store sentence segment if sentence is complete.
			*/
			bool isLastWordInSentence = isSentenceEndingPunctuation (word_text.get());
			bool isLastWordOverall = (i == n_words - 1);
			if (isLastWordInSentence || isLastWordOverall) {
				WhisperSegment *sentence = sentences.append();
				sentence -> text = Melder_dup (sentence_text.string);
				sentence -> tmin = sentence_tmin;
				sentence -> tmax = word_tmax;
				trace (U"Sentence segment: [ ", sentence -> tmin, U" - ", sentence -> tmax, U" ] ", sentence -> text.get());

				MelderString_empty (& sentence_text);
				isFirstWordInSentence = true;
			}

			/*
				Remove sentence-ending punctuation from word if necessary.
			*/
			if (isLastWordInSentence) {
				integer length = Melder_length (word_text.get());
				Melder_assert (length > 0);
				word_text [length - 1] = U'\0';
			}

			/*
				Create and store word segment without punctuation.
			*/
			WhisperSegment *word = words.append();
			word -> text = word_text.move();
			word -> tmin = word_tmin;
			word -> tmax = word_tmax;
			trace (U"Word segment ", i, U": [ ", word -> tmin, U" - ", word -> tmax, U" ]", word -> text.get());
		}

		WhisperTranscription transcription;
		transcription.fullTranscription.text = Melder_dup (full_text.string);
		transcription.fullTranscription.tmin = sound -> xmin;
		transcription.fullTranscription.tmax = sound -> xmax;
		transcription.words = words.move();
		transcription.sentences = sentences.move();

		trace (U"Full transcription:",
			U" [ ", transcription.fullTranscription.tmin, U" - ",
			transcription.fullTranscription.tmax, U" ]", transcription.fullTranscription.text.get());

		return transcription;

	} catch (MelderError) {
		Melder_throw (U"Sound not transcribed.");
	}
}

static conststring32 theWhisperModelsFolder () {
	static autostring32 whisperModelFolderPath;
	if (! whisperModelFolderPath) {
		try {
			structMelderFolder modelsFolder { };
			MelderFolder_getSubfolder (Melder_preferencesFolder (),	U"models", & modelsFolder);
			if (! MelderFolder_exists (& modelsFolder))
				MelderFolder_create (& modelsFolder);
			structMelderFolder whispercppFolder { };
			MelderFolder_getSubfolder (& modelsFolder,	U"whispercpp", & whispercppFolder);
			if (! MelderFolder_exists (& whispercppFolder))
				MelderFolder_create (& whispercppFolder);
			whisperModelFolderPath = Melder_dup (MelderFolder_peekPath (& whispercppFolder));
			Melder_casual (whisperModelFolderPath.get());
		} catch (MelderError) {
			Melder_clearError ();
		}
	}
	return whisperModelFolderPath.get();
}

constSTRVEC theCurrentSpeechRecognizerModelNames () {
	static autoSTRVEC whisperModelNames;
	try {
		whisperModelNames = fileNames_STRVEC (Melder_cat (theWhisperModelsFolder (), U"/*.bin"));
	} catch (MelderError) {
		Melder_clearError ();
	}
	return whisperModelNames.get();
}

constSTRVEC theSpeechRecognizerLanguageNames () {
	static autoSTRVEC sortedWhisperLanguageNames;
	if (! sortedWhisperLanguageNames) {
		autoSTRVEC unsorted;
		try {
			const uint8 nLanguages = whisper_lang_max_id();
			for (uint8 i = 0; i < nLanguages; i ++) {
				autostring32 languageName = Melder_8to32 (whisper_lang_str_full (i));

				/*
					Capitalize the first letter, e.g. "dutch" -> "Dutch".
				*/
				if (languageName [0] != U'\0')
					languageName [0] = Melder_toUpperCase (languageName [0]);

				unsorted.append (languageName.get());
			}
			autoSTRVEC sorted = sort_STRVEC (unsorted.get());

			/*
				Create a list to return (with autodetect option).
			*/
			sortedWhisperLanguageNames. append (U"Autodetect language");
			for (integer i = 1; i <= sorted.size; i ++)
				sortedWhisperLanguageNames. append (sorted [i].get());

		} catch (MelderError) {
			unsorted.reset();
			sortedWhisperLanguageNames.reset();
			Melder_clearError ();
		}
	}
	return sortedWhisperLanguageNames.get();
}

/* End of file SpeechRecognizer.cpp */

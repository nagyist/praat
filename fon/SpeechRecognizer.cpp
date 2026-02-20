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
autoSpeechRecognizer SpeechRecognizer_create (conststring32 modelName, conststring32 languageName, bool useDtw) {
	try {
		autoSpeechRecognizer me = Thing_new (SpeechRecognizer);
		my d_modelName = Melder_dup (modelName);
		my d_languageName = Melder_dup (languageName);
		my d_useDtw = false;

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

		/*
			Enable DTW (Dynamic Time Warping algorithm used for more precise token boundaries).
		*/
		if (useDtw) {
			my d_useDtw = true;
			contextParams.dtw_token_timestamps = true;
			if (str32str (modelName, U"tiny.en"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_TINY_EN;
			else if (str32str (modelName, U"tiny"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_TINY;
			else if (str32str (modelName, U"base.en"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_BASE_EN;
			else if (str32str (modelName, U"base"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_BASE;
			else if (str32str (modelName, U"small.en"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_SMALL_EN;
			else if (str32str (modelName, U"small"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_SMALL;
			else if (str32str (modelName, U"medium.en"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_MEDIUM_EN;
			else if (str32str (modelName, U"medium"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_MEDIUM;
			else if (str32str (modelName, U"large-v3-turbo") || str32str (modelName, U"turbo"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_LARGE_V3_TURBO;
			else if (str32str (modelName, U"large-v3"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_LARGE_V3;
			else if (str32str (modelName, U"large-v2"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_LARGE_V2;
			else if (str32str (modelName, U"large-v1") || str32str (modelName, U"large"))
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_LARGE_V1;
			else
				contextParams.dtw_aheads_preset = WHISPER_AHEADS_N_TOP_MOST;
		}

		conststring32 modelPath = Melder_cat (theWhisperModelsFolder(), U"/", modelName);
		conststring8 utf8ModelPath = Melder_peek32to8 (modelPath);

		/*
			Initialise Whisper model.
		*/
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
			Set up Whisper parameters.
		*/
		whisper_full_params params = whisper_full_default_params (WHISPER_SAMPLING_GREEDY);
		params.token_timestamps = true;	// must be true to use t0 and t1 (non-DTW) token timestamps
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
			Run Whisper and collect all tokens using DTW (there are multiple tokens per segment) into one flat list.
		*/
		if (whisper_full (my whisperContext.get(), params, samples32.data(), static_cast <int> (sound -> nx)) != 0)
			Melder_throw (U"Whisper failed to process audio");
		const int n_segments = whisper_full_n_segments (my whisperContext.get());

		struct Token {
			autostring32 text;
			double t_dtw;   // DTW timestamp (end of token), in seconds, or -0.01 if unavailable
			double t1;    // "inference time trick" timestamp (end of token), in seconds
		};
		autovector <Token> allTokens = newvectorzero <Token> (0);

		for (int i = 0; i < n_segments; ++ i) {
			const int n_tokens = whisper_full_n_tokens (my whisperContext.get(), i);
			for (int j = 0; j < n_tokens; ++ j) {
				whisper_token_data tdata = whisper_full_get_token_data (my whisperContext.get(), i, j);
				if (tdata.id >= whisper_token_eot (my whisperContext.get()))
					continue;   // skip special tokens
				Token *token = allTokens.append();
				token -> text = Melder_8to32 (whisper_full_get_token_text (my whisperContext.get(), i, j));
				token -> t_dtw = tdata.t_dtw / 100.0;
				token -> t1 = tdata.t1 / 100.0;
				trace (U"Segment ", i, U"; token ", allTokens.size, U": text = ", token -> text.get(),
						U", t_dtw = ", token -> t_dtw, U", t1 = ", token -> t1);
			}
		}

		/*
			Build word and sentence segments from the flat token list.
		*/
		autoMelderString sentence_text;
		autoMelderString full_text;
		autovector <WhisperSegment> words = newvectorzero <WhisperSegment> (0);
		autovector <WhisperSegment> sentences = newvectorzero <WhisperSegment> (0);

		double token_tmin = 0.0;    // default for first token
		double sentence_tmin = 0.0; // default for first sentence
		bool isFirstTokenInSentence = true;

		bool isDtwAvailable = allTokens.size > 0 && allTokens [allTokens.size]. t_dtw >= 0; // check the last token as the first might have length 0
		bool useDtw = false;
		if (my d_useDtw) {
			if (isDtwAvailable) {
				useDtw = true;
			} else {
				Melder_information (U"DTW (Dynamic Time Warping) timestamps are not available. "
						U"Falling back to time-inference trick to compute word boundaries timestamps.");
			}
		}

		for (integer i = 1; i <= allTokens.size; ++ i) {
			double token_tmax = useDtw ? allTokens [i]. t_dtw : allTokens [i]. t1;
			if (i > 1)
				token_tmin = useDtw ? allTokens [i - 1]. t_dtw : allTokens [i - 1]. t1;

			/*
				Add word to the sentence and to the full transcription text.
			*/
			MelderString_append (& sentence_text, allTokens [i].text.get());
			MelderString_append (& full_text, allTokens [i].text.get());

			/*
				Store start of sentence timestamp.
			*/
			if (isFirstTokenInSentence) {
				sentence_tmin = token_tmin;
				isFirstTokenInSentence = false;
			}

			/*
				Create and store sentence segment if sentence is complete.
			*/
			bool isLastTokenInSentence = isSentenceEndingPunctuation (allTokens [i].text.get());
			bool isLastTokenOverall = (i == allTokens.size);
			if (isLastTokenInSentence || isLastTokenOverall) {
				WhisperSegment *sentence = sentences.append();
				sentence -> text = Melder_dup (sentence_text.string);
				sentence -> tmin = sentence_tmin;
				sentence -> tmax = token_tmax;
				trace (U"Sentence: [ ", sentence -> tmin, U" - ", sentence -> tmax, U" ] ",	sentence -> text.get());
				MelderString_empty (& sentence_text);
				isFirstTokenInSentence = true;
			}

			/*
				Remove sentence-ending punctuation from token if necessary.
			*/
			if (isLastTokenInSentence) {
				integer length = Melder_length (allTokens [i].text.get());
				Melder_assert (length > 0);
				allTokens [i].text [length - 1] = U'\0';
			}

			/*
				Create word-level segment (making sure there are no zero-length word segments).
			*/
			Melder_assert (token_tmax >= token_tmin);
			conststring32 token_text = allTokens [i]. text.get();
			bool isNewWord = (token_text [0] == U' ' || words.size == 0);
			if (token_text [0] == U' ')
				token_text ++;   // skip leading space
			if (token_tmax == token_tmin)
				isNewWord = false;	// zero length token -> merge into previous one

			if (isNewWord) { // new word: trip leading space from text
				WhisperSegment *word = words.append();
				word -> text = Melder_dup (token_text);
				word -> tmin = token_tmin;
				word -> tmax = token_tmax;
				trace (U"Word ", words.size, U": [ ", word -> tmin, U" - ", word -> tmax, U" ]", word -> text.get());
			} else if (i > 1) { // continuation token: merge into last word
				Melder_assert (words.size > 0);
				WhisperSegment & word = words [words.size];
				word.text = Melder_dup (Melder_cat (word.text.get(), token_text));
				word.tmax = token_tmax;
				trace (U"Word ", words.size, U": [ ", word. tmin, U" - ", word. tmax, U" ]", word. text.get());
			}
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

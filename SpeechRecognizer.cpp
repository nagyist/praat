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

		// whisperContext
		whisper_context_params contextParams = whisper_context_default_params();
		contextParams.use_gpu = false;
		contextParams.flash_attn = false;

		conststring32 modelPath = Melder_cat (theWhisperModelsFolder (), U"/", modelName);
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

autostring32 SpeechRecognizer_recognize (SpeechRecognizer me, constSound sound) {
	try {
		whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
		params.print_progress = true;   // also default
		params.print_special = false;   // also default
		params.print_timestamps = true; // also default
		params.token_timestamps = true; // also default, necessary if we want to time tokens
		params.single_segment = true;  // force single segment output
		params.split_on_word = true;    // not default, not clear what it is doing
		params.debug_mode = true;       // not default, not clear what it is doing
		//params.detect_language = true;  // default false, if true - only detecting the language and not transcribing

		if (whisper_is_multilingual (my whisperContext.get ())) {
			if (my d_languageName && !str32str(my d_languageName.get (), U"Autodetect")) {
				params.language = whisper_lang_str (whisper_lang_id (Melder_32to8 (my d_languageName.get ()).get ()));
			} else {
				params.language = "auto";
			}
		}

		// resample sound to 16kHz if needed
		autoSound resampled = Sound_resample (sound, 16000, 50);
		sound = resampled.get ();

		// convert sound to float32 for whisper
		std::vector<float> samples32;
		samples32.reserve (sound -> nx);
		for (integer i = 1; i <= sound -> nx; ++i)
			samples32.push_back (static_cast<float>(sound -> z[1][i]));

		// run whisper
		if (whisper_full (my whisperContext.get (), params, samples32.data(), static_cast<int>(sound -> nx)) != 0)
			Melder_throw (U"Whisper failed to process audio");
		autostring32 result = Melder_8to32 (whisper_full_get_segment_text (my whisperContext.get (), 0));

		return result;

	} catch (MelderError) {
		Melder_throw (U"Sound not transcribed.");
	}
}

static conststring32 theWhisperModelsFolder () {
	static autostring32 whisperModelFolder;

	if (! whisperModelFolder) {
		try {
			structMelderFolder modelsFolder { };
			MelderFolder_getSubfolder (Melder_preferencesFolder (),	U"models/whispercpp", & modelsFolder);
			if (! MelderFolder_exists (& modelsFolder))
				MelderFolder_create (& modelsFolder);
			whisperModelFolder = Melder_dup (MelderFolder_peekPath (& modelsFolder));
		} catch (MelderError) {
			Melder_clearError ();
		}
	}
	return whisperModelFolder.get();
}

constSTRVEC theSpeechRecognizerModelNames () {
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
			for (uint8 i = 0; i < nLanguages; ++i) {
				autostring32 languageName = Melder_dup (Melder_peek8to32 (whisper_lang_str_full (i)));

				// capitalize first letter
				if (languageName [0] != U'\0')
					languageName [0] = Melder_toUpperCase (languageName [0]);

				unsorted.append(languageName.get ());
			}
			autoSTRVEC sorted = sort_STRVEC (unsorted.get());

			// create a list to return (with autodetect option)
			sortedWhisperLanguageNames.append(U"Autodetect language");
			for (integer i = 1; i <= sorted.size; ++i)
				sortedWhisperLanguageNames.append (sorted[i].get());

		} catch (MelderError) {
			unsorted.reset();
			sortedWhisperLanguageNames.reset();
			Melder_clearError ();
		}
	}
	return sortedWhisperLanguageNames.get();
}



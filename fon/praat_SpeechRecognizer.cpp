#include "praat_SpeechRecognizer.h"
#include "SpeechRecognizer.h"
#include "praatM.h"


DIRECT (HELP__SpeechRecognizer_help) {
	HELP (U"SpeechRecognizer")
}

FORM (CREATE_ONE__SpeechRecognizer_create, U"Create SpeechRecognizer", U"Create SpeechRecognizer...") {
	static autoSTRVEC modelNames = copy_STRVEC (theSpeechRecognizerModelNames());   // cannot be called twice in the same scope
	LISTNUMSTR (modelIndex, modelName, U"Whisper model", modelNames.get(),
		static_cast <int> (NUMfindFirst (modelNames.get(), theSpeechRecognizerDefaultModelName)))
	LISTNUMSTR (languageIndex, languageName, U"Language", theSpeechRecognizerLanguageNames(),
		static_cast <int> (NUMfindFirst (theSpeechRecognizerLanguageNames (), theSpeechRecognizerDefaultLanguageName)))
	OK
DO
	CREATE_ONE
		Melder_require (modelNames.size > 0,
			U"Found no Whisper-cpp models to do speech recognition with.\n"
			U"You can install them into the subfolders “whispercpp” of the folder “models” in the Praat preferences folder, and restart Praat."
		);
		autoSpeechRecognizer result = SpeechRecognizer_create (modelName, languageName);
	CREATE_ONE_END (modelName, U"_", languageName)
}

DIRECT (QUERY_ONE_FOR_STRING__SpeechRecognizer_getModelName) {
	QUERY_ONE_FOR_STRING (SpeechRecognizer)
		conststring32 result = my d_modelName.get();
	QUERY_ONE_FOR_STRING_END
}

DIRECT (QUERY_ONE_FOR_STRING__SpeechRecognizer_getLanguageName) {
	QUERY_ONE_FOR_STRING (SpeechRecognizer)
		conststring32 result = my d_languageName.get();
	QUERY_ONE_FOR_STRING_END
}

DIRECT (QUERY_ONE_AND_ONE_FOR_STRING__SpeechRecognizer_Sound_recognize) {
	QUERY_ONE_AND_ONE_FOR_STRING (SpeechRecognizer, Sound)
		autostring32 text = SpeechRecognizer_recognize (me, you);
		conststring32 result = text.get();
	QUERY_ONE_AND_ONE_FOR_STRING_END
}

void praat_SpeechRecognizer_init () {
	Thing_recognizeClassesByName (classSpeechRecognizer, nullptr);

	praat_addMenuCommand (U"Objects", U"New", U"Speech-to-text recognition", nullptr, 0, nullptr);
		praat_addMenuCommand (U"Objects", U"New", U"SpeechRecognizer help", nullptr, 1,
			HELP__SpeechRecognizer_help);
		praat_addMenuCommand (U"Objects", U"New", U"-- new SpeechRecognizer --", nullptr, 1, nullptr);
		praat_addMenuCommand (U"Objects", U"New", U"Create SpeechRecognizer...", nullptr, 1,
			CREATE_ONE__SpeechRecognizer_create);

	praat_addAction1 (classSpeechRecognizer, 0, U"SpeechRecognizer help", nullptr, 0,
		HELP__SpeechRecognizer_help);
	praat_addAction1 (classSpeechRecognizer, 0, U"Query -", nullptr, 0, nullptr);
		praat_addAction1 (classSpeechRecognizer, 1, U"Get Whisper model name", nullptr, 1,
				QUERY_ONE_FOR_STRING__SpeechRecognizer_getModelName);
		praat_addAction1 (classSpeechRecognizer, 1, U"Get language name", nullptr, 1,
				QUERY_ONE_FOR_STRING__SpeechRecognizer_getLanguageName);

	praat_addAction2 (classSpeechRecognizer, 1, classSound, 1, U"Recognize sound", nullptr, 0,
		QUERY_ONE_AND_ONE_FOR_STRING__SpeechRecognizer_Sound_recognize);
}

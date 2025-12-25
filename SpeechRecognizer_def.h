#define ooSTRUCT SpeechRecognizer
oo_DEFINE_CLASS (SpeechRecognizer, Daata)

	oo_STRING (d_modelName)
	oo_STRING (d_languageName)

	#if oo_DECLARING
		void v1_info () override;
		autoWhisperContext whisperContext;
	#endif

oo_END_CLASS (SpeechRecognizer)
#undef ooSTRUCT

/* End of file SpeechRecognizer_def.h */

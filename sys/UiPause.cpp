/* UiPause.cpp
 *
 * Copyright (C) 2009-2020,2022-2024 Paul Boersma
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
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

#include "UiPause.h"
#include "praatP.h"

static autoUiForm thePauseForm;
static int thePauseForm_clicked = 0;
static int theCancelContinueButton = 0;
static int theEventLoopDepth = 0;

static void thePauseFormOkCallback (UiForm /* sendingForm */, integer /* narg */, Stackel /* args */,
	conststring32 /* sendingString */, Interpreter /* interpreter */,
	conststring32 /* invokingButtonTitle */, bool /* modified */, void *closure, Editor optionalEditor)
{
	/*
		Get the data from the pause form.
	*/
	thePauseForm_clicked = UiForm_getClickedContinueButton (thePauseForm.get());
	if (thePauseForm_clicked != theCancelContinueButton)
		UiForm_Interpreter_addVariables (thePauseForm.get(), (Interpreter) closure);   // 'closure', not 'interpreter' or 'theInterpreter'!
}
static void thePauseFormCancelCallback (UiForm /* dia */, void * /* closure */) {
	if (theCancelContinueButton != 0) {
		thePauseForm_clicked = theCancelContinueButton;
	} else {
		thePauseForm_clicked = -1;   // STOP button
	}
}
void UiPause_begin (GuiWindow topShell, Editor optionalPauseWindowOwningEditor, conststring32 title, Interpreter interpreter) {
	if (theEventLoopDepth > 0)
		Melder_throw (Melder_upperCaseAppName(), U" cannot have more than one pause form at a time.");
	thePauseForm = UiForm_create (topShell, optionalPauseWindowOwningEditor, Melder_cat (U"Pause: ", title),
		thePauseFormOkCallback, interpreter,   // pass interpreter as closure!
		nullptr, nullptr);
}
void UiPause_real (conststring32 label, conststring32 defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “real” should be between a “beginPause” and an “endPause”.");
	UiForm_addReal (thePauseForm.get(), nullptr, nullptr, label, defaultValue);
}
void UiPause_positive (conststring32 label, conststring32 defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “positive” should be between a “beginPause” and an “endPause”.");
	UiForm_addPositive (thePauseForm.get(), nullptr, nullptr, label, defaultValue);
}
void UiPause_integer (conststring32 label, conststring32 defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “integer” should be between a “beginPause” and an “endPause”.");
	UiForm_addInteger (thePauseForm.get(), nullptr, nullptr, label, defaultValue);
}
void UiPause_natural (conststring32 label, conststring32 defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “natural” should be between a “beginPause” and an “endPause”.");
	UiForm_addNatural (thePauseForm.get(), nullptr, nullptr, label, defaultValue);
}
void UiPause_word (conststring32 label, conststring32 defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “word” should be between a “beginPause” and an “endPause”.");
	UiForm_addWord (thePauseForm.get(), nullptr, nullptr, label, defaultValue);
}
void UiPause_sentence (conststring32 label, conststring32 defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “sentence” should be between a “beginPause” and an “endPause”.");
	UiForm_addSentence (thePauseForm.get(), nullptr, nullptr, label, defaultValue);
}
void UiPause_text (conststring32 label, conststring32 defaultValue, integer numberOfLines) {
	if (! thePauseForm)
		Melder_throw (U"The function “text” should be between a “beginPause” and an “endPause”.");
	UiForm_addText (thePauseForm.get(), nullptr, nullptr, label, defaultValue, numberOfLines);
}
void UiPause_boolean (conststring32 label, bool defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “boolean” should be between a “beginPause” and an “endPause”.");
	UiForm_addBoolean (thePauseForm.get(), nullptr, nullptr, label, defaultValue);
}
void UiPause_infile (conststring32 label, conststring32 defaultValue, integer numberOfLines) {
	if (! thePauseForm)
		Melder_throw (U"The function “infile” should be between a “beginPause” and an “endPause”.");
	UiForm_addInfile (thePauseForm.get(), nullptr, nullptr, label, defaultValue, numberOfLines);
}
void UiPause_outfile (conststring32 label, conststring32 defaultValue, integer numberOfLines) {
	if (! thePauseForm)
		Melder_throw (U"The function “outfile” should be between a “beginPause” and an “endPause”.");
	UiForm_addOutfile (thePauseForm.get(), nullptr, nullptr, label, defaultValue, numberOfLines);
}
void UiPause_folder (conststring32 label, conststring32 defaultValue, integer numberOfLines) {
	if (! thePauseForm)
		Melder_throw (U"The function “folder” should be between a “beginPause” and an “endPause”.");
	UiForm_addFolder (thePauseForm.get(), nullptr, nullptr, label, defaultValue, numberOfLines);
}
void UiPause_realvector (conststring32 label, kUi_realVectorFormat defaultFormat, conststring32 defaultValue, integer numberOfLines) {
	if (! thePauseForm)
		Melder_throw (U"The function “realvector” should be between a “beginPause” and an “endPause”.");
	UiForm_addRealVector (thePauseForm.get(), nullptr, nullptr, label, defaultFormat, defaultValue, numberOfLines);
}
void UiPause_positivevector (conststring32 label, kUi_realVectorFormat defaultFormat, conststring32 defaultValue, integer numberOfLines) {
	if (! thePauseForm)
		Melder_throw (U"The function “positivevector” should be between a “beginPause” and an “endPause”.");
	UiForm_addRealVector (thePauseForm.get(), nullptr, nullptr, label, defaultFormat, defaultValue, numberOfLines);
}
void UiPause_integervector (conststring32 label, kUi_integerVectorFormat defaultFormat, conststring32 defaultValue, integer numberOfLines) {
	if (! thePauseForm)
		Melder_throw (U"The function “integervector” should be between a “beginPause” and an “endPause”.");
	UiForm_addIntegerVector (thePauseForm.get(), nullptr, nullptr, label, defaultFormat, defaultValue, numberOfLines);
}
void UiPause_naturalvector (conststring32 label, kUi_integerVectorFormat defaultFormat, conststring32 defaultValue, integer numberOfLines) {
	if (! thePauseForm)
		Melder_throw (U"The function “naturalvector” should be between a “beginPause” and an “endPause”.");
	UiForm_addIntegerVector (thePauseForm.get(), nullptr, nullptr, label, defaultFormat, defaultValue, numberOfLines);
}
void UiPause_choice (conststring32 label, int defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “choice” should be between a “beginPause” and an “endPause”.");
	UiForm_addChoice (thePauseForm.get(), nullptr, nullptr, nullptr, label, defaultValue, 1);
}
void UiPause_optionmenu (conststring32 label, int defaultValue) {
	if (! thePauseForm)
		Melder_throw (U"The function “optionmenu” should be between a “beginPause” and an “endPause”.");
	UiForm_addOptionMenu (thePauseForm.get(), nullptr, nullptr, nullptr, label, defaultValue, 1);
}
void UiPause_option (conststring32 optionText) {
	if (! thePauseForm)
		Melder_throw (U"The function “option” should be between a “beginPause” and an “endPause”.");
	UiOption option = UiForm_addOption (thePauseForm.get(), optionText);
	if (! option) {
		thePauseForm. reset();
		Melder_throw (U"Found the function “option” without a preceding “choice” or “optionmenu”.");
	}
}
void UiPause_heading (conststring32 label) {
	if (! thePauseForm)
		Melder_throw (U"The function “comment” should be between a “beginPause” and an “endPause”.");
	UiForm_addHeading (thePauseForm.get(), nullptr, label);
}
void UiPause_comment (conststring32 label) {
	if (! thePauseForm)
		Melder_throw (U"The function “comment” should be between a “beginPause” and an “endPause”.");
	UiForm_addComment (thePauseForm.get(), nullptr, label);
}
void UiPause_caption (conststring32 label) {
	if (! thePauseForm)
		Melder_throw (U"The function “caption” should be between a “beginPause” and an “endPause”.");
	UiForm_addCaption (thePauseForm.get(), nullptr, label);
}

static void Gui_waitAndHandleOneEvent_any () {
	#if gtk
		gtk_main_iteration ();
	#elif cocoa
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		//[theDemoEditor -> windowForm -> d_cocoaWindow   flushWindow];
		NSEvent *nsEvent = [NSApp
			nextEventMatchingMask: NSAnyEventMask
			untilDate: [NSDate distantFuture]   // wait
			inMode: NSDefaultRunLoopMode
			dequeue: YES
		];
		Melder_assert (nsEvent);
		[NSApp  sendEvent: nsEvent];
		[NSApp  updateWindows];   // called automatically?
		[pool release];
	#elif motif
		XEvent event;
		GuiNextEvent (& event);
		XtDispatchEvent (& event);
	#endif
}

int UiPause_end (int numberOfContinueButtons, int defaultContinueButton, int cancelContinueButton,
	conststring32 continueText1, conststring32 continueText2, conststring32 continueText3,
	conststring32 continueText4, conststring32 continueText5, conststring32 continueText6,
	conststring32 continueText7, conststring32 continueText8, conststring32 continueText9,
	conststring32 continueText10, Interpreter interpreter)
{
	if (! thePauseForm)
		Melder_throw (U"Found the function “endPause” without a preceding “beginPause”.");
	Melder_assert (interpreter);
	Editor savedEditor = interpreter -> optionalDynamicEnvironmentEditor();
	UiForm_setPauseForm (thePauseForm.get(), numberOfContinueButtons, defaultContinueButton, cancelContinueButton,
		continueText1, continueText2, continueText3, continueText4, continueText5,
		continueText6, continueText7, continueText8, continueText9, continueText10,
		thePauseFormCancelCallback);
	theCancelContinueButton = cancelContinueButton;
	UiForm_finish (thePauseForm.get());
	const bool wasBackgrounding = Melder_backgrounding;
	//if (theCurrentPraatApplication -> batch) goto end;
	if (wasBackgrounding)
		praat_foreground ();
	/*
		Put the pause form on the screen.
	*/
	UiForm_destroyWhenUnmanaged (thePauseForm.get());
	UiForm_do (thePauseForm.get(), false);
	/*
		Wait for the user to click Stop or Continue.
	*/
	{// scope
		autoMelderSaveCurrentFolder saveFolder;
		thePauseForm_clicked = 0;
		Melder_assert (theEventLoopDepth == 0);
		theEventLoopDepth ++;
		try {
			do {
				Gui_waitAndHandleOneEvent_any ();
			} while (! thePauseForm_clicked);
		} catch (MelderError) {
			Melder_flushError (U"An error made it to the outer level in a pause window; should not occur! Please write to paul.boersma@uva.nl");
		}
		theEventLoopDepth --;
	}
	if (wasBackgrounding)
		praat_background ();
	thePauseForm. releaseToUser();   // undangle
	if (thePauseForm_clicked == -1) {
		Interpreter_stop (interpreter);
		Melder_throw (U"You interrupted the script.");
		//Melder_flushError ();
		//Melder_clearError ();
	} else {
		//Melder_casual (U"Clicked ", thePauseForm_clicked);
	}
	if (interpreter -> optionalDynamicEnvironmentEditor() != savedEditor) {
		Melder_assert (savedEditor);
		Melder_assert (! interpreter -> optionalDynamicEnvironmentEditor());
		Melder_assert (interpreter -> optionalDynamicEditorEnvironmentClassName());
				// testing the assumption that the environment can be lost but never added during pause
		Melder_throw (U"Cannot continue after pause, because the ", interpreter -> optionalDynamicEditorEnvironmentClassName(), U" has been closed.");
	}
	return thePauseForm_clicked;
}

/* End of file UiPause.cpp */


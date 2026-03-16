/* manual_SpeechRecognizer.cpp
 *
 * Copyright (C) 2026 Anastasia Shchupak
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

#include "ManPagesM.h"

void manual_SpeechRecognizer_init (ManPages me);
void manual_SpeechRecognizer_init (ManPages me) {

MAN_PAGES_BEGIN R"~~~(
################################################################################
"SpeechRecognizer"
© Anastasia Shchupak 2026-03-15

The SpeechRecognizer is one of the @@types of objects@ in Praat.
It performs automatic speech recognition (speech-to-text) on a @@Sound@ object.
The actual recognition is performed by the @@whisper.cpp@ engine,
and therefore our SpeechRecognizer is an interface to whisper.cpp.

Commands
========

Creation:
,	@@Create SpeechRecognizer...@

Recognition:
,	@@SpeechRecognizer & Sound: Transcribe|Transcribe@

Installing Whisper models
=========================

Before you can use the SpeechRecognizer, you need to install one or more Whisper model files
(in GGML format, with extension ##.bin#) into the subfolder ##whispercpp# of the folder ##models#
in the Praat preferences folder.

Whisper models come in several sizes, each offering a different trade-off between speed and accuracy.
Model names that contain ##.en# are English-only models. All other models are multilingual.
Available model sizes are: %%tiny%, %%base%, %%small%, %%medium%, %%large-v1%, %%large-v2%,
%%large-v3%, and %%large-v3-turbo% (also known as %%turbo%).
Larger models are more accurate but require more memory and processing time.

Model files can be obtained from the Hugging Face repository at
##https://huggingface.co/ggerganov/whisper.cpp/tree/main#.

################################################################################
"Create SpeechRecognizer..."
© Anastasia Shchupak 2026-03-15

Creates the @@whisper.cpp@ speech recognizer.

Settings
========

##Whisper model
:	determines which Whisper model to use for recognition.
The list is populated with the ##.bin# files found in the ##whispercpp# subfolder
of the ##models# folder in the Praat preferences folder.
Models that contain ##.en# in their name are English-only; all other models are multilingual.

##Language
:	determines the language to be recognized.
Choose ##Autodetect language# to let the model detect the language automatically.
If you know the language of the audio, selecting it explicitly may improve recognition accuracy.
Note that English-only models (those with ##.en# in the name) can only be used with
##Autodetect language# or ##English#.

Installing Whisper models
=========================

Before you can use the SpeechRecognizer, you need to install one or more Whisper model files
(in GGML format, with extension ##.bin#) into the subfolder ##whispercpp# of the folder ##models#
in the Praat preferences folder.

Whisper models come in several sizes, each offering a different trade-off between speed and accuracy.
Model names that contain ##.en# are English-only models. All other models are multilingual.
Available model sizes are: %%tiny%, %%base%, %%small%, %%medium%, %%large-v1%, %%large-v2%,
%%large-v3%, and %%large-v3-turbo% (also known as %%turbo%).
Larger models are more accurate but require more memory and processing time.

Model files can be obtained from the Hugging Face repository at
##https://huggingface.co/ggerganov/whisper.cpp/tree/main#.

################################################################################
"SpeechRecognizer & Sound: Transcribe"
© Anastasia Shchupak 2026-03-15

Performs speech recognition on the selected @@Sound@ using the selected @@SpeechRecognizer@,
and writes the recognized text to the Info window.

The sound is automatically resampled to 16 kHz (the sampling frequency expected by @@whisper.cpp@)
if its original sampling frequency differs.

The recognition uses the @@Silero VAD@ (Voice Activity Detection) built into @@whisper.cpp@
to skip non-speech parts of the audio, which improves both speed and accuracy.

The result is a flat text string containing the full transcription.

################################################################################
"TextGrid & Sound: Transcribe interval"
© Anastasia Shchupak 2026-03-15

Transcribes the audio in a specific interval of the selected @@TextGrid@ using the @@whisper.cpp@ engine,
and writes the transcription result into the TextGrid.

This command extracts the sound corresponding to the selected interval,
runs speech recognition on it, and splits the interval into sentence-level sub-intervals
with the recognized text as labels. Optionally, a word-level tier is also created.

The original interval is split into multiple intervals, one per recognized sentence.
Sentence boundaries are determined by terminal punctuation (periods, exclamation marks, question marks).
If ##Include words# is selected, then word-level alignment is also performed.
For this, a new word tier is created if one does not already exist.
The word tier then contains one interval per recognized word,
with boundaries derived from Whisper's token-level timestamps produced using Dynamic Time Warping (DTW).

Settings
========

##Tier number
:	the number of the interval tier in which the interval to be transcribed resides.

##Interval number
:	the number of the interval within the tier to transcribe.

##Include words
:	if on, a new word-level tier is created (or reused if it already exists) directly below
the selected tier. This tier is named by appending ##/word# to the name of the selected tier.
Then, each recognized word gets its own interval in the word tier,
with word-level boundaries computed using @@whisper.cpp@'s internal DTW.

##Allow silences
:	if on, @@Silero VAD@ (Voice Activity Detection) is used to detect and skip non-speech portions
of the audio before recognition. This generally improves both speed and accuracy.
Speed is improved by making the audio for recognition shorter, and accuracy by preventing Whisper from
hallucinating text for silent segments.

##Whisper model
:	determines which Whisper model to use for recognition.
The list is populated with the ##.bin# files found in the ##whispercpp# subfolder
of the ##models# folder in the Praat preferences folder.
Models that contain ##.en# in their name are English-only; all other models are multilingual.

##Language
:	determines the language to be recognized.
Choose ##Autodetect language# to let the model detect the language automatically.
If you know the language of the audio, selecting it explicitly may improve recognition accuracy.
Note that English-only models (those with ##.en# in the name) can only be used with
##Autodetect language# or ##English#.

Installing Whisper models
=========================

Before you can use the SpeechRecognizer, you need to install one or more Whisper model files
(in GGML format, with extension ##.bin#) into the subfolder ##whispercpp# of the folder ##models#
in the Praat preferences folder.

Whisper models come in several sizes, each offering a different trade-off between speed and accuracy.
Model names that contain ##.en# are English-only models. All other models are multilingual.
Available model sizes are: %%tiny%, %%base%, %%small%, %%medium%, %%large-v1%, %%large-v2%,
%%large-v3%, and %%large-v3-turbo% (also known as %%turbo%).
Larger models are more accurate but require more memory and processing time.

Model files can be obtained from the Hugging Face repository at
##https://huggingface.co/ggerganov/whisper.cpp/tree/main#.

################################################################################
"Sound: To TextGrid (speech activity, Silero)..."
© Anastasia Shchupak 2026-03-15

A command that creates a @@TextGrid@ for the selected @@Sound@ with one tier,
in which the non-speech and speech intervals are marked. The discrimination between the two is based on
the @@Silero VAD@ neural network model.

Speech activity detection, sometimes referred to as voice activity detection (VAD),
is a method to discriminate speech from non-speech segments in audio.
Unlike the spectral-flatness-based method in @@Sound: To TextGrid (speech activity, LTSF)...@,
this command uses a deep neural network (@@Silero VAD@) that was trained on a large dataset of speech
and non-speech audio.

The Silero VAD model is loaded from data compiled into Praat, so no external model files are required.
The sound is internally resampled to 16 kHz before being processed by the model.

Settings
========

##Speech probability threshold (0 - 1)
:	determines the sensitivity of the speech detector. Higher values make the detector less sensitive,
meaning that it requires stronger evidence of speech to mark a segment as speech. This reduces
false positives (non-speech incorrectly labelled as speech), but may cause some speech to be missed.
Lower values make the detector more sensitive. A default of 0.5 works well for most use cases.

##Min. non-speech interval (s)
:	determines the minimum duration for a gap between speech intervals to be considered non-speech.
Shorter gaps will be merged with the surrounding speech.
If you find that brief pauses (such as plosive closures) are splitting speech intervals
and you don't want this to happen, increase this value.

##Min. speech interval (s)
:	determines the minimum duration for an interval to be marked as speech. Shorter intervals are discarded.
This helps filter out very short bursts of noise that might otherwise be detected as speech.

##Padding added around each speech segment (s)
:	determines how much silence (or noise) is included before and after each detected speech segment. Adding a small
amount of padding ensures that speech onsets and offsets are not clipped.

##Non-speech interval label
:	the label assigned to intervals classified as non-speech in the resulting TextGrid.

##Speech interval label
:	the label assigned to intervals classified as speech in the resulting TextGrid.

Algorithm
=========

The Silero VAD model processes the audio in small frames and outputs a probability that each frame contains speech.
Consecutive frames with speech probability above the threshold are grouped into speech segments,
subject to the minimum duration and padding constraints. All other parts of the audio are marked as non-speech.

If no speech is detected anywhere in the sound, the entire duration is marked with a single non-speech interval.

################################################################################
"whisper.cpp"
© Anastasia Shchupak 2026-03-15

##Whisper# is an automatic speech recognition (ASR) system by OpenAI, designed to transcribe spoken language into written text.
Trained on large datasets of diverse audio, Whisper models generalise to many datasets and domains without the need for fine-tuning.

##whisper.cpp# is a lightweight C/C++ reimplementation of OpenAI’s Whisper,
developed by Georgi Gerganov and many other contributors.
As of March 2026, it supports multilingual speech recognition (99 languages), as well as language identification.
It is available in Praat (see @Acknowledgments).

################################################################################
"Silero VAD"
© Anastasia Shchupak 2026-03-15

Silero VAD is an open-source deep learning model developed by the Silero Team.
It is designed to detect human speech in audio streams.
It acts as an efficient voice activity detector, distinguishing speech from silence or noise.

The integration in Praat uses the @@whisper.cpp@ implementation of Silero VAD,
with Silero VAD model data compiled into Praat (see @Acknowledgments).

################################################################################
)~~~"
MAN_PAGES_END

}

/* End of file manual_SpeechRecognizer.cpp */

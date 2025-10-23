/* Sound_to_PowerCepstrogram.cpp
 *
 * Copyright (C) 2012-2025 David Weenink
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

#include "NUM2.h"
#include "Cepstrum_and_Spectrum.h"
#include "SampledIntoSampled.h"
#include "Sound_and_Spectrum.h"
#include "Sound_extensions.h"
#include "SoundFrames.h"
#include "Sound_to_PowerCepstrogram.h"

Thing_implement (SoundFrameIntoPowerCepstrogramFrame, SoundFrameIntoSampledFrame, 0);

void structSoundFrameIntoPowerCepstrogramFrame :: initBasicSoundFrameIntoPowerCepstrogramFrame (constSound input,
	mutablePowerCepstrogram output, double effectiveAnalysisWidth, kSound_windowShape windowShape)
{
	SoundFrameIntoPowerCepstrogramFrame_Parent :: initBasicSoundFrameIntoSampledFrame (input, output,
			effectiveAnalysisWidth, windowShape);
	our outputPowerCepstrogram = output;
}
	
void structSoundFrameIntoPowerCepstrogramFrame :: copyBasic (constSampledFrameIntoSampledFrame other2) {
	constSoundFrameIntoPowerCepstrogramFrame other = static_cast <constSoundFrameIntoPowerCepstrogramFrame> (other2);
	SoundFrameIntoPowerCepstrogramFrame_Parent :: copyBasic (other);
	our outputPowerCepstrogram = other -> outputPowerCepstrogram;
}

void structSoundFrameIntoPowerCepstrogramFrame :: initHeap () {
	SoundFrameIntoPowerCepstrogramFrame_Parent :: initHeap ();
	powerCepstrum = PowerCepstrum_create (outputPowerCepstrogram -> ymax, outputPowerCepstrogram -> ny);
}

#if 0
bool structSoundFrameIntoPowerCepstrogramFrame :: inputFrameToOutputFrame () {
	/*
		Step 1: spectrum of the sound frame
	*/
	fftData.part (1, soundFrameSize)  <<=  soundFrame;
	if (numberOfFourierSamples > soundFrameSize)
		fftData.part (soundFrameSize + 1, numberOfFourierSamples)  <<=  0.0;
	NUMfft_forward (fourierTable.get(), fftData.get());
	for (integer i = 1 ; i <= numberOfFourierSamples; i ++)
		fftData [i] *= sound -> dx;

	/*
		step 2: log of the spectrum power values log (re * re + im * im)
	*/
	fftData [1] = log (fftData [1] * fftData [1] + 1e-300);
	for (integer i = 1; i < numberOfFourierSamples / 2; i ++) {
		const double re = fftData [2 * i], im = fftData [2 * i + 1];
		fftData [2 * i] = log (re * re + im * im + 1e-300);
		fftData [2 * i + 1] = 0.0;
	}
	fftData [numberOfFourierSamples] = log (fftData [numberOfFourierSamples] * fftData [numberOfFourierSamples] + 1e-300);
	/*
		Step 3: inverse fft of the log spectrum
	*/
	NUMfft_backward (fourierTable.get(), fftData.get());
	const double df = 1.0 / (sound -> dx * numberOfFourierSamples);
	for (integer i = 1; i <= powercepstrum -> nx; i ++) {
		const double val = fftData [i] * df;
		powercepstrum -> z [1] [i] = val * val;
	}
	return true;
}		
#endif

bool structSoundFrameIntoPowerCepstrogramFrame :: inputFrameIntoOutputFrame (integer currentFrame) {
	/*
		Step 1: spectrum of the sound frame
		a. soundFrameToForwardFourierTransform ()
		b. scaling
	*/
	
	for (integer i = 1 ; i <= numberOfFourierSamples; i ++)
		fourierSamples [i] *= frameAsSound -> dx;

	/*
		step 2: log of the spectrum power values log (re * re + im * im)
	*/
	fourierSamples [1] = log (fourierSamples [1] * fourierSamples [1] + 1e-300);
	for (integer i = 1; i < numberOfFourierSamples / 2; i ++) {
		const double re = fourierSamples [2 * i], im = fourierSamples [2 * i + 1];
		fourierSamples [2 * i] = log (re * re + im * im + 1e-300);
		fourierSamples [2 * i + 1] = 0.0;
	}
	fourierSamples [numberOfFourierSamples] = log (fourierSamples [numberOfFourierSamples] * fourierSamples [numberOfFourierSamples] + 1e-300);
	/*
		Step 3: inverse fft of the log spectrum
	*/
	NUMfft_backward (fourierTable.get(), fourierSamples.get());
	const double df = 1.0 / (frameAsSound -> dx * numberOfFourierSamples);
	for (integer i = 1; i <= powerCepstrum -> nx; i ++) {
		const double val = fourierSamples [i] * df;
		powerCepstrum -> z [1] [i] = val * val;
	}
	return true;
}

void structSoundFrameIntoPowerCepstrogramFrame :: saveOutputFrame (integer iframe) {
	outputPowerCepstrogram -> z.column (iframe)  <<=  powerCepstrum -> z.row (1); 
}

void Sound_into_PowerCepstrogram (constSound input, mutablePowerCepstrogram output, double effectiveAnalysisWidth, kSound_windowShape windowShape) {
	SampledIntoSampled_assertEqualDomains (input, output);
	autoSoundFrameIntoPowerCepstrogramFrame frameIntoFrame = Thing_new (SoundFrameIntoPowerCepstrogramFrame);
	frameIntoFrame -> initBasicSoundFrameIntoPowerCepstrogramFrame (input, output, effectiveAnalysisWidth, windowShape);
	frameIntoFrame -> wantSpectrum = true;
	frameIntoFrame -> fftInterpolationFactor = 1;
	SampledIntoSampled_mt (frameIntoFrame.get(), 40);
}

autoPowerCepstrogram Sound_to_PowerCepstrogram_new (Sound me, double pitchFloor, double dt, double maximumFrequency, double preEmphasisFrequency) {
	try {
		const kSound_windowShape windowShape = kSound_windowShape::GAUSSIAN_2;
		const double effectiveAnalysisWidth = 3.0 / pitchFloor; // minimum analysis window has 3 periods of lowest pitch
		const double physicalAnalysisWidth = getPhysicalAnalysisWidth2 (effectiveAnalysisWidth, windowShape);
		const double physicalSoundDuration = my dx * my nx;
		volatile const double windowDuration = Melder_clippedRight (physicalAnalysisWidth, physicalSoundDuration);
		Melder_require (physicalSoundDuration >= physicalAnalysisWidth,
			U"Your sound is too short:\n"
			U"it should be longer than ", physicalAnalysisWidth, U" s."
		);
		const double samplingFrequency = 2.0 * maximumFrequency;
		autoSound input = Sound_resampleAndOrPreemphasize (me, maximumFrequency, 50_integer, preEmphasisFrequency);
		double t1;
		integer nFrames;
		Sampled_shortTermAnalysis (input.get(), windowDuration, dt, & nFrames, & t1);
		const integer soundFrameSize = getSoundFrameSize2 (physicalAnalysisWidth, input -> dx);
		const integer nfft = Melder_clippedLeft (2_integer, Melder_iroundUpToPowerOfTwo (soundFrameSize));
		const integer nq = nfft / 2 + 1;
		const double qmax = 0.5 * nfft / samplingFrequency, dq = 1.0 / samplingFrequency;
		autoPowerCepstrogram output = PowerCepstrogram_create (my xmin, my xmax, nFrames, dt, t1, 0, qmax, nq, dq, 0);
		bool subtractFrameMean = true, wantSpectrum = true;
		const double powerScaling = input -> dx * input -> dx;
		MelderThread_PARALLELIZE (nFrames, 10)
			autoSoundFrames soundFrames = SoundFrames_create (input.get(), effectiveAnalysisWidth, dt, windowShape, subtractFrameMean, wantSpectrum, 1_integer);
			Melder_assert (soundFrames -> numberOfFourierSamples == nfft);
		MelderThread_FOR (iframe) {
			soundFrames -> getFrame (iframe);
			VECVU powerCepstrum = output -> z.column (iframe);
			VEC fourierSamples = soundFrames -> fourierSamples.get();
			VEC powerSpectrum = soundFrames -> powerSpectrum.get();
			/* The average powerspectrum has now already been calculated */
			fourierSamples [1] = log (powerSpectrum [1] * powerScaling + 1e-300);
			for (integer i = 1; i <= nfft / 2; i ++) {
				fourierSamples [2 * i] = log (powerSpectrum	[i] * powerScaling + 1e-300);
				fourierSamples [2 * i + 1] = 0.0;
			}
			fourierSamples [nfft] = log (powerSpectrum [nfft] * powerScaling + 1e-300);
			/*
				Step 3: inverse fft of the log spectrum
			*/
			NUMfft_backward (soundFrames -> fourierTable.get(), fourierSamples);
			const double df = 1.0 / (input -> dx * nfft);
			for (integer i = 1; i <= nq; i ++) {
				const double val = fourierSamples [i] * df;
				powerCepstrum [i] = val * val;
			}
		} MelderThread_ENDFOR
		return output;
	} catch (MelderError) {
		Melder_throw (me, U": no PowerCepstrogram created.");
	}
}

autoPowerCepstrogram Sound_to_PowerCepstrogram_new2 (Sound me, double pitchFloor, double dt, double maximumFrequency, double preEmphasisFrequency) {
	try {
		const kSound_windowShape windowShape = kSound_windowShape::GAUSSIAN_2;
		const double effectiveAnalysisWidth = 3.0 / pitchFloor; // minimum analysis window has 3 periods of lowest pitch
		const double physicalAnalysisWidth = getPhysicalAnalysisWidth2 (effectiveAnalysisWidth, windowShape);
		const double physicalSoundDuration = my dx * my nx;
		volatile const double windowDuration = Melder_clippedRight (physicalAnalysisWidth, physicalSoundDuration);
		Melder_require (physicalSoundDuration >= physicalAnalysisWidth,
			U"Your sound is too short:\n"
			U"it should be longer than ", physicalAnalysisWidth, U" s."
		);
		const double samplingFrequency = 2.0 * maximumFrequency;
		autoSound input = Sound_resampleAndOrPreemphasize (me, maximumFrequency, 50_integer, preEmphasisFrequency);
		double t1;
		integer nFrames;
		Sampled_shortTermAnalysis (me, windowDuration, dt, & nFrames, & t1);
		const integer soundFrameSize = getSoundFrameSize2 (physicalAnalysisWidth, input -> dx);
		const integer nfft = Melder_clippedLeft (2_integer, Melder_iroundUpToPowerOfTwo (soundFrameSize));
		const integer nq = nfft / 2 + 1;
		const double qmax = 0.5 * nfft / samplingFrequency, dq = 1.0 / samplingFrequency;
		autoPowerCepstrogram output = PowerCepstrogram_create (my xmin, my xmax, nFrames, dt, t1, 0, qmax, nq, dq, 0);
		Sound_into_PowerCepstrogram (input.get(), output.get(), effectiveAnalysisWidth, windowShape);
		return output;
	} catch (MelderError) {
		Melder_throw (me, U": no PowerCepstrogram created.");
	}
}

autoPowerCepstrogram Sound_to_PowerCepstrogram_old (Sound me, double pitchFloor, double dt, double maximumFrequency, double preEmphasisFrequency) {
	try {
		const double analysisWidth = 3.0 / pitchFloor; // minimum analysis window has 3 periods of lowest pitch
		const double physicalAnalysisWidth = 2.0 * analysisWidth;
		const double physicalSoundDuration = my dx * my nx;
		volatile const double windowDuration = Melder_clippedRight (2.0 * analysisWidth, my dx * my nx);   // gaussian window
		Melder_require (physicalSoundDuration >= physicalAnalysisWidth,
			U"Your sound is too short:\n"
			U"it should be longer than 6.0 / pitchFloor (", physicalAnalysisWidth, U" s)."
		);
		// Convenience: analyse the whole sound into one Cepstrogram_frame
		const double samplingFrequency = 2.0 * maximumFrequency;
		autoSound sound = Sound_resample (me, samplingFrequency, 50);
		Sound_preEmphasize_inplace (sound.get(), preEmphasisFrequency);
		double t1;
		integer nFrames;
		Sampled_shortTermAnalysis (me, windowDuration, dt, & nFrames, & t1);
		autoSound sframe = Sound_createSimple (1_integer, windowDuration, samplingFrequency);
		autoSound window = Sound_createGaussian (windowDuration, samplingFrequency);
		/*
			Find out the size of the FFT
		*/
		const integer nfft = Melder_clippedLeft (2_integer, Melder_iroundUpToPowerOfTwo (sframe -> nx));   // TODO: explain edge case
		const integer nq = nfft / 2 + 1;
		const double qmax = 0.5 * nfft / samplingFrequency, dq = 1.0 / samplingFrequency;
		autoPowerCepstrogram thee = PowerCepstrogram_create (my xmin, my xmax, nFrames, dt, t1, 0, qmax, nq, dq, 0);

		autoMelderProgress progress (U"Cepstrogram analysis");

		for (integer iframe = 1; iframe <= nFrames; iframe++) {
			const double t = Sampled_indexToX (thee.get(), iframe); // TODO express the following 3 lines more clearly
			Sound_into_Sound (sound.get(), sframe.get(), t - windowDuration / 2);
			Vector_subtractMean (sframe.get());
			Sounds_multiply (sframe.get(), window.get());
			autoSpectrum spec = Sound_to_Spectrum (sframe.get(), true);   // FFT yes
			autoPowerCepstrum cepstrum = Spectrum_to_PowerCepstrum (spec.get());
			for (integer i = 1; i <= nq; i ++)
				thy z [i] [iframe] = cepstrum -> z [1] [i];

			if (iframe % 10 == 1)
				Melder_progress ((double) iframe / nFrames, U"PowerCepstrogram analysis of frame ",
						iframe, U" out of ", nFrames, U".");
		}
		return thee;
	} catch (MelderError) {
		Melder_throw (me, U": no PowerCepstrogram created.");
	}
}

autoPowerCepstrogram Sound_to_PowerCepstrogram (Sound me, double pitchFloor, double dt, double maximumFrequency, double preEmphasisFrequency) {
	autoPowerCepstrogram result;
	if (Melder_debug == -10)
		result = Sound_to_PowerCepstrogram_old (me, pitchFloor, dt, maximumFrequency, preEmphasisFrequency);
	else
		result = Sound_to_PowerCepstrogram_new (me, pitchFloor, dt, maximumFrequency, preEmphasisFrequency);
	return result;
}

/* End of file Sound_to_PowerCepstrogram.cpp */

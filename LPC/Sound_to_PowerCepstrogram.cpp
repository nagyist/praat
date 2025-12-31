/* Sound_to_PowerCepstrogram.cpp
 *
 * Copyright (C) 2012-2025 David Weenink, 2025 Paul Boersma
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
#include "SoundFrameIntoSampledFrame.h"   // needed only for getPhysicalAnalysisWidth2; TODO: move that function into the right location

static void Sound_into_PowerCepstrogram (constSound input, mutablePowerCepstrogram output, double effectiveAnalysisWidth, 
	kSound_windowShape windowShape) 
{
	SampledIntoSampled_assertEqualDomains (input, output);
	constexpr integer thresholdNumberOfFramesPerThread = 40;
	const integer numberOfFrames = output -> nx;
	//autoMelderProgress progress (U"Analyse power cepstrogram...");

	/*
		The stuff that is constant, and equal among threads.
	*/
	const double physicalAnalysisWidth = getPhysicalAnalysisWidth2 (effectiveAnalysisWidth, windowShape);
	const integer soundFrameSize = getSoundFrameSize2 (physicalAnalysisWidth, input -> dx);
	autoVEC windowFunction = raw_VEC (soundFrameSize);
	windowShape_into_VEC (windowShape, windowFunction.get());

	MelderThread_PARALLEL (numberOfFrames, thresholdNumberOfFramesPerThread) {
		bool subtractFrameMean = true;   // TODO: check
		integer fftInterpolationFactor = 1;

		autoPowerCepstrum powerCepstrum = PowerCepstrum_create (output -> ymax, output -> ny);
		autoSound frameAsSound = Sound_create (1, 0.0, soundFrameSize * input -> dx, soundFrameSize, input -> dx, 0.5 * input -> dx);
		VEC soundFrame = frameAsSound -> z.row (1);
		Melder_assert (soundFrame.size == soundFrameSize);

		/*
			Spectrum.
		*/
		integer numberOfFourierSamples = frameAsSound -> nx;
		if (fftInterpolationFactor > 0) {
			numberOfFourierSamples = Melder_iroundUpToPowerOfTwo (numberOfFourierSamples);
			for (integer imultiply = fftInterpolationFactor; imultiply > 1; imultiply --)
				numberOfFourierSamples *= 2;
		}
		autoVEC fourierSamples = raw_VEC (numberOfFourierSamples);
		autoVEC fourierSamplesChannel = raw_VEC (numberOfFourierSamples);
		const integer numberOfFrequencies = numberOfFourierSamples / 2 + 1;
		autoNUMFourierTable fourierTable = NUMFourierTable_create (numberOfFourierSamples);
		autoSpectrum spectrum = Spectrum_create (0.5 / frameAsSound -> dx, numberOfFrequencies);
		spectrum -> dx = 1.0 / (frameAsSound -> dx * numberOfFourierSamples);
		MelderThread_FOR (iframe) {
			if (MelderThread_IS_MASTER && 0) {
				const double estimatedProgress = MelderThread_ESTIMATED_PROGRESS;
				Melder_progress (0.98 * estimatedProgress,
					U"Analysed approximately ", Melder_iround (numberOfFrames * estimatedProgress),
					U" out of ", numberOfFrames, U" frames"
				);
			}
			const double midTime = Sampled_indexToX (output, iframe);
			integer soundFrameBegin = Sampled_xToNearestIndex (input, midTime - 0.5 * physicalAnalysisWidth);   // approximation
			integer channelBegin = soundFrameBegin;

			for (integer isample = 1; isample <= soundFrame.size; isample ++, channelBegin ++)
				soundFrame [isample] = ( channelBegin > 0 && channelBegin <= input -> nx ? input -> z [1] [channelBegin] : 0.0 );
			if (subtractFrameMean)
				centre_VEC_inout (soundFrame, nullptr);
			//const double soundFrameExtremum = NUMextremum_u (soundFrame);   // not used
			soundFrame  *=  windowFunction.get();
			/*
				Step 1:
				Take the channel unscaled power spectra and average them
			*/
			fourierSamples.part (1, soundFrameSize)  <<=  frameAsSound -> z.row (1);
			fourierSamples. part (soundFrameSize + 1, fourierSamples.size)  <<=  0.0;
			NUMfft_forward (fourierTable.get(), fourierSamples.get());
			for (integer i = 1; i <= fourierSamples.size; i ++) // unscaled "power"
				fourierSamples [i] *= fourierSamples [i];
			const integer numberOfChannels = frameAsSound -> ny;
			if (numberOfChannels > 1) {
				/*
					Multiple channels: take the average of the power spectra
					scaling is not necessary yet
				*/
				for (integer ichan = 1; ichan <= numberOfChannels; ichan ++) {
					channelBegin = soundFrameBegin;
					for (integer isample = 1; isample <= soundFrame.size; isample ++, channelBegin ++)
						soundFrame [isample] = ( channelBegin > 0 && channelBegin <= input -> nx ? input -> z [ichan] [channelBegin] : 0.0 );
					soundFrame  *=  windowFunction.get();
					fourierSamplesChannel.part (1, soundFrameSize)  <<=  frameAsSound -> z.row (ichan);
					fourierSamples. part (soundFrameSize + 1, fourierSamples.size)  <<=  0.0;
					NUMfft_forward (fourierTable.get(), fourierSamples.get());
					for (integer i = 1; i <= fourierSamples.size; i ++) // to unscaled "power"
						fourierSamples [i] *= fourierSamples [i];
					fourierSamples.get() +=  fourierSamplesChannel.get();
				}
				fourierSamples.get() /= numberOfChannels;
			}

			/*
				Step 2:
				Scale the (average) power spectrum
			*/
			const VEC re = spectrum -> z.row (1);
			const VEC im = spectrum -> z.row (2);
			const integer numberOfFrequencies = spectrum -> nx;
			const double scaling = output -> dx * output -> dx; // because we squared already
			re [1] = fourierSamples [1] * scaling;
			im [1] = 0.0;
			for (integer i = 2; i < numberOfFrequencies; i ++) {
				re [i] = fourierSamples [i + i - 2] * scaling;   // fourierSamples [2], [4], ...
				im [i] = fourierSamples [i + i - 1] * scaling;   // fourierSamples [3], [5], ...
			}
			if ((numberOfFourierSamples & 1) != 0) {
				if (numberOfFourierSamples > 1) {
					re [numberOfFrequencies] = fourierSamples [numberOfFourierSamples - 1] * scaling;
					im [numberOfFrequencies] = fourierSamples [numberOfFourierSamples] * scaling;
				}
			} else {
				re [numberOfFrequencies] = fourierSamples [numberOfFourierSamples] * scaling;
				im [numberOfFrequencies] = 0.0;
			}

			/*
				Step 3:
				Take log of the spectrum power values log (re + im ) because we already squared
			*/
			fourierSamples [1] = log ( re [1] + 1e-300);
			for (integer i = 1; i < numberOfFrequencies / 2; i ++) {
				fourierSamples [2 * i] = log (re [i] + im [i] + 1e-300);
				fourierSamples [2 * i + 1] = 0.0;
			}
			fourierSamples [numberOfFrequencies] = log (re [numberOfFrequencies] + 1e-300);
			/*
				Step 4: inverse fft of the log spectrum
			*/
			NUMfft_backward (fourierTable.get(), fourierSamples.get());
			const double df = 1.0 / (frameAsSound -> dx * numberOfFourierSamples);
			for (integer i = 1; i <= powerCepstrum -> nx; i ++) {
				const double val = fourierSamples [i] * df;
				powerCepstrum -> z [1] [i] = val * val;
			}
			output -> z.column (iframe)  <<=  powerCepstrum -> z.row (1);
		}
	} MelderThread_ENDPARALLEL
}

autoPowerCepstrogram Sound_to_PowerCepstrogram_new (constSound me, double pitchFloor, double dt, double maximumFrequency, double preEmphasisFrequency) {
	try {
		const kSound_windowShape windowShape = kSound_windowShape::GAUSSIAN_2;
		const double effectiveAnalysisWidth = 3.0 / pitchFloor; // minimum analysis window has 3 periods of lowest pitch
		const double physicalAnalysisWidth = getPhysicalAnalysisWidth (effectiveAnalysisWidth, windowShape);
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
		const integer soundFrameSize = getSoundFrameSize (physicalAnalysisWidth, input -> dx);
		const integer numberOfFourierSamples = Melder_clippedLeft (2_integer, Melder_iroundUpToPowerOfTwo (soundFrameSize));
		const integer halfNumberOfFourierSamples = numberOfFourierSamples / 2;
		const integer numberOfFrequencies = halfNumberOfFourierSamples + 1;
		const integer numberOfChannels = my ny;
		const double qmax = 0.5 * numberOfFourierSamples / samplingFrequency, dq = 1.0 / samplingFrequency;
		autoPowerCepstrogram output = PowerCepstrogram_create (my xmin, my xmax, nFrames, dt, t1, 0, qmax, numberOfFrequencies, dq, 0);
		bool subtractFrameMean = true;
		const double powerScaling = input -> dx * input -> dx; // =amplitude_scaling^2

		MelderThread_PARALLEL (nFrames, 10) {
			autoSoundFrames soundFrames = SoundFrames_createForIntoSampled (input.get(), output.get(), effectiveAnalysisWidth, windowShape, subtractFrameMean);
			autoVEC fourierSamples = raw_VEC (numberOfFourierSamples);
			autoVEC power_channelAveraged = raw_VEC (numberOfFourierSamples);
			autoVEC onesidedPSD = raw_VEC (numberOfFrequencies);
			autoNUMFourierTable fourierTable = NUMFourierTable_create (numberOfFourierSamples);		// of dimension numberOfFourierSamples;
			MelderThread_FOR (iframe) {
				/*
					Get average power spectrum of channels
					Let X(f) be the Fourier Transform of x(t) defined on the domain [-F,+F].
					Power P[f] of a spectral component X(f):
						P[f] =	|X(f)/sqrt(2)|^2 = 0.5|X(f)|^2 for f != 0,
								|X(0)|^2 for f=0.
					The onesidedPSD [f] =  2 * P (f)=X(f)^2 for f >= 0 and |X(0)|^2 for f=0
					The bin width of the first and last frequency in the onesidedPSD is half the bin width at the other frequencies
					Do scaling and averaging together
				*/
				soundFrames -> getFrame (iframe);
				Sound sound = soundFrames -> frameAsSound.get();
				power_channelAveraged.get()  <<=  0.0;
				onesidedPSD.get()  <<=  0.0;
				for (integer ichannel = 1; ichannel <= numberOfChannels; ichannel ++) {
					fourierSamples.part (1, soundFrameSize)  <<=  sound -> z.row (ichannel);
					fourierSamples.part (soundFrameSize + 1, numberOfFourierSamples)  <<=  0.0;
					NUMfft_forward (fourierTable.get(), fourierSamples.get());
					onesidedPSD [1] += fourierSamples [1] * fourierSamples [1];
					for (integer i = 2; i < numberOfFrequencies; i ++) {
						double re = fourierSamples [2 * i - 2], im = fourierSamples [2 * i - 1];
						onesidedPSD [i] += re * re + im * im;
					}
					onesidedPSD [numberOfFrequencies] += fourierSamples [numberOfFourierSamples] * 	fourierSamples [numberOfFourierSamples];
					for (integer i = 1; i < numberOfFourierSamples; i ++)
						power_channelAveraged [i] += fourierSamples [i] * fourierSamples [i];
				}
				onesidedPSD.get()  *=  powerScaling / numberOfChannels; // scaling and averaging over channels
				power_channelAveraged.get()  *=  powerScaling / numberOfChannels;
				/*
					Get log power.
				*/
				fourierSamples [1] = log (onesidedPSD [1] + 1e-300);
				for (integer i = 2; i < numberOfFrequencies; i ++) {
					fourierSamples [2 * i - 2] = log (onesidedPSD [i] + 1e-300);
					fourierSamples [2 * i - 1] = 0.0;
				}
				fourierSamples [numberOfFourierSamples] = log (onesidedPSD [numberOfFrequencies]);
				/*
					Inverse transform
				*/
				NUMfft_backward (fourierTable.get(), fourierSamples.get());
				/*
					scale first.
				*/
				const double df = 1.0 / (sound -> dx * numberOfFourierSamples);
				fourierSamples.get()  *=  df;
				for (integer i = 1; i <= numberOfFrequencies; i ++)
					output -> z [i] [iframe] = fourierSamples [i] * fourierSamples [i];
			}
		} MelderThread_ENDPARALLEL
		return output;
	} catch (MelderError) {
		Melder_throw (me, U": no PowerCepstrogram created.");
	}
}

static autoPowerCepstrogram Sound_to_PowerCepstrogram_old (Sound me, double pitchFloor, double dt, double maximumFrequency, double preEmphasisFrequency) {
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

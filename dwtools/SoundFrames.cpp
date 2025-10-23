/* SoundFrames.cpp
 *
 * Copyright (C) 2025 David Weenink
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
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

#include "SoundFrames.h"

Thing_implement (SoundFrames, Thing, 0);

void structSoundFrames :: init (constSound input, double effectiveAnalysisWidth, double timeStep, 
	kSound_windowShape windowShape, bool subtractFrameChannelMean, bool wantPowerSpectrum, 
	integer fftInterpolationFactor)
{
	our inputSound = input;
	our physicalAnalysisWidth = getPhysicalAnalysisWidth (effectiveAnalysisWidth, windowShape);
	if (timeStep == 0.0) {
		// calculate output_dt
	}
	our dt = timeStep;
	Sampled_shortTermAnalysis (inputSound, physicalAnalysisWidth, dt, & our numberOfFrames, & our t1);
	initCommon (windowShape, subtractFrameChannelMean, wantPowerSpectrum, fftInterpolationFactor);
}
	
void structSoundFrames :: initWithSampled (constSound input, constSampled output, double effectiveAnalysisWidth,
	kSound_windowShape windowShape, bool subtractFrameChannelMean, bool wantPowerSpectrum,
	integer fftInterpolationFactor)
{
	Melder_assert (input -> xmin == output -> xmin && input -> xmax == output -> xmax);
	our inputSound = input;
	our t1 = output -> x1;
	our numberOfFrames = output -> nx;
	our dt = output -> dx;
	our physicalAnalysisWidth = getPhysicalAnalysisWidth (effectiveAnalysisWidth, windowShape);	
	initCommon (windowShape, subtractFrameChannelMean, wantPowerSpectrum, fftInterpolationFactor);
}

void structSoundFrames :: initCommon (kSound_windowShape windowShape, bool subtractChannelMean,
	bool wantPowerSpectrum, integer fftInterpolationFactor)
{
	our windowShape = windowShape;
	our subtractChannelMean = subtractChannelMean;
	our wantPowerSpectrum = wantPowerSpectrum;
	const integer numberOfChannels = inputSound -> ny;
	soundFrameSize = getSoundFrameSize (physicalAnalysisWidth, inputSound -> dx);
	windowFunction = raw_VEC (soundFrameSize);   // TODO: move out of thread repetition
	windowShape_into_VEC (windowShape, windowFunction.get());
	frameAsSound = Sound_create (numberOfChannels, 0.0, soundFrameSize * inputSound -> dx, soundFrameSize,
		inputSound -> dx, 0.5 * inputSound -> dx);
	our frameChannelMeans = raw_VEC (numberOfChannels);
	soundFrame = raw_VEC (soundFrameSize);
	if (wantPowerSpectrum) {
		numberOfFourierSamples = frameAsSound -> nx;
		if (fftInterpolationFactor > 0) {
			numberOfFourierSamples = Melder_iroundUpToPowerOfTwo (numberOfFourierSamples);
			for (integer imultiply = fftInterpolationFactor; imultiply > 1; imultiply --)
				numberOfFourierSamples *= 2;
		}
		fourierSamples = raw_VEC (numberOfFourierSamples);
		fourierTable = NUMFourierTable_create (numberOfFourierSamples);
		const integer numberOfFrequencies = numberOfFourierSamples / 2 + 1;
		powerSpectrum = raw_VEC (numberOfFrequencies);
		channelPowerSpectrum = raw_VEC (numberOfFrequencies);
	}
}

VEC structSoundFrames :: getFrame (integer iframe) {
	const double midTime = t1 + (iframe - 1) * dt;
	integer startSample = Sampled_xToNearestIndex (inputSound, midTime - 0.5 * physicalAnalysisWidth); // approximation
	const integer numberOfChannels = inputSound -> ny;
	VEC powerspectrum = powerSpectrum.get(); // instead of always using the .get()
	if (wantPowerSpectrum)
		powerspectrum  <<=  0.0;
	for (integer ichannel = 1; ichannel <= numberOfChannels; ichannel ++) {
		VEC soundChannel = inputSound -> z.row (ichannel), frameChannel = frameAsSound -> z.row (ichannel);
		integer currentSample = startSample;
		for (integer i = 1; i <= soundFrame.size; i ++, currentSample ++)
			frameChannel [i] = (( currentSample > 0 && currentSample <= inputSound -> nx) ? soundChannel [currentSample] : 0.0 );

		if (subtractChannelMean)
			for (integer ichannel = 1; ichannel <= numberOfChannels; ichannel ++) {
				double mean;
				centre_VEC_inout (frameChannel, & mean);
				frameChannelMeans [ichannel] = mean;
			}
		frameChannel  *=  windowFunction.get();
	}
	
	for (integer i = 1; i <= soundFrameSize; i ++)
		soundFrame [i] = Sampled_getValueAtSample (frameAsSound.get(), i, 0, 0); // average channels
		
	return soundFrame.get();
}

void structSoundFrames :: getPowerSpectrum (VEC const& powerspectrum) {
	Melder_assert (wantPowerSpectrum);
	Melder_assert (powerspectrum.size == numberOfFourierSamples / 2 + 1);
	const integer numberOfChannels = inputSound -> ny;
	powerspectrum  <<=  0.0;
	for (integer ichannel = 1; ichannel <= numberOfChannels; ichannel ++) {
		getFrameChannelPowerSpectrum (ichannel);
		powerspectrum += channelPowerSpectrum.get();
	}
	if (numberOfChannels > 1)
		powerspectrum /= numberOfChannels;
}

void structSoundFrames :: getFrameChannelPowerSpectrum (integer ichannel) {
	Melder_assert (wantPowerSpectrum);
	Melder_assert (ichannel > 0 && ichannel <= inputSound -> ny);
	fourierSamples.part (1, soundFrameSize)  <<=  frameAsSound -> z.row (ichannel);
	fourierSamples.part (soundFrameSize + 1, numberOfFourierSamples)  <<=  0.0;
	VEC data = fourierSamples.get();
	NUMfft_forward (fourierTable.get(), data);
	const integer numberOfFrequencies = channelPowerSpectrum.size;
	channelPowerSpectrum [1] = data [1] * data [1];
	for (integer i = 2; i < numberOfFrequencies; i ++ )
		channelPowerSpectrum [i] = data [2 * i - 2] * data [2 * i - 2] + data [2 * i - 1] * data [2 * i - 1];
	channelPowerSpectrum [numberOfFrequencies] = data [numberOfFourierSamples] * data [numberOfFourierSamples];
}

autoSoundFrames SoundFrames_create (constSound input, constSampled output, double effectiveAnalysisWidth,
	kSound_windowShape windowShape, bool subtractFrameChannelMean, bool wantPowerSpectrum, integer fftInterpolationFactor)
{
	try {
		autoSoundFrames me = Thing_new (SoundFrames);
		my initWithSampled (input, output, effectiveAnalysisWidth, windowShape, subtractFrameChannelMean,
			wantPowerSpectrum, fftInterpolationFactor);
		return me;
	} catch (MelderError) {
		Melder_throw (U"SoundFrames (with Sampled) could not be created.");
	}
}


autoSoundFrames SoundFrames_create (constSound input, double effectiveAnalysisWidth,
	double timeStep, kSound_windowShape windowShape,
	bool subtractFrameChannelMean, bool wantPowerSpectrum, integer fftInterpolationFactor)
{
	try {
		autoSoundFrames me = Thing_new (SoundFrames);
		my init (input, effectiveAnalysisWidth, timeStep, windowShape, subtractFrameChannelMean,
			wantPowerSpectrum, fftInterpolationFactor);
		return me;
	} catch (MelderError) {
		Melder_throw (U"SoundFrames could not be created.");
	}
}

/* End of file SoundFrames.cpp */

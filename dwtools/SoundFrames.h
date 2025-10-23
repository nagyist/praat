#ifndef _SoundFrames_h_
#define _SoundFrames_h_
/* SoundFrames.h
 *
 * Copyright (C) 2024-2025 David Weenink
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

#include "melder.h"
#include "NUMFourier.h"
#include "Sound_extensions.h"
#include "SampledFrameIntoSampledFrame.h"
#include "Spectrum.h"

inline integer getSoundFrameSize_odd (double approximatePhysicalAnalysisWidth, double samplingPeriod) {
	const double halfFrameDuration = 0.5 * approximatePhysicalAnalysisWidth;
	const integer halfFrameSamples = Melder_ifloor (halfFrameDuration / samplingPeriod);
	return 2 * halfFrameSamples + 1;
}

inline integer getSoundFrameSize (double physicalAnalysisWidth, double samplingPeriod) {
	Melder_assert (physicalAnalysisWidth > 0.0);
	Melder_assert (samplingPeriod > 0.0);
	return Melder_iround (physicalAnalysisWidth / samplingPeriod);
}

inline double getPhysicalAnalysisWidth (double effectiveAnalysisWidth, kSound_windowShape windowShape) {
	const double physicalAnalysisWidth = ( windowShape == kSound_windowShape::RECTANGULAR ||
		windowShape == kSound_windowShape::TRIANGULAR || windowShape == kSound_windowShape::HAMMING ||
		windowShape == kSound_windowShape::HANNING ? effectiveAnalysisWidth : 2.0 * effectiveAnalysisWidth )
	;
	return physicalAnalysisWidth;
}

Thing_define (SoundFrames, Thing) {

	constSound inputSound;
	double t1, dt;
	integer numberOfFrames;
	double physicalAnalysisWidth;			// depends on the effectiveAnalysiswidth and the window window shape
	integer soundFrameSize; 				// determined by the physicalAnalysisWidth and the samplingFrequency of the Sound
	autoSound frameAsSound;					// the (possibly multichannel) frame as a Sound
	autoVEC soundFrameExtremum;				// the largest amplitude in each inputSound frame channel either positive or negative
	autoVEC windowFunction;					// the actual window used of size soundFrameSize
	autoVEC soundFrame;						// conveniance: average odf the channels
	kSound_windowShape windowShape;			// Type: Rectangular, triangular, hamming, etc..
	bool subtractChannelMean = true;		// if true, the frame mean will be subtracted before windowing
	autoVEC frameChannelMeans;
	bool wantPowerSpectrum = false;			// of the frameAsSound;
	autoVEC powerSpectrum;					// size numberOfFourierSamples / 2 + 1
	autoVEC channelPowerSpectrum;
	integer fftInterpolationFactor = 1;		// 0 = DFT, 1 = FFT, 2, 4, 8 FFT with extra zero's
	integer numberOfFourierSamples;
	autoVEC fourierSamples;					// size = numberOfFourierSamples
	autoNUMFourierTable fourierTable;		// of dimension numberOfFourierSamples;

private:
	
	void initCommon (kSound_windowShape windowShape, bool subtractFrameMean,
		bool wantSpectrum, integer fftInterpolationFactor);
	
public:
	
	void init (constSound input, double effectiveAnalysisWidth, double timeStep,
		kSound_windowShape windowShape, bool subtractFrameMean,
		bool wantSpectrum, integer fftInterpolationFactor);
	
	/*
		Initialise the object and use the sampling (x1, dx, nx) of the Sampled 
	*/
	void initWithSampled (constSound input, constSampled output, double effectiveAnalysisWidth,
		kSound_windowShape windowShape, bool subtractFrameMean,
		bool wantSpectrum, integer fftInterpolationFactor);
	
	VEC getFrame (integer iframe);

	void soundFrameToForwardFourierTransform ();

	void getPowerSpectrum (VEC const& powerspectrum);
	
	void getFrameChannelPowerSpectrum (integer ichannel);
	
	VEC getFourierSamplesVector () {
		Melder_assert (wantPowerSpectrum);
		return fourierSamples.get();
	}
};

autoSoundFrames SoundFrames_create (constSound input, constSampled output, double effectiveAnalysisWidth,
	kSound_windowShape windowShape, bool subtractFrameMean, 
	bool wantSpectrum, integer fftInterpolationFactor);

autoSoundFrames SoundFrames_create (constSound input, double effectiveAnalysisWidth,
	double timeStep, kSound_windowShape windowShape, bool subtractFrameMean,
	bool wantSpectrum, integer fftInterpolationFactor);

#endif /* _SoundFrames_h_ */

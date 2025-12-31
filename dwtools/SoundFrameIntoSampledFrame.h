#ifndef _SoundFrameIntoSampledFrame_h_
#define _SoundFrameIntoSampledFrame_h_
/* SoundFrameIntoSampledFrame.h
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
#include "Spectrum.h"

inline integer getSoundFrameSize2_odd (double approximatePhysicalAnalysisWidth, double samplingPeriod) {
	const double halfFrameDuration = 0.5 * approximatePhysicalAnalysisWidth;
	const integer halfFrameSamples = Melder_ifloor (halfFrameDuration / samplingPeriod);
	return 2 * halfFrameSamples + 1;
}

inline integer getSoundFrameSize2 (double physicalAnalysisWidth, double samplingPeriod) {
	Melder_assert (physicalAnalysisWidth > 0.0);
	Melder_assert (samplingPeriod > 0.0);
	return Melder_iround (physicalAnalysisWidth / samplingPeriod);
}

inline double getPhysicalAnalysisWidth2 (double effectiveAnalysisWidth, kSound_windowShape windowShape) {
	const double physicalAnalysisWidth = ( windowShape == kSound_windowShape::RECTANGULAR ||
		windowShape == kSound_windowShape::TRIANGULAR || windowShape == kSound_windowShape::HAMMING ||
		windowShape == kSound_windowShape::HANNING ? effectiveAnalysisWidth : 2.0 * effectiveAnalysisWidth )
	;
	return physicalAnalysisWidth;
}

#endif /* _SoundFrameIntoSampledFrame_h_ */

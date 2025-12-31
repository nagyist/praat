#ifndef _SampledIntoSampled_h_
#define _SampledIntoSampled_h_
/* SampledIntoSampled.h
 *
 * Copyright (C) 2024,2025 David Weenink
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

#include "SampledFrameIntoSampledFrame.h"

inline void Sampled_requireEqualSampling (constSampled me, constSampled thee) {
	Melder_assert (me && thee);
	Melder_require (my x1 == thy x1 && my nx == thy nx && my dx == thy dx,
		U"The sampling of ", me, U" and ", thee, U" should be equal.");
}

inline void SampledIntoSampled_assertEqualSampling (constSampled me,  constSampled thee) {
	Melder_assert (me && thee);
	Melder_assert (my x1 == thy x1 && my nx == thy nx && my dx == thy dx);
}

inline void SampledIntoSampled_requireEqualDomains (constSampled me,  constSampled thee) {
	Melder_assert (me && thee);
	Melder_require (my xmin == thy xmin && my xmax == thy xmax,
		U"The domains of ", me, U" and ", thee, U" should be equal.");
}

inline void SampledIntoSampled_assertEqualDomains (constSampled me,  constSampled thee) {
	Melder_assert (me && thee);
	Melder_assert (my xmin == thy xmin && my xmax == thy xmax);
}

inline void SampledIntoSampled_requireEqualDomainsAndSampling (constSampled me,  constSampled thee) {
	Melder_assert (me && thee);
	SampledIntoSampled_requireEqualDomains (me, thee);
	Sampled_requireEqualSampling (me, thee);
}

inline void SampledIntoSampled_assertEqualDomainsAndSampling (constSampled me,  constSampled thee) {
	Melder_assert (me && thee);
	SampledIntoSampled_assertEqualDomains (me, thee);
	SampledIntoSampled_assertEqualSampling (me, thee);
}

/*********** timing only *********/

void SampledIntoSampled_timeMultiThreading (double soundDuration);

#endif /* _SampledIntoSampled_h_ */


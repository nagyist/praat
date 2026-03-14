/* CCA_def.h
 * 
 * Copyright (C) 1993-2018 David Weenink
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

/*
 djmw 2001
 djmw 20020423 GPL header
 djmw 20031214 Added x/yLabels
 djmw 20060529 Added object version numbers.
 djmw 20080122 float -> double
 */


#define ooSTRUCT CCA
oo_DEFINE_CLASS (CCA, Daata)

	oo_INTEGER (numberOfCoefficients)
	oo_INTEGER (numberOfObservations)
	oo_OBJECT (Strings, 0, yLabels)
	oo_OBJECT (Strings, 0, xLabels)
	
	#if oo_READING
		oo_VERSION_UNTIL (1)
			oo_OBJECT (Eigen, 0, y)
			oo_OBJECT (Eigen, 0, x)
		oo_VERSION_ELSE
			oo_OBJECT (Eigen, 1, y)
			oo_OBJECT (Eigen, 1, x)
		oo_VERSION_END
	#else
		oo_OBJECT (Eigen, 1, y)
		oo_OBJECT (Eigen, 1, x)
	#endif
		
	#if oo_DECLARING
		void v1_info ()
			override;
	#endif

oo_END_CLASS (CCA)
#undef ooSTRUCT


/* End of file CCA_def.h */

/* Eigen.cpp
 *
 * Copyright (C) 1993-2020, 2026 David Weenink
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

#include "Eigen.h"
#include "MAT_numerics.h"
#include "NUMmachar.h"
#include "NUMselect.h"
#include "NUMlapack.h"
#include "NUM2.h"
#include "NUMcomplex.h"
#include "SVD.h"

#include "oo_DESTROY.h"
#include "Eigen_def.h"
#include "oo_COPY.h"
#include "Eigen_def.h"
#include "oo_EQUAL.h"
#include "Eigen_def.h"
#include "oo_CAN_WRITE_AS_ENCODING.h"
#include "Eigen_def.h"
#include "oo_WRITE_TEXT.h"
#include "Eigen_def.h"
#include "oo_READ_TEXT.h"
#include "Eigen_def.h"
#include "oo_WRITE_BINARY.h"
#include "Eigen_def.h"
#include "oo_READ_BINARY.h"
#include "Eigen_def.h"
#include "oo_DESCRIPTION.h"
#include "Eigen_def.h"

Thing_implement (Eigen, Daata, 1);

static void Graphics_ticks (Graphics g, double min, double max, bool hasNumber, bool hasTick, bool hasDottedLine, bool integers) {
	double range = max - min, scale = 1.0;

	if (range == 0.0) {
		return;
	} else if (range > 1.0) {
		while (range / scale > 10.0)
			scale *= 10.0;
		range /= scale;
	} else {
		while (range / scale < 10.0)
			scale /= 10.0;
		range *= scale;
	}

	const double dtick = ( range < 3.0 ? 0.5 : 1.0 ) * scale;
	double tick = dtick * Melder_roundDown (min / dtick);
	if (tick < min)
		tick += dtick;
	while (tick <= max) {
		const double xWC = integers ? round (tick) : tick;
		Graphics_markBottom (g, xWC, hasNumber, hasTick, hasDottedLine, nullptr);
		tick += dtick;
	}
}
	
double Eigen_getSpectralRadius (Eigen me) {
	double norm = NUMsum2 (my eigenvalues.get());
	if (my onlyReals)
		norm += NUMsum2 (my eigenvalues_im.get());
	return sqrt (norm);
}

static void Eigen_getEigenvectorsExtrema (Eigen me, double *min, double *max) {
	double min_re, max_re;
	NUMextrema_u (my eigenvectors.get().asvector(), & min_re, & max_re);
	if (! my onlyReals) {
		double min_im, max_im;
		NUMextrema_u (my eigenvectors_im.get().asvector(), & min_im, & max_im);
		min_re = std::min (min_re, min_im);
		max_re = std::max (max_re, max_im);
	}
	if (min)
		*min = min_re;
	if (max)
		*max = max_re;
}


void Eigen_init (Eigen me, integer numberOfEigenvalues, integer dimension) {
	my numberOfEigenvalues = numberOfEigenvalues;
	my dimension = dimension;
	my eigenvalues = zero_VEC (numberOfEigenvalues);
	my eigenvectors = zero_MAT (numberOfEigenvalues, dimension);
	my onlyReals = true;
}

void Eigen_initImaginaryParts (Eigen me) {
	my onlyReals = false;
	my eigenvalues_im = zero_VEC (my numberOfEigenvalues);
	my eigenvectors_im = zero_MAT (my numberOfEigenvalues, my dimension);
	my onlyReals = false;
}

autoEigen Eigen_create (integer numberOfEigenvalues, integer dimension) {
	try {
		autoEigen me = Thing_new (Eigen);
		Eigen_init (me.get(), numberOfEigenvalues, dimension);
		return me;
	} catch (MelderError) {
		Melder_throw (U"Eigen not created.");
	}
}

void Eigen_initFromSquareMAT (Eigen me, constMATVU const& mat, kMAT_TYPE matType, integer numberOfEigenvalues) {
	Melder_assert (numberOfEigenvalues >=1 && numberOfEigenvalues <= mat.nrow);
	Eigen_init (me, numberOfEigenvalues, mat.ncol);
	MAT_into_Eigen (mat, matType, me);
}

autoEigen Eigen_createFromSquareMAT (constMATVU const& mat, kMAT_TYPE matType, integer numberOfEigenvalues) {
	try {
		const integer dimension = mat.ncol;
		Melder_require (mat.nrow == dimension,
			U"The matrix is not square. Its number of rows and columns should be equal.");
		if (numberOfEigenvalues <= 0)
			numberOfEigenvalues = dimension;
		Melder_require (numberOfEigenvalues <= dimension,
			U"The number of eigenvalues is too large. It should be in the interval from 1 to ", dimension, U".");
		autoEigen me = Thing_new (Eigen);
		Eigen_initFromSquareMAT (me.get(), mat, matType, numberOfEigenvalues);
		return me;
	} catch (MelderError) {
		Melder_throw (U"Cannot create Eigen from ", kMAT_TYPE_getText (matType), U" matrix.");
	}
}

/*
	Solve: (A'A - lambda)x = 0 for eigenvalues lambda and eigenvectors x.
	svd(A) = UDV' => A'A = (UDV')'(UDV') = VD^2V'
	(VD^2V'-lambda)x = 0 => (D^2 - lambda)V'x = 0 => solution V'x = I => x = V
	Eigenvectors: the columns of the matrix V
	Eigenvalues: D_i^2
*/
void Eigen_initFromSquareRoot (Eigen me, constMATVU const& a) {
	Melder_require (a.nrow >= 1,
		U"The matrix must at least have one row.");
	const integer nsv = std::min (a.nrow, a.ncol);
	my dimension = a.ncol;
	autoSVD svd = SVD_createFromGeneralMatrix (a);
	/*
		Make sv's that are too small zero. These values occur automatically
		when the rank of A'A < a.ncol. This happens if, for
		example, numberOfRows <= a.ncol.
		(n points in  an n-dimensional space define maximally an n-1
		dimensional surface for which we maximally need an n-1 dimensional
		basis.)
	*/
	const integer numberOfZeroed = SVD_zeroSmallSingularValues (svd.get(), 0.0);
	const integer numberOfEigenvalues = nsv - numberOfZeroed;

	Eigen_init (me, numberOfEigenvalues, a.ncol);
	integer k = 0;
	for (integer i = 1; i <= nsv; i ++)
		if (svd -> d [i] > 0.0) {
			my eigenvalues [++ k] = svd -> d [i] * svd -> d [i];
			for (integer j = 1; j <= a.ncol; j ++)
				my eigenvectors [k] [j] = svd -> v [j] [i];
		}
	/*
		Eigenvalues/eigenvectors already by the svd
	*/
}

autoEigen Eigen_createFromSquareRoot (constMATVU const& a, integer numberOfEigenvalues) {
	Melder_require (a.nrow >= a.ncol,
		U"The number of rows must be greater than or equal to the number of columns.");
	if (numberOfEigenvalues <= 0)
		numberOfEigenvalues = a.ncol;
		
	Melder_require (numberOfEigenvalues <= a.ncol,
		U"The number of eigenvalues cannot be larger than the number of columns.");
	try {
		autoSVD svd = SVD_createFromGeneralMatrix (a);
		/*
			Make sv's that are too small zero. These values occur automatically
			when the rank of A'A < a.ncol. This happens if, for
			example, numberOfRows <= a.ncol.
			(n points in  an n-dimensional space define maximally an n-1
			dimensional surface for which we maximally need an n-1 dimensional
			basis.)
		*/
		const integer numberOfZeroed = SVD_zeroSmallSingularValues (svd.get(), 0.0);

		autoEigen me = Eigen_create (numberOfEigenvalues, a.ncol);
		integer k = 0;
		for (integer i = 1; i <= numberOfEigenvalues; i ++) {
			my eigenvalues [++ k] = svd -> d [i] * svd -> d [i];
			for (integer j = 1; j <= my dimension; j ++)
					my eigenvectors [k] [j] = svd -> v [j] [i];
		}
		return me;
	} catch (MelderError) {
		Melder_throw (U"Cannot create Eigen from square root matrix.");
	}
}

void Eigen_initFromSquareRootPair (Eigen me, constMAT a, constMAT b) {
	Melder_require (a.ncol == b.ncol,
		U"The numbers of columns should be equal, not ", a.ncol, U" and ", b.ncol, U".");
	// Eigen has not been inited yet.
	integer k, ll, m = a.nrow, n = a.ncol, p = b.nrow;
	integer lwork = std::max (std::max (3 * n, m), p) + n, info;

	/*	Melder_assert (numberOfRows >= numberOfColumns || numberOfRows_b >= numberOfColumns);*/

	my dimension = a.ncol;

	autoVEC alpha = raw_VEC (n);
	autoVEC beta = raw_VEC (n);
	autoVEC work = raw_VEC (lwork);
	autoINTVEC iwork = zero_INTVEC (n);
	autoMAT q = raw_MAT (n, n);
	autoMAT ac = transpose_MAT (a);
	autoMAT bc = transpose_MAT (b);

	(void) NUMlapack_dggsvd_ ("N", "N", "Q", m, n, p, & k, & ll,
		& ac [1] [1], m, & bc [1] [1], p, & alpha [1], & beta [1], nullptr, m,
		nullptr, p, & q [1] [1], n, work.asArgumentToFunctionThatExpectsZeroBasedArray(), iwork.asArgumentToFunctionThatExpectsZeroBasedArray(), & info);
	Melder_require (info == 0,
		U"LAPACK dggsvd fails with code ", info, U".");
	/*
		Calculate the eigenvalues (alpha [i] / beta [i])^2 and store in alpha [i].
	*/
	for (integer i = k + 1; i <= k + ll; i ++) {
		const double t = alpha [i] / beta [i];
		alpha [i] = t * t;
	}
	/*
		Deselect the eigenvalues < eps * max_eigenvalue by making them undefined
	*/
	const double maxEigenvalue = num::NUMmax_e (alpha.part (k + 1, k + ll));
	integer numberOfDeselected = 0;
	for (integer i = k + 1; i <= k + ll; i ++) {
		if (alpha [i] < NUMfpp -> eps * maxEigenvalue) {
			numberOfDeselected ++;
			alpha [i] = undefined;
		}
	}

	Melder_require (ll - numberOfDeselected > 0,
		U"No eigenvectors can be found. Matrix too singular.");

	Eigen_init (me, ll - numberOfDeselected, a.ncol);

	integer numberOfEigenvalues = 0;
	for (integer i = k + 1; i <= k + ll; i ++) {
		if (isundef (alpha [i]))
			continue;
		my eigenvalues [++ numberOfEigenvalues] = alpha [i];
		for (integer j = 1; j <= a.ncol; j ++)
			my eigenvectors [numberOfEigenvalues] [j] = q [i] [j];
	}

	Eigen_sort (me, false);

	MATnormalizeRows_inplace (my eigenvectors.get(), 2.0, 1.0);
}

void Eigen_initFromSymmetricMatrix (Eigen me, constMATVU const& a) {
	Melder_assert (a.ncol == a.nrow);
	if (NUMisEmpty (my eigenvectors.get()))   // ppgb: BUG dubious logic
		Eigen_init (me, a.ncol, a.ncol);
	else
		Melder_assert (my eigenvectors.nrow == my eigenvectors.ncol && a.ncol == my eigenvectors.ncol);
	MAT_getEigenSystemFromSymmetricMatrix_preallocated (my eigenvectors.get(), my eigenvalues.get(), a, false);
}

void Eigen_fromSymmetricTridiagonal (Eigen me, constVEC const& diagonal, constVEC const& offDiagonal) {
	Melder_assert (diagonal.size == my dimension);
	Melder_assert (offDiagonal.size >= my dimension - 1);
	const char *jobz = "V", *safe = "S";
	const char *range = ( my numberOfEigenvalues < my dimension ? "I" : "A" );
	const integer il = 1, iu = my numberOfEigenvalues, evLeadingDimension = my dimension;
	const integer lwork = 20 * my dimension, liwork = 10 * my dimension;
	const double vl = 0.0, vu = 0.0, abstol = 2.0 * dlamch_ (safe);
	autoVEC work = raw_VEC (lwork);
	autoINTVEC iwork = raw_INTVEC (liwork), isuppz = raw_INTVEC (2 * my numberOfEigenvalues);
	autoVEC diagonalCopy = copy_VEC (diagonal), offDiagonalCopy = copy_VEC (offDiagonal);
	integer numberOfEigenvaluesFound, info = 0;
	(void) NUMlapack_dstevr (jobz, range, my dimension, & diagonalCopy [1], & offDiagonalCopy [1],
		vl, vu, il, iu, abstol, & numberOfEigenvaluesFound, & my eigenvalues [1],
		& my eigenvectors [1][1], evLeadingDimension, & isuppz [1], & work [1], lwork,
		& iwork [1], liwork, & info);
	Melder_require (info == 0,
		U"LAPACK dstevr fails with code ", info, U".");
	Melder_require (numberOfEigenvaluesFound == my numberOfEigenvalues,
		U"The number of eigenvalues found (", numberOfEigenvaluesFound, U") differs from the number "
		"of eigenvalues wanted (", my numberOfEigenvalues, U").");
}

integer Eigen_getNumberOfEigenvectors (Eigen me) {
	return my numberOfEigenvalues;
}

double Eigen_getEigenvectorElement (Eigen me, integer ivec, integer element) {
	if (ivec > my numberOfEigenvalues || element < 1 || element > my dimension)
		return undefined;
	return my eigenvectors [ivec] [element];
}

autoVEC Eigen_listEigenvalues (Eigen me) {
	try {
		return copy_VEC (my eigenvalues.get());
	} catch (MelderError) {
		Melder_throw (me, U": cannot list eigenvalues.");
	}
}

autoVEC Eigen_getEigenvector (Eigen me, integer ivec) {
	try {
		Melder_require (ivec >= 1 and ivec <= my numberOfEigenvalues,
			U"The eigenvector number should be in the interval from 1 to ", my numberOfEigenvalues, U".");
		autoVEC eigenvector = copy_VEC (my eigenvectors.row (ivec));
		return eigenvector;
	} catch (MelderError) {
		Melder_throw (me, U": cannot get eigenvector.");
	}
}

autoCOMPVEC Eigen_getEigenvector_complex (Eigen me, integer ivec) {
	try {
		Melder_require (ivec >= 1 and ivec <= my numberOfEigenvalues,
			U"The eigenvector number should be in the interval from 1 to ", my numberOfEigenvalues, U".");
		autoCOMPVEC eigenvector = zero_COMPVEC (my dimension);
		for (integer i = 1; i <= my dimension; i ++) {
			eigenvector [i].real (my eigenvectors [ivec] [i]);
			if (! my onlyReals)
				eigenvector [i].imag (my eigenvectors_im [ivec] [i]);
		}
		return eigenvector;
	} catch (MelderError) {
		Melder_throw (me, U": cannot get complex eigenvector.");
	}
}

integer Eigen_getDimensionOfComponents (Eigen me) {
	return my dimension;
}

double Eigen_getSumOfEigenvalues (Eigen me, integer from, integer to) {
	if (from < 1)
		from = 1;
	if (to < 1)
		to = my numberOfEigenvalues;
	if (to > my numberOfEigenvalues || from > to)
		return undefined;	
	return NUMsum (my eigenvalues.part (from, to));
}

double Eigen_getCumulativeContributionOfComponents (Eigen me, integer from, integer to) {
	longdouble partial = 0.0, sum = 0.0;
	if (to == 0)
		to = my numberOfEigenvalues;
	if (from > 0 && to <= my numberOfEigenvalues && from <= to) {
		for (integer i = 1; i <= my numberOfEigenvalues; i ++) {
			sum += my eigenvalues [i];
			if (i >= from && i <= to)
				partial += my eigenvalues [i];
		}
	}
	return sum > 0.0 ? double (partial / sum) : 0.0;
}

integer Eigen_getDimensionOfFraction (Eigen me, double fraction) {
	const double sum = Eigen_getSumOfEigenvalues (me, 1, my numberOfEigenvalues);

	if (sum == 0.0)
		return 1;

	integer n = 1;
	longdouble p = my eigenvalues [1];
	while (p / sum < fraction && n < my numberOfEigenvalues) {
		p += my eigenvalues [++ n];
	}
	return n;
}

void Eigen_sort_special (Eigen me, bool sortAscending) {
	/*
		Precondition: The eigenvalues are always sorted.
		Sort is reverse.
	*/
	Melder_require (my onlyReals,
		U"Don't know how to sort complex numbers.");
	bool isAscending = ( my eigenvalues [1] <= my eigenvalues [my numberOfEigenvalues] );
	if (isAscending != sortAscending) { // reverse 
		NUMreverseOrder<VEC> (my eigenvalues.get());
		for (integer i = 1; i <= my numberOfEigenvalues / 2; i ++) {
			const integer ilast = my numberOfEigenvalues + 1 - i;
			for (integer j = 1; j <= my dimension; j ++)
				std::swap (my eigenvectors [i] [j], my eigenvectors [ilast] [j]);
		}
	}
}

void Eigen_sort (Eigen me, bool sortAscending) {
	try {
		/*
			Almost inplace sorting of eigenvalues and eigenvectors together. 
			We need only 2 * numberOfEigenvalues extra memory.
		*/
		Melder_require (my onlyReals,
			U"Don't know how to sort complex numbers.");
		autoINTVEC keys = from_to_by_INTVEC (1_integer, my numberOfEigenvalues, 1_integer);
		autoVEC sortedColumn = raw_VEC (my numberOfEigenvalues);
		NUMsortTogether <VEC, INTVEC> (my eigenvalues.get(), keys.get());
		if (! sortAscending) {
			NUMreverseOrder<INTVEC> (keys.get());
			NUMreverseOrder<VEC> (my eigenvalues.get());
		}
			for (integer i = 1; i <= my numberOfEigenvalues / 2; i ++) {
				std::swap (keys [i], keys [my numberOfEigenvalues + 1 - i]); // reverse
				std::swap (my eigenvalues [i], my eigenvalues [my numberOfEigenvalues + 1 - i]);
			}
		for (integer icol = 1; icol <= my dimension; icol ++) {
			const integer rowIndex = keys [icol];
			for (integer irow = 1; irow <= my numberOfEigenvalues; irow ++)
				sortedColumn [irow] = my eigenvectors [rowIndex] [icol];
			my eigenvectors.column (icol)  <<=  sortedColumn.get();
		}
	} catch (MelderError) {
		Melder_throw (me, U"Could not sort eigenvectors.");
	}
}

void Eigen_invertEigenvector (Eigen me, integer ivec) {
	Melder_require (ivec >= 1 and ivec <= my numberOfEigenvalues,
		U"The eigenvector number should be in the interval from 1 to ", my numberOfEigenvalues, U".");
	
	for (integer j = 1; j <= my dimension; j ++) {
		my eigenvectors [ivec] [j] = - my eigenvectors [ivec] [j];
	}
}

void Eigen_drawEigenvalues (Eigen me, Graphics g, integer first, integer last, double ymin, double ymax, bool fractionOfTotal, bool cumulative, double size_mm, conststring32 mark, bool garnish) {
	double scale = 1.0, sumOfEigenvalues = 0.0;
	if ((first == 0 && last == 0) || last <= first) {
		first = 1;
		last = my numberOfEigenvalues;		
	}
	Melder_require (first >= 1 && last <= my numberOfEigenvalues && first < last,
		U"The first and last index must be within the interval [1,", my numberOfEigenvalues, U"].");
	const double xmin = first - 0.5;
	const double xmax = last + 0.5;
	if (my onlyReals && (fractionOfTotal || cumulative)) {
		sumOfEigenvalues = Eigen_getSumOfEigenvalues (me, 0, 0);
		if (sumOfEigenvalues <= 0.0)
			sumOfEigenvalues = 1.0;
		scale = sumOfEigenvalues;
	}
	if (ymax <= ymin) {
		ymax = Eigen_getSumOfEigenvalues (me, ( cumulative ? 1 : first ), first) / scale;
		ymin = Eigen_getSumOfEigenvalues (me, ( cumulative ? 1 : last ), last) / scale;
		if (ymin > ymax)
			std::swap (ymin, ymax);
		if (ymin == ymax) { // only one eigenvalue
			ymin -= 0.1 * ymin;
			ymax += 0.1 * ymax;
		}
	}
	Graphics_setInner (g);
	Graphics_setWindow (g, xmin, xmax, ymin, ymax);
	for (integer i = first; i <= last; i ++) {
		const double accu = Eigen_getSumOfEigenvalues (me, (cumulative ? 1 : i), i);
		Graphics_mark (g, i, accu / scale, size_mm, mark);
	}
	Graphics_unsetInner (g);
	if (garnish) {
		Graphics_drawInnerBox (g);
		Graphics_textLeft (g, true, (fractionOfTotal ?
			(cumulative ? U"Cumulative fractional eigenvalue" : U"Fractional eigenvalue" ) :
			( cumulative ? U"Cumulative eigenvalue" : U"Eigenvalue" ) ));
		Graphics_ticks (g, first, last, true, true, false, true);
		Graphics_marksLeft (g, 2, true, true, false);
		Graphics_textBottom (g, true, U"Index");
	}
}

void Eigen_drawEigenvector (Eigen me, Graphics g, integer ivec, integer first, integer last,
	double ymin, double ymax, bool weigh, double size_mm, conststring32 mark, bool connect, char32 **rowLabels, bool garnish)
{
	if (ivec < 1 || ivec > my numberOfEigenvalues)
		return;

	if (first >= last) {
		first = 1;
		last = my dimension;
	}
	if (first < 1 || first > my dimension || last < 1 || last > my dimension)
		return;
	const double w = ( weigh && my onlyReals ? sqrt (my eigenvalues [ivec]) : 1.0 );
	autoVEC vec_re = copy_VEC (my eigenvectors.row (ivec));
	vec_re.get()  *=  w;
	autoVEC vec_im;
	if (! my onlyReals) {
		vec_im = copy_VEC (my eigenvectors_im.row (ivec));
		vec_im.get()  *=  w;
	}
	if (ymax == ymin) {
		Eigen_getEigenvectorsExtrema (me, & ymin,  & ymax);
		ymax *= w;
		ymin *= w;
	}

	// If ymax < ymin the eigenvector will automatically be drawn inverted.

	Graphics_setInner (g);
	const double xmin = first, xmax = last;

	Graphics_setWindow (g, xmin, xmax, ymin, ymax);

	for (integer i = first; i <= last; i ++) {
		Graphics_mark (g, i, vec_re [i], size_mm, mark);
		if (connect && i > first)
			Graphics_line (g, i - 1.0, vec_re [i - 1], i, vec_re [i]);
	}
	if (! my onlyReals) {
		for (integer i = first; i <= last; i ++) {
			if (connect && i > first)
				Graphics_line (g, i - 1.0, vec_im [i - 1], i, vec_im [i]);
		}
	}
	Graphics_unsetInner (g);
	if (garnish) {
		Graphics_markBottom (g, first, false, true, false, rowLabels ? rowLabels [first] : Melder_integer (first));
		Graphics_markBottom (g, last, false, true, false, rowLabels ? rowLabels [last] : Melder_integer (last));
		Graphics_drawInnerBox (g);
		if (ymin * ymax < 0.0)
			Graphics_markLeft (g, 0.0, true, true, true, nullptr);
		Graphics_marksLeft (g, 2, true, true, false);
		if (! rowLabels)
			Graphics_textBottom (g, true, U"Element number");
	}
}

void Eigens_alignEigenvectors (OrderedOf<structEigen>* me) {
	if (my size < 2)
		return;

	const Eigen e1 = my at [1];
	const integer nev1 = e1 -> numberOfEigenvalues;
	const integer dimension = e1 -> dimension;

	for (integer i = 2; i <= my size; i ++)
		Melder_require (my at [i] -> dimension == dimension,
			U"The dimension of the eigenvectors should be equal (offending object is ",  i, U").");
	/*
		Correlate eigenvectors.
		If r < 0 then mirror the eigenvector.
	*/
	for (integer i = 2; i <= my size; i ++) {
		const Eigen e2 = my at [i];
		for (integer j = 1; j <= std::min (nev1, e2 -> numberOfEigenvalues); j ++) {
			const double ip = NUMinner (e1 -> eigenvectors.row (j), e2 -> eigenvectors.row (j));
			if (ip < 0.0)
				for (integer k = 1; k <= dimension; k ++)
					e2 -> eigenvectors [j] [k] = - e2 -> eigenvectors [j] [k];
		}
	}
}

static autoVEC Eigens_getAnglesBetweenSubspaces (Eigen me, Eigen thee, integer ivec_from, integer ivec_to) {
	const integer numberOfVectors = ivec_to - ivec_from + 1;

	const integer nmin = std::min (my numberOfEigenvalues, thy numberOfEigenvalues);

	Melder_require (my dimension == thy dimension,
		U"The eigenvectors should have equal dimensions.");
	Melder_require (ivec_from > 0 && ivec_from <= ivec_to && ivec_to <= nmin,
		U"Eigenvector range too large.");
	/*
		Algorithm 12.4.3 Golub & van Loan
		Because we deal with eigenvectors we don't have to do the QR decomposition,
			the columns in the Q's are the eigenvectors.
		Compute C.
	*/
	autoVEC angles_degrees = raw_VEC (numberOfVectors);

	autoMAT c = mul_MAT (my eigenvectors.horizontalBand (ivec_from, ivec_to),
			thy eigenvectors.horizontalBand (ivec_from, ivec_to).transpose());
	autoSVD svd = SVD_createFromGeneralMatrix (c.get());
	for (integer i = 1; i <= numberOfVectors; i ++)
		angles_degrees [i] = acos (svd -> d [i]) * (180.0 / NUMpi);

	return angles_degrees;
}

double Eigens_getAngleBetweenEigenplanes_degrees (Eigen me, Eigen thee) {
	autoVEC angles_degrees = Eigens_getAnglesBetweenSubspaces (me, thee, 1, 2);
	return angles_degrees [2];
}


/* End of file Eigen.cpp */

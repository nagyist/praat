/* MAT_numerics.cpp
 *
 * Copyright (C) 2018-2020 David Weenink
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

#include "NUMlapack.h"
#include "MAT_numerics.h"
#include "SVD.h"
#include "PAIRWISE_SUM.h"

#include "enums_getText.h"
#include "MATTypes_enums.h"
#include "enums_getValue.h"
#include "MATTypes_enums.h"

autoEigen MAT_to_Eigen (constMAT const& mat, kMAT_TYPE matType, integer numberOfEigenvalues) {
	try {
		Melder_assert (mat.nrow == mat.ncol);
		Melder_assert (numberOfEigenvalues>= 1 && numberOfEigenvalues <= mat.ncol);
		autoEigen me = Eigen_create (numberOfEigenvalues, mat.ncol);
		MAT_into_Eigen (mat, matType, me.get(), false);
		return me;
	} catch (MelderError) {
		Melder_throw (U"MAT_to_Eigen did not succeed.");
	}
}

void symmetricTridiagonal_into_Eigen (VEC const& diagonal, VEC const& offDiagonal, Eigen me) {
	Melder_assert (diagonal.size == my dimension);
	Melder_assert (offDiagonal.size >= my dimension - 1);
	try {
		integer info = 0;
		const char *jobz = "V", *safe = "S";
		const char *range = ( my numberOfEigenvalues < my dimension ? "I" : "A" );
		const integer il = 1, iu = my numberOfEigenvalues, evLeadingDimension = my dimension;
		const integer lwork = 20 * my dimension, liwork = 10 * my dimension;
		const double vl = 0.0, vu = 0.0, abstol = 2.0 * dlamch_ (safe);
		autoVEC work = raw_VEC (lwork);
		autoINTVEC iwork = raw_INTVEC (liwork), isuppz = raw_INTVEC (2 * my numberOfEigenvalues);
		/*
			Eigenvalues are returned in ascending order.
		*/
		integer numberOfEigenvaluesFound;
		(void) NUMlapack_dstevr (jobz, range, my dimension, & diagonal [1], & offDiagonal [1],
			vl, vu, il, iu, abstol, & numberOfEigenvaluesFound, & my eigenvalues [1],
			& my eigenvectors [1][1], evLeadingDimension, & isuppz [1], & work [1], lwork,
			& iwork [1], liwork, & info);
		Melder_require (info == 0,
			U"NUMlapack_dstevr fails with code ", info, U".");
		Melder_require (numberOfEigenvaluesFound == my numberOfEigenvalues,
			U"The number of eigenvalues found (", numberOfEigenvaluesFound, U") differs from the number "
			"of eigenvalues wanted (", my numberOfEigenvalues, U").");
		Eigen_sort_special (me, false);
	} catch (MelderError) {
		Melder_throw (U"symmetricTridiagonal_into_Eigen: not succesful.");
	}
}

void squareRoot_into_Eigen (MAT const& mat, Eigen me) {
	Melder_assert (mat.ncol <= mat.nrow);
	Melder_assert (mat.ncol == my dimension);
	autoSVD svd = SVD_createFromGeneralMatrix (mat);
	/*
		Make sv's that are too small zero. These values occur automatically
		when the rank of A'A < A.ncol. This happens if, for
		example, numberOfRows <= A.ncol.
		(n points in  an n-dimensional space define maximally an n-1
		dimensional surface for which we maximally need an n-1 dimensional
		basis.)
	*/
	const integer numberOfZeroed = SVD_zeroSmallSingularValues (svd.get(), 0.0);
	const integer numberOfEigenvalues = mat.ncol - numberOfZeroed;

	Eigen_init (me, numberOfEigenvalues, mat.ncol);
	integer k = 0;
	for (integer i = 1; i <= numberOfEigenvalues; i ++)
		if (svd -> d [i] > 0.0) {
			my eigenvalues [++ k] = svd -> d [i] * svd -> d [i];
			for (integer j = 1; j <= mat.ncol; j ++)
				my eigenvectors [k] [j] = svd -> v [j] [i];
		}
	/*
		Eigenvalues/eigenvectors already by the svd
	*/
	
}

void MAT_into_Eigen (constMATVU const& mat, kMAT_TYPE matType, Eigen me, bool sortAscending) {
	Melder_assert (mat.nrow == mat.ncol);
	Melder_assert (my dimension == mat.ncol);
	try {
		integer info, ilow = 1, iup = my numberOfEigenvalues;
		if (! sortAscending) {
			ilow = mat.ncol - my numberOfEigenvalues + 1;
			iup = ilow + my numberOfEigenvalues - 1;
		}
		const bool sortAscending = false;
		if (matType == kMAT_TYPE::SYMMETRIC) {
			const integer lwork = 3 * my dimension;
			autoVEC work = raw_VEC (lwork);
			/*
				Eigenvalues are returned in ascending order
			*/
			autoMAT matCopy = copy_MAT (mat); // symmetric, no need to transpose
			(void) NUMlapack_dsyev_ ("V", "U", my dimension, & matCopy [1] [1], my dimension, & my eigenvalues [1],
				& work [1], lwork, & info);
			Melder_require (info == 0,
				U"NUMlapack_dsyev fails with code ", info, U".");
			my eigenvectors.part (1, my numberOfEigenvalues, 1, mat.ncol)  <<= matCopy.part (ilow, iup, 1, mat.ncol);
			Eigen_sort_special (me, sortAscending);
		} else if (matType == kMAT_TYPE::SYMMETRIC_TRIDIAGONAL) {
			autoVEC diagonal = raw_VEC (my dimension);
			autoVEC offDiagonal = raw_VEC (my dimension - 1);
			for (integer i = 1; i <= my dimension; i ++)
				diagonal [i] = mat [i] [i];
			for (integer i = 1; i < my dimension; i ++)
				offDiagonal [i] = mat [i] [i + 1];
			Eigen_initFromSymmetricTridiagonal (me, diagonal.get(), offDiagonal.get(), sortAscending);
		} else if (matType == kMAT_TYPE::GENERAL) {
			Eigen_initImaginaryParts (me);
			/*
				Query for the amount of workspace needed.
			*/
			double wtmp [3];
			integer lwork = -1;
			autoMAT eigenvectors = raw_MAT (mat.nrow, mat.ncol);
			autoMAT matTransposed = transpose_MAT (mat); // because mat will be destroyed by NUMlapack_dgeev
			autoVEC eigenvalues_re = raw_VEC (mat.ncol), eigenvalues_im = raw_VEC (mat.ncol);
			NUMlapack_dgeev_ ("N", "V", mat.ncol, & matTransposed [1] [1], mat.ncol, & eigenvalues_re [1], & eigenvalues_im [1],
				nullptr, mat.ncol, & eigenvectors [1] [1], mat.ncol, & wtmp [1], lwork, & info);
			lwork = Melder_iceiling (wtmp [1]);
			autoVEC work = raw_VEC (lwork);
			NUMlapack_dgeev_ ("N", "V", mat.ncol, & matTransposed [1] [1], mat.ncol, & eigenvalues_re [1], & eigenvalues_im [1],
				nullptr, mat.ncol, & eigenvectors [1] [1], mat.ncol, & work [1], lwork, & info);
			Melder_require (info == 0,
				U"NUMlapack_dgeev fails with code ", info, U".");
			/*
				The following eigenvector extraction is based on the fact that the
				resulting eigenvalues are either real or occur in pairs (a+ib, a-ib).
				A real eigenvalue uses one column in the output eigenvectors matrix,
				a complex eigenvalue uses two columns with the real and imaginary parts
				that represent two eigenvectors:
					first eigenvector:  column(k) + i column (k+1)
					second eigenvector: column(k) - i column (k+1)
				Because LAPACK uses Fortran column major storage the eigenvectors can be accessed
				in C++ row-wise (and will also be stored in rows!).
			*/
			integer ivec = 1;
			while (ivec <= mat.ncol) {
				my eigenvalues [ivec] = eigenvalues_re [ivec];
				my eigenvalues_im [ivec] = eigenvalues_im [ivec];
				my eigenvectors.row (ivec)  <<=  eigenvectors.row (ivec); // Fortran column == C++ row!
				if (eigenvalues_im [ivec] != 0.0) {
					my eigenvectors_im.row (ivec)  <<=  eigenvectors.row (ivec + 1);
					my eigenvectors.row (ivec + 1)  <<=  eigenvectors.row (ivec);
					ivec ++;
					my eigenvalues [ivec] = eigenvalues_re [ivec];
					my eigenvalues_im [ivec] = eigenvalues_im [ivec];
					my eigenvectors_im.row (ivec)  <<=  eigenvectors.row (ivec);
					for (integer i = 1; i <= my dimension; i ++) 
						my eigenvectors_im [ivec] [i] = - my eigenvectors_im [ivec] [i];
				}
				ivec ++;
			}
		}
	} catch (MelderError) {
		Melder_throw (U"MAT_into_Eigen did not succeed.");
	}
}


void MAT_getEigenSystemFromSymmetricMatrix_preallocated (MAT eigenvectors, VEC eigenvalues, constMATVU const& m, bool sortAscending) {
	Melder_assert (m.nrow == m.ncol);
	Melder_assert (eigenvalues.size == m.ncol);
	Melder_assert (eigenvectors.nrow == eigenvectors.ncol);
	Melder_assert (m.nrow == eigenvectors.nrow);

	integer lwork = -1, info, ncol = m.ncol;
	double wt;
	
	eigenvectors  <<=  m;
	/*
		0. No need to transpose a because it is a symmetric matrix
		1. Query for the size of the work array
	*/
	(void) NUMlapack_dsyev_ ("V", "U", ncol, & eigenvectors [1] [1], ncol, & eigenvalues [1], & wt, lwork, & info);

	lwork = Melder_iceiling (wt);
	autoVEC work = raw_VEC (lwork);
	/*
		2. Calculate the eigenvalues and eigenvectors (row-wise)
	*/
	(void) NUMlapack_dsyev_ ("V", "U", ncol, & eigenvectors [1] [1], ncol, & eigenvalues [1], & work [1], lwork, & info);
	Melder_require (info == 0,
		U"NUMlapack_dsyev code = ", info, U").");
	/*
		3. Eigenvalues are returned in ascending order
	*/
	if (! sortAscending) {
		for (integer i = 1; i <= m.ncol / 2; i ++) {
			const integer ilast = m.ncol - i + 1;
			std::swap (eigenvalues [i], eigenvalues [ilast]);
			for (integer j = 1; j <= m.ncol; j ++)
				std::swap (eigenvectors [i] [j], eigenvectors [ilast] [j]);
		}
	}
}

void MAT_getEigenSystemFromSymmetricMatrix (constMAT a, autoMAT *out_eigenvectors, autoVEC *out_eigenvalues, bool sortAscending) {
	Melder_assert (a.nrow == a.ncol);	
	autoVEC eigenvalues = raw_VEC (a.nrow);
	autoMAT eigenvectors = raw_MAT (a.nrow, a.ncol);	
	
	MAT_getEigenSystemFromSymmetricMatrix_preallocated (eigenvectors.get(), eigenvalues.get(), a, sortAscending);
	
	if (out_eigenvectors)
		*out_eigenvectors = eigenvectors.move ();
	if (out_eigenvalues)
		*out_eigenvalues = eigenvalues.move ();
}

void MAT_asPrincipalComponents_preallocated (MATVU result, constMATVU const& m, integer numberOfComponents) {
	Melder_assert (numberOfComponents  > 0 && numberOfComponents <= m.ncol);
	Melder_assert (result.nrow == m.nrow && result.ncol == numberOfComponents);
	autoSVD svd = SVD_createFromGeneralMatrix (m);
	mul_MAT_out (result, m, svd -> v.verticalBand (1, result.ncol));
}

autoMAT MAT_asPrincipalComponents (constMATVU m, integer numberOfComponents) {
	Melder_assert (numberOfComponents  > 0 && numberOfComponents <= m.ncol);
	autoMAT result = raw_MAT (m.nrow, numberOfComponents);
	MAT_asPrincipalComponents_preallocated (result.get(), m, numberOfComponents);
	return result;
}

void MATpseudoInverse (MATVU const& target, constMATVU const& mat, double tolerance) {
	Melder_assert (target.nrow == mat.ncol && target.ncol == mat.nrow);
	autoSVD me = SVD_createFromGeneralMatrix (mat);
	(void) SVD_zeroSmallSingularValues (me.get(), tolerance);
	for (integer irow = 1; irow <= target.nrow; irow ++) {
		for (integer icol = 1; icol <= target.ncol; icol ++) {
			PAIRWISE_SUM (
				longdouble, sum,
				integer, mat.ncol,
				integer k = 1,
				my d [k] == 0.0 ? 0.0 : my v [irow] [k] * my u [icol] [k] / my d [k],
				k += 1
			)
			target [irow] [icol] = double (sum);
		}
	}
}

autoMAT newMATpseudoInverse (constMATVU const& mat, double tolerance) {
	autoMAT result = raw_MAT (mat.ncol, mat.nrow);
	MATpseudoInverse (result.all(), mat, tolerance);
	return result;
}

autoMAT MAT_createKacSylvester (integer dimension) {
	try {
		autoMAT m = zero_MAT (dimension, dimension);
		for (integer i = 1; i <= dimension - 1; i ++) {
			m [i] [i+1] = dimension - i;
			m [i+1] [i] = i;
		}
		return m;
	} catch (MelderError) {
		Melder_throw (U"KacSylvester not created.");
	}
}

/* End of file MAT_numerics.cpp */

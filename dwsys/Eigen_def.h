/* Eigen_def.h
 *
 * Copyright (C) 1993-2018, 2026 David Weenink
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
	Eigen represents the eigenvalues and eigenvectors of a square matrix.
	For a general square matrix some of the eigenvalues will be complex numbers
	and also some of the eigenvectors will be complex vectors.
	Most matrices we use are symmetric, which means that the
	eigenvalues and eigenvectors are real numbers.
	For these cases we don't need the imaginary parts:
	we don't read and write them.
 */

#define ooSTRUCT Eigen
oo_DEFINE_CLASS (Eigen, Daata)

	oo_INTEGER (numberOfEigenvalues)
	oo_INTEGER (dimension)
	oo_VEC (eigenvalues, numberOfEigenvalues)
	oo_MAT (eigenvectors, numberOfEigenvalues, dimension)
	
	#if oo_READING
		oo_VERSION_UNTIL (1)
			onlyReals = true;
		oo_VERSION_ELSE
			oo_BOOLEAN (onlyReals)
			if (! onlyReals) {
				oo_VEC (eigenvalues_im, numberOfEigenvalues)
				oo_MAT (eigenvectors_im, numberOfEigenvalues, dimension)
			}
		oo_VERSION_END
	#else
		oo_BOOLEAN (onlyReals) // convenience
		#if oo_WRITING || oo_COMPARING
			if (! onlyReals) {
				oo_VEC (eigenvalues_im, numberOfEigenvalues)
				oo_MAT (eigenvectors_im, numberOfEigenvalues, dimension)
			}
		#else	
			oo_VEC (eigenvalues_im, numberOfEigenvalues)
			oo_MAT (eigenvectors_im, numberOfEigenvalues, dimension)
		#endif
	#endif

oo_END_CLASS (Eigen)

#undef ooSTRUCT

#define ooSTRUCT Eigen2
oo_DEFINE_CLASS (Eigen2, Daata)

	oo_INTEGER (numberOfEigenvalues)
	oo_INTEGER (dimension)
	
	#if oo_READING
		oo_VERSION_UNTIL (1)
			/*
				Convert the real vector and matrix in the file to complex.
			*/
			autoVEC evals = raw_VEC (numberOfEigenvalues);
			autoMAT evecs = raw_MAT (numberOfEigenvalues, dimension);
			#if oo_READING_BINARY
				evals = vector_readBinary_r64 (numberOfEigenvalues, _filePointer_);
				evecs = matrix_readBinary_r64 (numberOfEigenvalues, dimension, _filePointer_);
			#else
				evals = vector_readText_r64 (numberOfEigenvalues, _textSource_, "eigenvalues");
				evecs = matrix_readText_r64 (numberOfEigenvalues, dimension, _textSource_, "eigenvectors");
			#endif
			for (integer i = 1; i <= numberOfEigenvalues; i ++) {
				eigenvalues [i] .real (evals [i]);
				for (integer j = 1; j <= dimension; j ++)
					eigenvectors [i] [j] .real (evecs [i] [j]);
			}
			onlyReals = true;
		oo_VERSION_ELSE
			oo_COMPVEC (eigenvalues, numberOfEigenvalues)
			oo_COMPMAT (eigenvectors, numberOfEigenvalues, dimension)
			oo_BOOLEAN (onlyReals) // commodity
		oo_VERSION_END
	#else
		oo_COMPVEC (eigenvalues, numberOfEigenvalues)
		oo_COMPMAT (eigenvectors, numberOfEigenvalues, dimension)
		oo_BOOLEAN (onlyReals) // commodity
	#endif

oo_END_CLASS (Eigen2)
#undef ooSTRUCT

/* End of file Eigen_def.h */

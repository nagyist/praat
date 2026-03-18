#ifndef _Eigen_h_
#define _Eigen_h_
/* Eigen.h
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

/*
	Eigen represents the eigenvalues and eigenvectors of a square matrix.
	If the matrix is symmetric, the eigenvalues and corresponding eigenvectors
	are real and sorted in descending order.
	If the square matrix is not symmetric than some of the eigenvalues and
	eigenvectors are complex. The complex eigenvalues always occur in pairs
	a + i b and a - i b, where the eigenvalue with the positive imaginary part always
	occurs before the other one in the list.
 */

#include "Collection.h"

#include "Eigen_def.h"
#include "Graphics.h"

#include "MAT_numerics.h"

void Eigen_init (Eigen me, integer numberOfEigenvalues, integer dimension);

void Eigen_initImaginaryParts (Eigen me);

autoEigen Eigen_create (integer numberOfEigenvalues, integer dimension);

autoEigen Eigen_createFromSquareRoot (constMATVU const& mat, integer numberOfEigenvalues);

void Eigen_initFromSquareMAT (Eigen me, constMATVU const& mat, kMAT_TYPE matType, integer numberOfEigenvalues);
autoEigen Eigen_createFromSquareMAT (constMATVU const& mat, kMAT_TYPE matType, integer numberOfEigenvalues);


void Eigen_initFromSymmetricMatrix (Eigen me, constMATVU const& a);

void Eigen_fromSymmetricTridiagonal (Eigen, constVEC const& diagonal, constVEC const& offDiagonal);

void Eigen_initFromSquareRoot (Eigen me, constMATVU const& a);
/*
	Calculate eigenstructure for symmetric matrix A'A (e.g. covariance matrix),
	when only A is given.
	Precondition: numberOfRows > 1
	Method: SVD.
*/

void Eigen_initFromSquareRootPair (Eigen me, constMAT a, constMAT b);
/*
	Calculate eigenstructure for A'Ax - lambda B'Bx = 0
	Preconditions: numberOfRows >= numberOfColumns &&
		numberOfRows_b >= numberOfColumns
	Method: Generalized SVD.
*/

integer Eigen_getNumberOfEigenvectors (Eigen me);

integer Eigen_getDimensionOfComponents (Eigen me);

double Eigen_getCumulativeContributionOfComponents (Eigen me, integer from, integer to);

integer Eigen_getDimensionOfFraction (Eigen me, double fraction);

double Eigen_getEigenvectorElement (Eigen me, integer ivec, integer element);

autoVEC Eigen_listEigenvalues (Eigen me);
autoVEC Eigen_listEigenvalues_imag (Eigen me);

autoVEC Eigen_getEigenvector (Eigen me, integer ivec);
autoVEC Eigen_getEigenvector_imag (Eigen me, integer ivec);

double Eigen_getSumOfEigenvalues (Eigen me, integer from, integer to);

inline bool Eigen_areAllEigenvaluesReal (Eigen me) {
	return my onlyReals;
}

void Eigen_sort (Eigen me, bool sortAscending);
/*
	Sort eigenvalues and corresponding eigenvectors.
	All eigenvalues have to be real!
*/

void Eigen_sort_special (Eigen me, bool sortAscending);
/*
	Precondition: eigenvalues are sorted.
	Postcondition: eigenvalues are ascending or descending.
*/

void Eigen_invertEigenvector (Eigen me, integer ivec); // TODO djmw complex?

void Eigen_drawEigenvalues (Eigen me, Graphics g, integer first, integer last, double ymin, double ymax,
	bool fractionOfTotal, bool cumulative, double size_mm, conststring32 mark, bool garnish);

void Eigen_drawEigenvector (Eigen me, Graphics g, integer ivec, integer first, integer last, double minimum, double maximum, bool weigh,
	double size_mm, conststring32 mark, bool connect, char32 **rowLabels, bool garnish);
/*
	Draw eigenvector. When rowLabels != nullptr, draw row text labels on bottom axis.
*/

/**
	Adapt the sign of each eigenvector except the first
	in such a way that it correlates positively with the first eigenvector.
*/
void Eigens_alignEigenvectors (OrderedOf<structEigen>* me);

double Eigens_getAngleBetweenEigenplanes_degrees (Eigen me, Eigen thee);
/*
	Get angle between the eigenplanes, spanned by the first two eigenvectors.
*/

#endif /* _Eigen_h_ */


/* MAT_TYPE_enums.h
 *
 * Copyright (C) 2026 David Weenink
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

enums_begin (kMAT_TYPE, 1)
	enums_add (kMAT_TYPE, 1, GENERAL, U"general")
	enums_add (kMAT_TYPE, 2, SYMMETRIC, U"symmetric")
	enums_add (kMAT_TYPE, 3, TRIDIAGONAL, U"tridiagonal")
	enums_add (kMAT_TYPE, 4, SYMMETRIC_TRIDIAGONAL, U"symmetric tridiagonal")
enums_end (kMAT_TYPE, 4, GENERAL)

/* End of file MAT_TYPE_enums.h */

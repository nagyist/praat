/* EditDistanceTable_def.h
 *
 * Copyright (C) 2012,2014 David Weenink, 2025 Paul Boersma
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


#define ooSTRUCT PairOfInteger
oo_DEFINE_STRUCT (PairOfInteger)

	oo_INTEGER (x)
	oo_INTEGER (y)

oo_END_STRUCT (PairOfInteger)
#undef ooSTRUCT


#define ooSTRUCT EditCostsTable
oo_DEFINE_CLASS (EditCostsTable, TableOfReal)

	#if oo_DECLARING
		void v1_info ()
			override;

		virtual bool v_matchTargetSymbol (conststring32 targetSymbol, conststring32 symbol);
		virtual bool v_matchSourceSymbol (conststring32 sourceSymbol, conststring32 symbol);
		virtual bool v_matchTargetWithSourceSymbol (conststring32 targetSymbol, conststring32 sourceSymbol);
	#endif

oo_END_CLASS (EditCostsTable)
#undef ooSTRUCT


#define ooSTRUCT EditDistanceTable
oo_DEFINE_CLASS (EditDistanceTable, TableOfReal)

	oo_OBJECT (Daata, 0, dummy1)   // this used to be a WarpingPath object, obsolete since 2025
	oo_INTEGER (pathLength)
	oo_STRUCTVEC (PairOfInteger, path, pathLength)
	oo_OBJECT (EditCostsTable, 0, editCostsTable)

	#if oo_DECLARING
		void v1_info ()
			override;
	#endif

oo_END_CLASS (EditDistanceTable)
#undef ooSTRUCT


/* End of file EditDistanceTable_def.h */

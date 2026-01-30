#ifndef _melder_assert_h_
#define _melder_assert_h_
/* melder_assert.h
 *
 * Copyright (C) 1992-2018 Paul Boersma
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

/*
	SYNOPSIS

	Melder_assert (condition);

	Crash with a message based on the following template:
		"Assertion failed in file <fileName> on line <lineNumber>: <condition>"
*/

void _private_Melder_assert (const char *fileName, int lineNumber, const char *condition);

#ifdef NDEBUG
	#define Melder_assert(x)   ((void) 0)
#else
	#define Melder_assert(x)   ((x) ? (void) (0) : (_private_Melder_assert (__FILE__, __LINE__, #x), abort ()))
#endif

template <typename Tag>
class _private_Melder_nonReentrant {
	inline static bool entered = false;
public:
	_private_Melder_nonReentrant (const char *fileName, int lineNumber) {
		if (entered) {
			_private_Melder_assert (fileName, lineNumber, "This non-reentrant function in now entered for the second time.");
			abort ();
		}
		entered = true;
	}
	~_private_Melder_nonReentrant () {
		entered = false;
	}
};
#define MELDER_ASSERT_NONREENTRANT \
	_private_Melder_nonReentrant <struct _private_Melder_nonReentrant_tag_##__COUNTER__> _private_Melder_nonReentrant_guard (__FILE__, __LINE__);

#endif // !_melder_assert_h_

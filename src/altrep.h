// Copyright (C) 2019  Nicolas Bennett

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.

// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef SRC_ALTREP_H_
#define SRC_ALTREP_H_

#include <R.h>
#include <Rinternals.h>
#include <Rversion.h>

#include <R_ext/Rdynload.h>

// workaround taken from romainfrancois/altrepisode:
// because R's <R_ext/Altrep.h> not so conveniently uses `class`
// as a variable name, and C++ is not happy about that
//
// SEXP R_new_altrep(R_altrep_class_t class, SEXP data1, SEXP data2);

#if R_VERSION < R_Version(3, 6, 0)

#define class klass

// Because functions declared in <R_ext/Altrep.h> have C linkage
extern "C" {
  #include <R_ext/Altrep.h>
}

// undo the workaround
#undef class

#else
  #include <R_ext/Altrep.h>
#endif

#endif  // SRC_ALTREP_H_

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

#include <RcppArmadillo.h>
#include <shmemr.h>

#include "altrep.h"

struct alt_shreal
{
  static R_altrep_class_t class_t;

  static SEXP make(std::string name, double length, std::string type)
  {
    Rcpp::List meta = shmemr::mem_init(name, length * sizeof(double), type);

    SEXP xp = PROTECT(shmemr::get_mem_ptr(meta));

    SEXP res = R_new_altrep(class_t, xp, meta);

    UNPROTECT(1);
    return res;
  }

  static double* ptr(SEXP x)
  {
    auto res = R_ExternalPtrAddr(R_altrep_data1(x));

    if (!res)
    {
      auto meta = shmemr::mem_reinit(R_altrep_data2(x));
      res = shmemr::get_mem_ptr(meta);
    }

    return static_cast<double*>(res);
  }

  // ALTREP methods -------------------

  static R_xlen_t Length(SEXP x)
  {
    auto lst = Rcpp::List(R_altrep_data2(x));
    return static_cast<R_xlen_t>(lst["length"]);
  }

  static Rboolean Inspect(SEXP x, int pre, int deep, int pvec,
      void (*inspect_subtree)(SEXP, int, int, int))
  {
    Rprintf("alt_shreal (len=%d, ptr=%p)\n", Length(x), ptr(x));
    return TRUE;
  }

  // ALTVEC methods ------------------

  static const void* Dataptr_or_null(SEXP x)
  {
    return ptr(x);
  }

  static void* Dataptr(SEXP x, Rboolean writeable)
  {
    return ptr(x);
  }

  // ALTREAL methods -----------------

  // this does not do bounds checking because that's expensive, so
  // the caller must take care of that
  static double real_Elt(SEXP vec, R_xlen_t i)
  {
    return ptr(vec)[i];
  }

  static R_xlen_t Get_region(SEXP vec, R_xlen_t start, R_xlen_t size,
      double* out)
  {
    double* dptr = ptr(vec);
    R_xlen_t len = Length(vec) - start;
    R_xlen_t ncopy = len > size ? len : size;

    for (R_xlen_t k = 0; k < ncopy; k++)
    {
      out[k] = dptr[start + k];
    }

    return ncopy;
  }

  // -------- initialize the altrep class with the methods above

  static void Init(DllInfo* dll)
  {
    class_t = R_make_altreal_class("alt_shreal", "shvecr", dll);

    // altrep
    R_set_altrep_Length_method(class_t, Length);
    R_set_altrep_Inspect_method(class_t, Inspect);

    // altvec
    R_set_altvec_Dataptr_method(class_t, Dataptr);
    R_set_altvec_Dataptr_or_null_method(class_t, Dataptr_or_null);

    // altreal
    R_set_altreal_Elt_method(class_t, real_Elt);
    R_set_altreal_Get_region_method(class_t, Get_region);
  }

};

// static initialization of alt_shreal::class_t
R_altrep_class_t alt_shreal::class_t;

// Called the package is loaded (needs Rcpp 0.12.18.3)
// [[Rcpp::init]]
void init_alt_shreal(DllInfo* dll)
{
  alt_shreal::Init(dll);
}

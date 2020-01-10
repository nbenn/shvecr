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

struct arma_vec
{
  static R_altrep_class_t class_t;

  static SEXP make(arma::dvec* data, bool owner)
  {
    SEXP xp = PROTECT(R_MakeExternalPtr(data, R_NilValue, R_NilValue));

    if (owner) {
      R_RegisterCFinalizerEx(xp, arma_vec::finalize, TRUE);
    }

    SEXP res = R_new_altrep(class_t, xp, R_NilValue);

    UNPROTECT(1);
    return res;
  }

  static void finalize(SEXP xp)
  {
    delete static_cast<arma::dvec*>(R_ExternalPtrAddr(xp));
  }

  static arma::dvec* ptr(SEXP x)
  {
    return static_cast<arma::dvec*>(R_ExternalPtrAddr(R_altrep_data1(x)));
  }

  static arma::dvec& get(SEXP vec)
  {
    return *ptr(vec) ;
  }

  // ALTREP methods -------------------

  static R_xlen_t Length(SEXP vec)
  {
    return get(vec).n_elem;
  }

  static Rboolean Inspect(SEXP x, int pre, int deep, int pvec,
      void (*inspect_subtree)(SEXP, int, int, int))
  {
    Rprintf("arma::dvec (len=%d, ptr=%p)\n", Length(x), ptr(x));
    return TRUE;
  }

  // ALTVEC methods ------------------

  static const void* Dataptr_or_null(SEXP vec)
  {
    return get(vec).memptr();
  }

  static void* Dataptr(SEXP vec, Rboolean writeable)
  {
    return get(vec).memptr();
  }


  // ALTREAL methods -----------------

  // this does not do bounds checking because that's expensive, so
  // the caller must take care of that
  static double real_Elt(SEXP vec, R_xlen_t i)
  {
    return get(vec)[i];
  }

  static R_xlen_t Get_region(SEXP vec, R_xlen_t start, R_xlen_t size,
      double* out)
  {
    out = get(vec).memptr() + start;
    R_xlen_t len = get(vec).n_elem - start;
    return len > size ? len : size;
  }

  // -------- initialize the altrep class with the methods above

  static void Init(DllInfo* dll)
  {
    class_t = R_make_altreal_class("arma_vec", "shvecr", dll);

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

// static initialization of arma_vec::class_t
R_altrep_class_t arma_vec::class_t;

// Called the package is loaded (needs Rcpp 0.12.18.3)
// [[Rcpp::init]]
void init_arma_vec(DllInfo* dll)
{
  arma_vec::Init(dll);
}

//' an altrep object that wraps a arma::dvec
//'
//' @export
// [[Rcpp::export]]
SEXP doubles()
{
  // create a new arma::dvec
  //
  // this uses `new` because we want the vector to survive
  // it is deleted when the altrep object is garbage collected
  auto v = new arma::dvec {-2.0, -1.0, 0.0, 1.0, 2.0};

  // The altrep object owns the arma::dvec
  return arma_vec::make(v, true);
}

// example C++ function that returns `n` random number between 0 and 1
arma::dvec randoms(int n)
{
  return arma::randu<arma::dvec>(n);
}

// [[Rcpp::export]]
SEXP doubles_example()
{
  // get a arma::dvec from somewhere
  auto v = randoms(10);

  // wrap it into an altrep object of class `arma_vec::class_t`
  // the altrep object does not own the vector, because we only need
  // it within this scope, it will be deleted just like any C++ object
  // at the end of this C++ function
  SEXP x = PROTECT(arma_vec::make(&v, false));

  // call sum(x) in the base environment
  SEXP s_sum = Rf_install("sum");
  SEXP call = PROTECT(Rf_lang2(s_sum, x));
  SEXP res = Rf_eval(call, R_BaseEnv);

  UNPROTECT(2);

  return res;
}

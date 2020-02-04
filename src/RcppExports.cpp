// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// new_altreal
SEXP new_altreal(std::string name, double length, std::string type);
RcppExport SEXP _shvecr_new_altreal(SEXP nameSEXP, SEXP lengthSEXP, SEXP typeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type name(nameSEXP);
    Rcpp::traits::input_parameter< double >::type length(lengthSEXP);
    Rcpp::traits::input_parameter< std::string >::type type(typeSEXP);
    rcpp_result_gen = Rcpp::wrap(new_altreal(name, length, type));
    return rcpp_result_gen;
END_RCPP
}
// create_vec3
SEXP create_vec3(SEXP mem, double length, SEXP type);
RcppExport SEXP _shvecr_create_vec3(SEXP memSEXP, SEXP lengthSEXP, SEXP typeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type mem(memSEXP);
    Rcpp::traits::input_parameter< double >::type length(lengthSEXP);
    Rcpp::traits::input_parameter< SEXP >::type type(typeSEXP);
    rcpp_result_gen = Rcpp::wrap(create_vec3(mem, length, type));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_shvecr_new_altreal", (DL_FUNC) &_shvecr_new_altreal, 3},
    {"_shvecr_create_vec3", (DL_FUNC) &_shvecr_create_vec3, 3},
    {NULL, NULL, 0}
};

void init_alt_shreal(DllInfo* dll);
RcppExport void R_init_shvecr(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    init_alt_shreal(dll);
}

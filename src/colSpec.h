#include <mdbtools.h>

// copied and adapted from readxl
// this acts also as a mapping between Access types and R types.
inline Rcpp::RObject makeCol(int type, int n) {

  switch (type) {
  case MDB_BOOL:
    return Rcpp::LogicalVector(n, NA_LOGICAL);
  case MDB_BYTE:
    return Rcpp::CharacterVector(n, NA_STRING); // TODO
  case MDB_INT:
    return Rcpp::IntegerVector(n, NA_INTEGER);
  case MDB_LONGINT:
    return Rcpp::IntegerVector(n, NA_INTEGER);
  case MDB_COMPLEX:
    return Rcpp::CharacterVector(n, NA_STRING); // TODO;
  case MDB_FLOAT:
    return Rcpp::NumericVector(n, NA_REAL);
  case MDB_DOUBLE:
    return Rcpp::NumericVector(n, NA_REAL);
  case MDB_TEXT:
    return Rcpp::StringVector(n, NA_STRING);
  case MDB_OLE:
    return Rcpp::CharacterVector(n, NA_STRING); // TODO;
  case MDB_DATETIME:
    return Rcpp::CharacterVector(n, NA_STRING); // TODO;
  case MDB_MEMO:
    return Rcpp::StringVector(n, NA_STRING);
  case MDB_MONEY:
    return Rcpp::StringVector(n, NA_STRING);
  }

  return R_NilValue;
}




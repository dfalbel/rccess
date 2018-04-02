#include <mdbtools.h>

enum ColType {
  COL_UNKNOWN, // implies column type needs to be guessed
  COL_LOGICAL,
  COL_DATE,
  COL_NUMERIC,
  COL_TEXT,
  COL_SKIP     // occurs only as user-specified column type
};

std::string colTypeDesc(ColType type) {
  switch(type) {
  case COL_UNKNOWN: return "unknown";
  case COL_LOGICAL: return "logical";
  case COL_DATE:    return "date";
  case COL_NUMERIC: return "numeric";
  case COL_TEXT:    return "text";
  case COL_SKIP:    return "skip";
  }
  return "???";
};

// convert MDB types to R types
ColType mdbType2colType (int mdb_type) {

  switch (mdb_type) {
  case MDB_BOOL: return COL_LOGICAL;
  case MDB_BYTE: return COL_UNKNOWN;
  case MDB_INT: return COL_NUMERIC;
  case MDB_LONGINT: return COL_NUMERIC;
  case MDB_MONEY: return COL_UNKNOWN;
  case MDB_FLOAT: return COL_NUMERIC;
  case MDB_DOUBLE: return COL_NUMERIC;
  case MDB_DATETIME: return COL_DATE;
  case MDB_BINARY: return COL_UNKNOWN;
  case MDB_TEXT: return COL_TEXT;
  case MDB_OLE: return COL_UNKNOWN;
  case MDB_MEMO: return COL_UNKNOWN;
  case MDB_REPID: return COL_UNKNOWN;
  case MDB_NUMERIC: return COL_NUMERIC;
  case MDB_COMPLEX: return COL_UNKNOWN;
  };

  return COL_UNKNOWN;
}

// another makeCol to try out.
inline Rcpp::RObject makeCol2(ColType type, int n) {

  switch(type) {
  case COL_UNKNOWN:
    return R_NilValue;
  case COL_SKIP:
    return R_NilValue;
  case COL_LOGICAL:
    return Rcpp::LogicalVector(n, NA_LOGICAL);
  case COL_DATE: {
    Rcpp::RObject col = Rcpp::NumericVector(n, NA_REAL);
    col.attr("class") = Rcpp::CharacterVector::create("POSIXct", "POSIXt");
    col.attr("tzone") = "UTC";
    return col;
  }
  case COL_NUMERIC:
    return Rcpp::NumericVector(n, NA_REAL);
  case COL_TEXT:
    return Rcpp::CharacterVector(n, NA_STRING);
  }

  return R_NilValue;
}


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




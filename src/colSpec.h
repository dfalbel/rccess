#include <mdbtools.h>

// copied from readxl
enum RColType {
  COL_UNKNOWN, // implies column type needs to be guessed
  COL_BLANK,   // occurs when col_types = NULL and observe only CELL_BLANKs
  COL_LOGICAL,
  COL_DATE,
  COL_NUMERIC,
  COL_TEXT,
  COL_LIST,    // occurs only as user-specified column type
  COL_SKIP     // occurs only as user-specified column type
};

// copied from readxl
inline std::string RColTypeDesc(RColType type) {
  switch(type) {
  case COL_UNKNOWN: return "unknown";
  case COL_BLANK:   return "blank";
  case COL_LOGICAL: return "logical";
  case COL_DATE:    return "date";
  case COL_NUMERIC: return "numeric";
  case COL_TEXT:    return "text";
  case COL_LIST:    return "list";
  case COL_SKIP:    return "skip";
  }
  return "???";
}

// copied from readxl
inline Rcpp::CharacterVector RColTypeDescs(std::vector<RColType> types) {
  Rcpp::CharacterVector out(types.size());
  for (size_t i = 0; i < types.size(); ++i) {
    out[i] = RColTypeDesc(types[i]);
  }
  return out;
}

// copied and adapted from readxl
// this acts also as a mapping between Access types and R types.
inline Rcpp::RObject makeCol(int type, int n) {
  switch(type) {
  case COL_UNKNOWN:
  case COL_BLANK:
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
  case COL_LIST:
    return Rcpp::List(n, Rcpp::LogicalVector(1, NA_LOGICAL));
  }

  return R_NilValue;
}



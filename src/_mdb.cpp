#include <Rcpp.h>
#include "_mdb.h"
using namespace Rcpp;

// [[Rcpp::export]]
CharacterVector get_table_names(std::string path) {
  Mdb m(path);

  return m.getTableNames();
};

#include <Rcpp.h>
#include "_mdb.h"
using namespace Rcpp;

// [[Rcpp::export]]
CharacterVector get_table_names(std::string path) {
  Mdb m(path);

  return m.getTableNames();
};

// [[Rcpp::export]]
CharacterVector get_var_names (std::string path, std::string table_name) {
  Mdb m(path);

  return m.getVarNames(table_name);
}

#include <Rcpp.h>
#include "mdb.h"
using namespace Rcpp;

// [[Rcpp::export]]
CharacterVector get_table_names(std::string path) {
  Mdb m(path);

  return m.getTableNames();
};

// [[Rcpp::export]]
Rcpp::DataFrame get_table_schema (std::string path, std::string table_name) {
  Mdb m(path);

  return m.getTableSchema(table_name);
}

// [[Rcpp::export]]
Rcpp::DataFrame get_table (std::string path, std::string table_name) {
  Mdb m(path);

  return m.getTable(table_name);
}


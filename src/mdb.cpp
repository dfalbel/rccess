#include <Rcpp.h>
#include "mdb.h"
using namespace Rcpp;

// [[Rcpp::export]]
IntegerVector get_num_catalogs(std::string path) {
  Mdb m(path);
  return m.getNumCatalog();
}

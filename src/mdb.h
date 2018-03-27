#include <Rcpp.h>
#include <libmdb/mdbtools.h>
using namespace Rcpp;

// copied from
// https://github.com/tidyverse/readxl/blob/62cc348473939ddedb9c85fe678a583f8a0d9b14/src/XlsWorkBook.h#L9
inline std::string normalizePath(std::string path) {
  Rcpp::Environment baseEnv = Rcpp::Environment::base_env();
  Rcpp::Function normalizePath = baseEnv["normalizePath"];
  return Rcpp::as<std::string>(normalizePath(path, "/", true));
}

class Mdb {

  std::string path_;
  int num_catalog;

public:

  Mdb (const std::string& path) {
    this->path_ = normalizePath(path);

    MdbHandle *mdb = mdb_open (path_.c_str(), MDB_NOFLAGS);

    if (!mdb) {
      Rcpp::stop("Couldn't open database.");
    };

    if (!mdb_read_catalog (mdb, MDB_ANY)) {
      Rcpp::stop("File does not appear to be an Access database");
    };

    this->num_catalog = mdb->num_catalog;

  };

  int getNumCatalog () {
    return this->num_catalog;
  }

};

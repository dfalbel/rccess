#include <Rcpp.h>
#include <mdbtools.h>
using namespace Rcpp;


// copied from
// https://github.com/tidyverse/readxl/blob/62cc348473939ddedb9c85fe678a583f8a0d9b14/src/XlsWorkBook.h#L9
inline std::string normalizePath(std::string path) {
  Rcpp::Environment baseEnv = Rcpp::Environment::base_env();
  Rcpp::Function normalizePath = baseEnv["normalizePath"];
  return Rcpp::as<std::string>(normalizePath(path, "/", true));
};

class Mdb {

  std::string path_;
  MdbHandle *mdb;

public:

  // class init
  Mdb (const std::string& path) {

    this->path_ = normalizePath(path);
    this->mdb = mdb_open (path_.c_str(), MDB_NOFLAGS);

    if (!this->mdb) {
      Rcpp::stop("Couldn't open database.");
    };

    if (!mdb_read_catalog (this->mdb, MDB_ANY)) {
      Rcpp::stop("File does not appear to be an Access database");
    };

  };

  // destructor
  ~Mdb () {
    mdb_close(this->mdb);
  };

  // get included table names
  CharacterVector getTableNames () {

    MdbCatalogEntry * entry;
    CharacterVector table_names;

    for (int i=0; i < this->mdb->num_catalog; i++) {
      entry = static_cast<MdbCatalogEntry*>(g_ptr_array_index (mdb->catalog, i));

      if (entry->object_type != MDB_TABLE)
        continue;
      if (mdb_is_system_table(entry))
        continue;

      table_names.push_back(entry->object_name);
    };

    return table_names;
  };

  CharacterVector getVarNames(std::string table_name) {

    MdbCatalogEntry * entry;
    CharacterVector var_names;

    for (int i=0; i < this->mdb->num_catalog; i++) {
      entry = static_cast<MdbCatalogEntry*>(g_ptr_array_index (mdb->catalog, i));
      if (entry->object_name == table_name) {
        MdbTableDef *table = mdb_read_table (entry);
        mdb_read_columns (table);
        MdbColumn *col;

        for (i = 0; i < table->num_cols; i++) {
          col = static_cast<MdbColumn*>(g_ptr_array_index (table->columns, i));
          var_names.push_back(col->name);
        };
      };
    };

    return var_names;
  };

};



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

Rcpp::String mdb_col_disp_type (MdbColumn *col) {
  switch (col->col_type) {
  case MDB_BOOL:
    return "bool";
    break;
  case MDB_BYTE:
    return "byte";
    break;
  case MDB_INT:
    return "int";
    break;
  case MDB_LONGINT:
    return "longint";
    break;
  case MDB_COMPLEX:
    return "complex";
    break;
  case MDB_FLOAT:
    return "float";
    break;
  case MDB_DOUBLE:
    return "double";
    break;
  case MDB_TEXT:
    return "text";
    break;
  case MDB_DATETIME:
    return "datetime";
    break;
  case MDB_MEMO:
    return "memo";
    break;
  case MDB_MONEY:
    return "money";
    break;
  }
  return NA_STRING;
}

MdbTableDef *read_table_by_name (MdbHandle *mdb, std::string table_name, int obj_type) {
  unsigned int i;
  MdbCatalogEntry *entry;

  mdb_read_catalog(mdb, obj_type);

  for (i=0; i<mdb->num_catalog; i++) {
    entry = static_cast<MdbCatalogEntry*>(g_ptr_array_index(mdb->catalog, i));
    if (entry->object_name == table_name)
      return mdb_read_table(entry);
  }

  Rcpp::stop("Didn't find a table with the provided name.");
}

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

  Rcpp::DataFrame getTableSchema(std::string table_name) {

    CharacterVector col_names;
    CharacterVector col_types;

    MdbTableDef *table = read_table_by_name(this->mdb, table_name, MDB_TABLE);

    mdb_read_columns (table);
    MdbColumn *col;

    for (int j = 0; j < table->num_cols; j++) {
      col = static_cast<MdbColumn*>(g_ptr_array_index (table->columns, j));
      col_names.push_back(col->name);
      col_types.push_back(mdb_col_disp_type(col));
    };

    return Rcpp::DataFrame::create(
      Named("col_names") = col_names,
      Named("col_types") = col_types
    );
  };

  Rcpp::DataFrame getTable (std::string table_name) {

    MdbTableDef *table;
    table = read_table_by_name(this->mdb, table_name, MDB_TABLE);
    mdb_read_columns(table);
    mdb_rewind_table(table);

    return Rcpp::DataFrame::create();
  };


};


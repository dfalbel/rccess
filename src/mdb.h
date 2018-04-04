#include <Rcpp.h>
#include "mdbtools.h"
#include "colSpec.h"
using namespace Rcpp;


// copied from
// https://github.com/tidyverse/readxl/blob/62cc348473939ddedb9c85fe678a583f8a0d9b14/src/XlsWorkBook.h#L9
std::string normalizePath(const std::string path) {
  Rcpp::Environment baseEnv = Rcpp::Environment::base_env();
  Rcpp::Function normalizePath = baseEnv["normalizePath"];
  return Rcpp::as<std::string>(normalizePath(path, "/", true));
};

Rcpp::String mdb_col_disp_type (const MdbColumn *col) {
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
  return "unknown";
};

MdbTableDef *read_table_by_name (MdbHandle *mdb, const std::string table_name) {

  MdbCatalogEntry *entry;

  for (int i=0; i<mdb->num_catalog; i++) {
    entry = static_cast<MdbCatalogEntry*>(g_ptr_array_index(mdb->catalog, i));
    if (entry->object_name == table_name) {
      return mdb_read_table(entry);
    };
  }

  Rcpp::stop("Didn't find a table with the provided name.");
  return NULL;
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

    if (!mdb_read_catalog (this->mdb, MDB_TABLE)) {
      Rcpp::stop("File does not appear to be an Access database");
    };

  };

  // destructor
  ~Mdb () {
    mdb_close(this->mdb);
  };

  // get included table names
  Rcpp::CharacterVector getTableNames () {

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
    MdbTableDef *table;

    table = read_table_by_name(this->mdb, table_name);

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
    table = read_table_by_name(this->mdb, table_name);

    mdb_read_columns(table);
    mdb_rewind_table(table);

    char **bound_values;
    int  *bound_lens;

    bound_values = (char **) g_malloc(table->num_cols * sizeof(char *));
    bound_lens = (int *) g_malloc(table->num_cols * sizeof(int));

    for (int i=0;i<table->num_cols;i++) {
      /* bind columns */
      bound_values[i] = (char *) g_malloc0(MDB_BIND_SIZE);
      mdb_bind_column(table, i+1, bound_values[i], &bound_lens[i]);
    }

    // Initialize the List (will convert to data frame later)
    Rcpp::List out(table->num_cols);
    MdbColumn *col;
    Rcpp::StringVector out_names(table->num_cols);
    for (int j = 0; j < table->num_cols; j++) {
      col =  static_cast<MdbColumn*>(g_ptr_array_index(table->columns,j));
      ColType type = mdbType2colType(col->col_type);
      out[j] = makeCol(type, table->num_rows);
      out_names[j] = col->name;
    }
    out.attr("names") = out_names;
    out.attr("class") = "data.frame";
    out.attr("row.names") = seq_len(table->num_rows);

    std::string value;
    size_t length;
    int j = 0;
    while(mdb_fetch_row(table)) {

      for (int i=0; i < table->num_cols; i++) {
        col =  static_cast<MdbColumn*>(g_ptr_array_index(table->columns,i));
        value = bound_values[i];
        length = bound_lens[i];
        ColType type = mdbType2colType(col->col_type);

        switch(type) {

        case COL_DATE: {
          // we are still treating dates as strings
          Rcpp::CharacterVector column = out[i];
          column[j] = value;
          break;
        };
        case COL_LOGICAL: {
          Rcpp::LogicalVector column = out[i];
          column[j] = std::stoi(value);
          break;
        };
        case COL_NUMERIC: {
          Rcpp::NumericVector column = out[i];
          if (value.length() > 0){
            column[j] = std::stold(value);
          } else {
            column[j] = NA_REAL;
          }
          break;
        };
        case COL_SKIP: {
          // skiping column
        };
        case COL_TEXT: {
          Rcpp::CharacterVector column = out[i];
          column[j] = value;
          break;
        };
        case COL_UNKNOWN: {
          // unknown column
        };
        };

        //Rcout << value << "\n";
        //column[k] = value;

        //Rcout << "column: " << (i + 1) << " of " << table->num_cols << " col_name: "<< col->name << " col_type: " << static_cast<std::string>(mdb_col_disp_type(col)) <<" value: "<< value << "\n";
      }
      j++;
    }

    return out;
  };

};


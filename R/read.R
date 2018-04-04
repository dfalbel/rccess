#' @useDynLib rccess, .registration = TRUE
#' @importFrom Rcpp sourceCpp
NULL

#' Read MDB files
#'
#' @param path Path to the mdb/accdb file.
#' @param table_name Table to read. A string with the name of table to read. If
#' NULL, defaults to the first table.
#' @param n_max Maximum numbers of data rows to read. Use any negative number to read
#' all rows.
#'
#' @export
read_mdb <- function(path, table_name = NULL,  n_max = -1) {

  if (is.null(table_name)) {
    table_name <- mdb_tables(path)[1]
  }

  get_table(path, table_name, n_max)
}

#' List Tables in MDB files
#'
#' @param path Path to the mdb/accdb file.
#'
mdb_tables <- function(path) {
  get_table_names(path)
}

context("get_table functions")

path_mdb <- "../../inst/extdata/AgeRange.mdb"
path_accdb <-"../../inst/extdata/Books2010.accdb"

test_that("get_table_names errors", {

  expect_error(
    rccess:::get_table_names(paste0(path_mdb, "wrongPath"))
  )

  expect_error(
    rccess:::get_table_names("DESCRIPTION")
  )

})

test_that("get_table_names for mdb files", {

  expect_equal(
    rccess:::get_table_names(path_mdb),
    "tblEmployees"
  )

})

test_that("get_table_names for accdb files", {

  expect_is(
    rccess:::get_table_names(path_accdb),
    "character"
  )

  expect_length(
    rccess:::get_table_names(path_accdb),
    10L
  )

})

test_that("get_table_schema errors", {

  expect_error(
    rccess:::get_table_schema(path_mdb, "wrongTableName")
  )

  expect_error(
    rccess:::get_table_schema(path_accdb, "wrongTableName")
  )

})

test_that("get_table_schema for mdb files", {

  expect_is(
    rccess:::get_table_schema(path_mdb, "tblEmployees"),
    "data.frame"
  )

})

test_that("get_table_schema for accdb files", {

  expect_is(
    rccess:::get_table_schema(path_accdb, "Employees"),
    "data.frame"
  )

  expect_is(
    rccess:::get_table_schema(path_accdb, "AllSex"),
    "data.frame"
  )

})

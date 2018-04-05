context("get_table_schema.R")

path_mdb <- test_mdb("AgeRange.mdb")
path_accdb <- test_mdb("Books2010.accdb")

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

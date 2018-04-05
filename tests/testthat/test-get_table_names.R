context("get_table_names.R")

path_mdb <- test_mdb("AgeRange.mdb")
path_accdb <- test_mdb("Books2010.accdb")

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


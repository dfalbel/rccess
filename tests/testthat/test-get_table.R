context("get_table functions")

path <- "../../inst/extdata/AgeRange.mdb"

test_that("get_table_names", {

  expect_equal(
    rccess:::get_table_names(path),
    "tblEmployees"
  )

  expect_error(
    rccess:::get_table_names(paste0(path, "wrongPath"))
  )

  expect_error(
    rccess:::get_table_names("DESCRIPTION")
  )

})

test_that("get_table_schema", {

  expect_is(
    rccess:::get_table_schema(path, "tblEmployees"),
    "data.frame"
  )

  expect_error(
    rccess:::get_table_schema(path, "wrongTableName")
  )

})

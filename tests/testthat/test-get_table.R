context("get_table")

test_that("get_table_names", {

  expect_equal(
    rccess:::get_table_names("../../inst/extdata/AgeRange.mdb"),
    "tblEmployees"
  )

})


test_that("get_table_schema", {

  expect_is(
    rccess:::get_table_schema("../../inst/extdata/AgeRange.mdb", "tblEmployees"),
    "data.frame"
  )

})

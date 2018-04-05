context("get_table")

path_mdb <- test_mdb("AgeRange.mdb")
path_accdb <- test_mdb("Books2010.accdb")

test_that("get table for mdb files", {

  x <- rccess:::get_table(path_mdb, "tblEmployees")

  expect_equal(ncol(x), 16)
  expect_equal(nrow(x), 15)
  expect_named(x)
  expect_is(x, "data.frame")

})

test_that("get table for accdb files", {

  x <- rccess:::get_table(path_accdb, "Employees")

  expect_equal(ncol(x), 8)
  expect_equal(nrow(x), 43)
  expect_named(x)
  expect_is(x, "data.frame")


})

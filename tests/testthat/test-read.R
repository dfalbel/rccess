context("read_mdb")

path_mdb <- test_mdb("AgeRange.mdb")
path_accdb <- test_mdb("Books2010.accdb")

test_that("read mdb files", {

  x <- read_mdb(path_mdb, "tblEmployees")

  expect_equal(ncol(x), 16)
  expect_equal(nrow(x), 15)
  expect_named(x)
  expect_is(x, "data.frame")

})

test_that("read mdb files with no table name specified", {

  x <- read_mdb(path_mdb)

  expect_equal(ncol(x), 16)
  expect_equal(nrow(x), 15)
  expect_named(x)
  expect_is(x, "data.frame")

})

test_that("read mdb files with n_max", {

  x <- read_mdb(path_mdb, "tblEmployees", n_max = 10)

  expect_equal(ncol(x), 16)
  expect_equal(nrow(x), 10)
  expect_named(x)
  expect_is(x, "data.frame")

})

test_that("read accdb files", {

  x <- read_mdb(path_accdb, "Employees")

  expect_equal(ncol(x), 8)
  expect_equal(nrow(x), 43)
  expect_named(x)
  expect_is(x, "data.frame")

})

test_that("read accdb files with no table name specified", {

  x <- read_mdb(path_accdb)

  expect_equal(ncol(x), 1)
  expect_equal(nrow(x), 3)
  expect_named(x)
  expect_is(x, "data.frame")

})

test_that("reading more files", {
  files <- dir(rprojroot::find_testthat_root_file("mdb"), full.names = TRUE)
  all_df <- lapply(files, read_mdb)

  expect_true(all(sapply(all_df, class) == "data.frame"))
})



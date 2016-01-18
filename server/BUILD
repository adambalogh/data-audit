package(default_visibility=["//visibility:public"])

cc_library(
  name = "fetcher",
  hdrs = ["fetcher.h"],
  deps = [
    "//audit:common",
    "//audit/proto:cpor",
  ]
)

cc_library(
  name = "prover",
  srcs = ["prover.cc"],
  hdrs = ["prover.h"],
  deps = [
    ":fetcher",
    "//audit:common",
    "//audit:util",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_binary(
  name = "prover_test",
  srcs = ["prover_test.cc"],
  deps = [
    "@gtest//:main",
    ":prover",
    "//audit/third_party/openssl:openssl",
    "//audit:util",
    "//audit:common",
    "//audit/client/upload:file",
    "//audit/client/upload:block_tagger",
    "//audit/client:prf",
    "//audit:test_util",
    "//audit/proto:cpor",
  ]
)

cc_library(
  name = "local_disk_fetcher",
  srcs = ["local_disk_fetcher.cc"],
  hdrs = ["local_disk_fetcher.h"],
  deps = [
    ":fetcher",
    "//audit/proto:cpor",
    "//audit/client/upload:local_disk_storage",
  ]
)


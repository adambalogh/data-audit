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
    "//audit/third_party/gtest:gtest",
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



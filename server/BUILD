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
    "//audit/client:file_tag",
    "//audit/client:block_tagger",
    "//audit/client:prf",
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
  ]
)

cc_binary(
  name = "local_disk_fetcher_test",
  srcs = ["local_disk_fetcher_test.cc"],
  deps = [
    "@gtest//:main",
    ":local_disk_fetcher",
    "//audit/proto:cpor",
    "//audit:common",
    "//audit:util",
    "//audit/third_party/openssl:openssl",
  ]
)

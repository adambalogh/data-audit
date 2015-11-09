cc_library(
  name = "fetcher",
  srcs = ["fetcher.cc"],
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

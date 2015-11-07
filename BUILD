package(default_visibility=["//visibility:public"])

cc_library(
  name = "common",
  srcs = [],
  hdrs = [
    "common.h",
  ],
  deps = [
    "//audit/third_party/openssl:openssl",
  ]
)

cc_library(
  name = "cpor_types",
  srcs = [],
  hdrs = ["cpor_types.h"],
  deps = [
    ":util",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_library(
  name = "util",
  srcs = ["util.cc"],
  hdrs = ["util.h"],
  deps = [
    "//audit:common",
    "//audit/third_party/openssl:openssl",
    "//audit/third_party/cryptopp:cryptopp",
  ]
)

cc_test(
  name = "util_test",
  srcs = ["util_test.cc"],
  deps = [
    "@gtest//:main",
    "//audit:common",
    ":util",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ],
)

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
    ":common",
    ":util",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ],
)

cc_library(
  name = "test_util",
  hdrs = ["test_util.h"],
  srcs = ["test_util.cc"],
  deps = [
    ":util",
    ":common",
    "//audit/server:fetcher",
    "//audit/client/upload:file",
    "//audit/proto:cpor",
    "//audit/client:prf",
  ]
)

cc_library(
    name = "block_tag_map",
    hdrs = ["block_tag_map.h"],
    deps = [
        "//audit/proto:cpor",
    ]
)

cc_binary(
    name = "block_tag_map_test",
    srcs = ["block_tag_map_test.cc"],
    deps = [
        "//audit/third_party/gtest:gtest",
        ":block_tag_map",
    ]
)

package(default_visibility=["//visibility:public"])

cc_library(
  name = "file_tag",
  srcs = ["file_tag.cc"],
  hdrs = ["file_tag.h"],
  deps = [
    "//audit:util",
    "//audit/proto:cpor",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_binary(
  name = "file_tag_test",
  srcs = ["file_tag_test.cc"],
  deps = [
    "@gtest//:main",
    ":file_tag",
    "//audit:test_util",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_library(
  name = "block_tagger",
  srcs = ["block_tagger.cc"],
  hdrs = ["block_tagger.h"],
  deps = [
    "//audit:common",
    ":file_tag",
    ":prf",
    "//audit:util",
    "//audit/proto:cpor",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ],
)

cc_binary(
  name = "block_tagger_test",
  srcs = ["block_tagger_test.cc"],
  deps = [
    "//audit:common",
    "@gtest//:main",
    ":block_tagger",
    "//audit:util",
    ":prf",
    "//audit:test_util",
    "//audit/proto:cpor",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ],
)

cc_library(
  name = "verification",
  srcs = ["verification.cc"],
  hdrs = ["verification.h"],
  deps = [
    ":prf",
    "//audit/proto:cpor",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_library(
  name = "prf",
  srcs = ["prf.cc"],
  hdrs = ["prf.h"],
  deps = [
    "//audit:common",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_binary(
  name = "prf_test",
  srcs = ["prf_test.cc"],
  deps = [
    "//audit:common",
    "@gtest//:main",
    ":prf",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ],
)

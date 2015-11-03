cc_library(
  name = "block_tagger",
  srcs = ["block_tagger.cc"],
  hdrs = ["block_tagger.h"],
  deps = [
    ":cpor_types",
    ":prf",
    ":util",
    "//audit/proto:cpor",
    "//audit/third_party/cryptopp:cryptopp",
  ],
)

cc_test(
  name = "block_tagger_test",
  srcs = ["block_tagger_test.cc"],
  deps = [
    "@gtest//:main",
    ":block_tagger",
    ":util",
    ":prf",
    "//audit/proto:cpor",
    "//audit/third_party/cryptopp:cryptopp",
  ],
)

cc_library(
  name = "util",
  srcs = ["util.cc"],
  hdrs = ["util.h"],
  deps = [
    "//audit/third_party/cryptopp:cryptopp",
  ]
)

cc_test(
  name = "util_test",
  srcs = ["util_test.cc"],
  deps = [
    "@gtest//:main",
    ":util",
    "//audit/third_party/cryptopp:cryptopp",
  ],
)

cc_library(
  name = "cpor_types",
  srcs = [],
  hdrs = ["cpor_types.h"],
  deps = [
    "//audit/third_party/cryptopp:cryptopp",
  ]
)

cc_library(
  name = "prf",
  srcs = ["prf.cc"],
  hdrs = ["prf.h"],
  deps = [
    "//audit/third_party/cryptopp:cryptopp",
  ]
)


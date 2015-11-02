cc_library(
  name = "file_tagger",
  srcs = ["file_tagger.cc"],
  hdrs = ["file_tagger.h"],
  deps = [
    ":cpor_types",
    ":prf",
    ":util",
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

cc_library(
  name = "cpor_types",
  srcs = [],
  hdrs = ["cpor_types.h"],
  deps = []
)

cc_library(
  name = "prf",
  srcs = ["prf.cc"],
  hdrs = ["prf.h"],
  deps = [
    "//audit/third_party/cryptopp:cryptopp",
  ]
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
    ":file_tagger",
    "//audit/third_party/cryptopp:cryptopp",
  ],
)

cc_test(
  name = "file_tagger_test",
  srcs = ["file_tagger_test.cc"],
  deps = [
    "@gtest//:main",
    ":file_tagger",
    "//audit/third_party/cryptopp:cryptopp",
  ],
)


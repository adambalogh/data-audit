cc_library(
  name = "file_tagger",
  srcs = ["file_tagger.cc"],
  hdrs = ["file_tagger.h"],
  deps = [
    "//audit/third_party/cryptopp:cryptopp",
  ],
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
  name = "test",
  srcs = ["file_tagger_test.cc"],
  deps = [
    "@gtest//:main",
    ":file_tagger",
  ],
)


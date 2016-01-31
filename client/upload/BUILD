package(default_visibility=["//visibility:public"])

cc_library(
  name = "file",
  srcs = ["file.cc"],
  hdrs = ["file.h"],
  deps = [
    "//audit:util",
    "//audit/proto:cpor",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_binary(
  name = "file_test",
  srcs = ["file_test.cc"],
  deps = [
    "//audit/third_party/gtest:gtest",
    ":file",
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
    ":file",
    "//audit:util",
    "//audit/client:prf",
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
    "//audit/third_party/gtest:gtest",
    ":block_tagger",
    "//audit:util",
    "//audit:test_util",
    "//audit/client:prf",
    "//audit/proto:cpor",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ],
)

cc_library(
  name = "storage",
  hdrs = ["storage.h"],
  srcs = ["storage.cc"],
  deps = [
    ":stats",
    "//audit/client:progress_bar",
    "//audit/proto:cpor",
  ]
)

cc_library(
  name = "client",
  srcs = ["client.cc"],
  hdrs = ["client.h"],
  deps = [
    ":storage",
    ":file",
    ":block_tag_serializer",
    ":block_tagger",
    "//audit/proto:cpor",
    "//audit:util",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_library(
    name = "stats",
    hdrs = ["stats.h"],
    deps = []
)

cc_library(
    name = "block_tag_serializer",
    hdrs = ["block_tag_serializer.h"],
    srcs = ["block_tag_serializer.cc"],
    deps = [
        ":file",
        "//audit/proto:cpor",
    ]
)

cc_binary(
    name = "block_tag_serializer_test",
    srcs = ["block_tag_serializer_test.cc"],
    deps = [
        ":block_tag_serializer",
        "//audit/proto:cpor",
        "//audit/third_party/gtest:gtest",
    ]
)


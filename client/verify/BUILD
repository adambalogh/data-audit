package(default_visibility=["//visibility:public"])

cc_library(
  name = "verify_file",
  srcs = ["verify_file.cc"],
  hdrs = ["verify_file.h"],
  deps = [
    "//audit/client:prf",
    "//audit:common",
    "//audit:util",
    "//audit/proto:cpor",
    "//audit/third_party/openssl:openssl",
  ]
)


cc_binary(
  name = "verify_file_test",
  srcs = ["verify_file_test.cc"],
  deps = [
    "@gtest//:main",
    ":verify_file",
    "//audit/client/upload:file",
    "//audit/client/upload:block_tagger",
    "//audit/proto:cpor",
    "//audit:test_util",
    "//audit/server:prover",
    "//audit:util",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_library(
  name = "local_disk_file_tag_source",
  srcs = ["local_disk_file_tag_source.cc"],
  hdrs = ["local_disk_file_tag_source.h"],
  deps = [
    "//audit/proto:cpor",
  ]
)

cc_library(
  name = "file_tag_source",
  hdrs = ["file_tag_source.h"],
  deps = [
  ]
)

cc_library(
  name = "client",
  srcs = ["client.cc"],
  hdrs = ["client.h"],
  deps = [
    ":verify_file",
    "//audit:util",
    "//audit/client:prf",
    "//audit/proto:cpor",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_library(
  name = "proof_source",
  hdrs = ["proof_source.h"],
  srcs = ["proof_source.cc"],
  deps = [
    "//audit/proto:cpor",
  ]
)

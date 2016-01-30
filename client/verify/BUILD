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
    "//audit/third_party/gtest:gtest",
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
    ":proof_source",
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

cc_library(
  name = "local_proof_source",
  hdrs = ["local_proof_source.h"],
  srcs = ["local_proof_source.cc"],
  deps = [
    ":proof_source",
    "//audit/proto:cpor",
    "//audit/server:prover",
    "//audit/providers/local_disk:local_disk_fetcher",
  ]
)

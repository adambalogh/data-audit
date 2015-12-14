package(default_visibility=["//visibility:public"])

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
     ":test_client",
  ]
)

cc_library(
  name = "test_client",
  srcs = ["test_client.cc"],
  hdrs = ["test_client.h"],
  deps = [
    "//audit:common",
    "//audit:util",
    "//audit:test_util",
    "//audit/client:prf",
    "//audit/client:file_tag",
    "//audit/client:block_tagger",
    "//audit/server:local_disk_fetcher",
    "//audit/client:verification",
    "//audit/server:prover",
  ]
)

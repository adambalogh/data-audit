package(default_visibility=["//visibility:public"])

cc_binary(
  name = "main",
  srcs = ["main.cc"],
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
    "//audit/client:local_disk_storage",
    "//audit/client:storage",
    "//audit/client:tagger",
    "//audit/proto:cpor",
  ]
)


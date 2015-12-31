package(default_visibility=["//visibility:public"])

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
    "//audit/third_party/openssl:openssl",
    "//audit:common",
    "//audit:util",
    "//audit:test_util",
    "//audit/client:prf",
    "//audit/client/upload:file",
    "//audit/client/upload:block_tagger",
    "//audit/server:local_disk_fetcher",
    "//audit/client/verify:verify",
    "//audit/server:prover",
    "//audit/client/upload:local_disk_storage",
    "//audit/client/upload:storage",
    "//audit/client/upload:client",
    "//audit/proto:cpor",
  ]
)


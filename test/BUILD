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
    "//audit/client/verify:local_disk_file_tag_source",
    "//audit/client/upload:file",
    "//audit/client/upload:block_tagger",
    "//audit/server:local_disk_fetcher",
    "//audit/client/verify:client",
    "//audit/client/verify:proof_source",
    "//audit/client/verify:local_proof_source",
    "//audit/client/verify:file_tag_source",
    "//audit/server:prover",
    "//audit/client/upload:local_disk_storage",
    "//audit/client/upload:storage",
    "//audit/client/upload:client",
    "//audit/proto:cpor",
  ]
)


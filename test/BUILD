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
    "//audit/client/verify:client",
    "//audit/client/verify:proof_source",
    "//audit/client/verify:file_tag_source",
    "//audit/server:prover",
    "//audit/providers/dropbox:storage",
    "//audit/providers/dropbox:fetcher",
    "//audit/providers/dropbox:file_tag_source",
    "//audit/providers/dropbox:token_source",
    "//audit/client/upload:storage",
    "//audit/client/upload:client",
    "//audit/proto:cpor",
  ]
)


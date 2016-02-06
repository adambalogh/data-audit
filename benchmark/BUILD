cc_binary(
  name = "benchmark",
  srcs = ["benchmark.cc"],
  deps = [
    "//audit/client:prf",
    "//audit/client/upload:block_tagger",
    "//audit/client/upload:file",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_binary(
    name = "verify",
    srcs = ["verify.cc"],
    deps = [
        "//audit/third_party/benchmark:benchmark",
        "//audit/client/verify:client",
        "//audit/client/verify:proof_source",
        "//audit/server:prover",
        "//audit/providers/local_disk:fetcher",
        "//audit/providers/local_disk:file_tag_source",
    ]
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//audit/third_party/benchmark:benchmark",
        "//audit/client/verify:client",
        "//audit/client/verify:proof_source",
        "//audit/server:prover",
        "//audit/client/upload:client",
        "//audit/client/upload:file",
        "//audit/providers/local_disk:fetcher",
        "//audit/providers/local_disk:file_tag_source",
        "//audit/providers/local_disk:storage",
    ]
)

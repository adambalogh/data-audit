cc_library(
    name = "dropbox_storage",
    hdrs = ["dropbox_storage.h"],
    srcs = ["dropbox_storage.cc"],
    deps = [
        "//audit/client/upload:storage",
    ]
)

cc_library(
    name = "token_source",
    hdrs = ["token_source.h"],
    srcs = ["token_source.cc"],
    deps = [
        "//audit/third_party/cpp-netlib:cpp-netlib",
    ]
)

cc_binary(
    name = "token_source_test",
    srcs = ["token_source_test.cc"],
    deps = [
        "//audit/third_party/gtest:gtest",
        ":token_source",
    ]
)

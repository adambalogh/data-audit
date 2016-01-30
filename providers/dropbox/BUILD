cc_library(
    name = "dropbox_storage",
    hdrs = ["dropbox_storage.h"],
    srcs = ["dropbox_storage.cc"],
    deps = [
        "//audit/client/upload:storage",
        "//audit/third_party/cpprestsdk:cpprestsdk",
        ":token_source",
    ]
)

cc_binary(
    name = "dropbox_storage_test",
    srcs = ["dropbox_storage_test.cc"],
    deps = [
        "//audit/third_party/gtest:gtest",
        "//audit/client/upload:storage",
        ":dropbox_storage",
    ]
)

cc_library(
    name = "token_source",
    hdrs = ["token_source.h"],
    srcs = ["token_source.cc"],
    deps = [
        "//audit/third_party/cpprestsdk:cpprestsdk",
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

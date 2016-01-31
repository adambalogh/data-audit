package(default_visibility=["//visibility:public"])

cc_library(
    name = "storage",
    hdrs = ["storage.h"],
    srcs = ["storage.cc"],
    deps = [
        "//audit/client/upload:storage",
        "//audit/third_party/cpprestsdk:cpprestsdk",
        ":dropbox_client",
        ":token_source",
    ]
)

cc_binary(
    name = "storage_test",
    srcs = ["storage_test.cc"],
    deps = [
        "//audit/third_party/gtest:gtest",
        "//audit/client/upload:storage",
        ":storage",
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

cc_library(
    name = "dropbox_client",
    hdrs = ["dropbox_client.h"],
    srcs = ["dropbox_client.cc"],
    deps = [
        "//audit/third_party/cpprestsdk:cpprestsdk",
        ":token_source",
    ]
)

cc_library(
    name = "file_tag_source",
    hdrs = ["file_tag_source.h"],
    srcs = ["file_tag_source.cc"],
    deps = [
        ":storage",
        "//audit/third_party/cpprestsdk:cpprestsdk",
        "//audit/proto:cpor",
    ]
)

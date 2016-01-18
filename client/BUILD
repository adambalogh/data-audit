package(default_visibility=["//visibility:public"])

cc_library(
  name = "prf",
  srcs = ["prf.cc"],
  hdrs = ["prf.h"],
  deps = [
    "//audit:common",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ]
)

cc_binary(
  name = "prf_test",
  srcs = ["prf_test.cc"],
  deps = [
    "//audit:common",
    "@gtest//:main",
    ":prf",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ],
)

cc_library(
    name = "file_browser",
    hdrs = ["file_browser.h"]
)

cc_library(
    name = "local_file_browser",
    hdrs = ["local_file_browser.h"],
    srcs = ["local_file_browser.cc"],
    deps = [
        ":file_browser",
        "//audit/client/upload:local_disk_storage",
    ]
)


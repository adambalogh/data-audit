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
    "//audit/third_party/gtest:gtest",
    ":prf",
    "//audit/third_party/cryptopp:cryptopp",
    "//audit/third_party/openssl:openssl",
  ],
)

cc_library(
    name = "progress_bar",
    hdrs = ["progress_bar.h"],
    deps = []
)

cc_binary(
    name = "progress_bar_test",
    srcs = ["progress_bar_test.cc"],
    deps = [
        "//audit/third_party/gtest:gtest",
        ":progress_bar",
    ]
)

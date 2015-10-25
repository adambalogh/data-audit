
cc_library(
  name = "CPORTypes",
  srcs = ["CPORTypes.cc"],
  hdrs = ["CPORTYpes.h"],
  deps = [
    "//Project/third_party/cryptopp:hmac",
    "//Project/third_party/cryptopp:integer",
    "//Project/third_party/cryptopp:sha",
  ]
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
    "//Project/third_party/cryptopp:hmac",
    ":CPOR",
  ],
)

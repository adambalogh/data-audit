cc_library(
  name = "FileProcessor",
  srcs = ["FileProcessor.cc"],
  hdrs = ["FileProcessor.h"],
  deps = [
    "//auditing/third_party/cryptopp:cryptopp",
  ]
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
    ":FileProcessor",
    "//auditing/third_party/cryptopp:cryptopp",
  ]
)


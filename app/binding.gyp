{
  "targets": [
    {
      "target_name": "native_module",
      "sources": [ 
        "cc/module.cc",
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "/usr/local/include",
        "../..",
        "/users/adambalogh/Developer",
      ],
      "link_settings": {
        "libraries": [
          "/usr/local/lib/libcryptopp.a",
          "/usr/local/lib/libprotobuf.a",
          "/usr/local/lib/libcrypto.a",
          "/usr/local/lib/libcpprest.dylib",

          "/users/adambalogh/Developer/audit/lib/libfiles.a",
          "/users/adambalogh/Developer/audit/lib/libcpor.a",
          "/users/adambalogh/Developer/audit/lib/liblocal_disk.a",
          "/users/adambalogh/Developer/audit/lib/libdropbox.a",
          "/users/adambalogh/Developer/audit/lib/libserver.a",
          "/users/adambalogh/Developer/audit/lib/libclient.a",
        ],
      },
      "conditions": [
        [ 'OS!="win"', {
          "cflags+": [ "-std=c++11" ],
          "cflags_c+": [ "-std=c++11" ],
          "cflags_cc+": [ "-std=c++11" ],
        }],
        [ 'OS=="mac"', {
          "xcode_settings": {
            "GCC_ENABLE_CPP_RTTI": "YES",
            "OTHER_CPLUSPLUSFLAGS": [
              "-std=c++11",
              "-stdlib=libc++",
              "-fexceptions",
            ],
            "OTHER_LDFLAGS": [
              "-stdlib=libc++",
              "-std=c++11",
            ],
            "MACOSX_DEPLOYMENT_TARGET": "10.11"
          },
        }],
      ],
    },
  ]
}

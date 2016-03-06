{
  "make_global_settings": [
    ['CXX','/usr/bin/clang++'],
    ['LINK','/usr/bin/clang++'],
  ],
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
          "/usr/local/lib/liblz4.a",

          "/users/adambalogh/Developer/audit/lib/libclientlib.a",
          "/users/adambalogh/Developer/audit/lib/libcporlib.a",
          "/users/adambalogh/Developer/audit/lib/libfile_browserlib.a",
          "/users/adambalogh/Developer/audit/lib/liblocal_disklib.a",
          "/users/adambalogh/Developer/audit/lib/libazurelib.a",
          "/users/adambalogh/Developer/audit/lib/libdropboxlib.a",
          "/users/adambalogh/Developer/audit/lib/libserverlib.a",
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
              "-std=c++14",
              "-stdlib=libc++",
              "-fexceptions",
            ],
            "OTHER_LDFLAGS": [
              "-stdlib=libc++",
              "-std=c++14",
            ],
            "MACOSX_DEPLOYMENT_TARGET": "10.11"
          },
        }],
      ],
    },
  ]
}

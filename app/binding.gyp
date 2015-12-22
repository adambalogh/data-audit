{
  "targets": [
    {
      "target_name": "upload",

      "sources": [ "upload.cc" ],

      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "/usr/local/include",
        "../..",
        "/users/adambalogh/Developer",
      ],

      "link_settings": {
        'libraries': [

          "/usr/local/lib/libcryptopp.a",
          "/usr/local/lib/libprotobuf.a",
          "/usr/local/lib/libcrypto.a",

          "/users/adambalogh/Developer/bazel-bin/audit/libutil.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/libfile_tag.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/liblocal_disk_storage.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/libprf.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/libblock_tagger.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/libtagger.a",
          "/users/adambalogh/Developer/bazel-bin/audit/proto/libcpor.a",
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

    }
  ]
}

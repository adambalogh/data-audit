{
  "targets": [
    {
      "target_name": "native_module",

      "sources": [ 
        "cc/upload.cc",
        "cc/verify.cc",
        "cc/login.cc",
        "cc/file_browser.cc",
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

          "/users/adambalogh/Developer/bazel-bin/audit/libutil.a",
          "/users/adambalogh/Developer/bazel-bin/audit/files/libfile_browser.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/libprf.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/libblock_tagger.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/libblock_tag_serializer.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/libclient.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/libfile.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/verify/libclient.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/verify/libverify_file.a",
          "/users/adambalogh/Developer/bazel-bin/audit/providers/dropbox/libstorage.a",
          "/users/adambalogh/Developer/bazel-bin/audit/providers/dropbox/libfetcher.a",
          "/users/adambalogh/Developer/bazel-bin/audit/providers/dropbox/libfile_tag_source.a",
          "/users/adambalogh/Developer/bazel-bin/audit/providers/dropbox/libdropbox_client.a",
          "/users/adambalogh/Developer/bazel-bin/audit/providers/dropbox/libtoken_source.a",
          "/users/adambalogh/Developer/bazel-bin/audit/providers/dropbox/libfile_list_source.a",
          "/users/adambalogh/Developer/bazel-bin/audit/proto/libcpor.a",
          "/users/adambalogh/Developer/bazel-bin/audit/server/libprover.a",
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

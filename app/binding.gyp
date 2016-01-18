{
  "targets": [

    {
      "target_name": "upload",

      "sources": [ "cc/upload.cc" ],

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
          "/users/adambalogh/Developer/bazel-bin/audit/client/libprf.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/libfile.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/liblocal_disk_storage.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/libblock_tagger.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/libclient.a",
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

    },

    {
      "target_name": "verify",

      "sources": [ "cc/verify.cc" ],

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
          "/users/adambalogh/Developer/bazel-bin/audit/client/libprf.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/verify/libclient.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/verify/liblocal_disk_file_tag_source.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/liblocal_disk_storage.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/verify/liblocal_proof_source.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/verify/libverify_file.a",
          "/users/adambalogh/Developer/bazel-bin/audit/server/liblocal_disk_fetcher.a",
          "/users/adambalogh/Developer/bazel-bin/audit/server/libprover.a",
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
    },
  
    {
      "target_name": "file_browser",

      "sources": [ "cc/file_browser.cc" ],

      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "/usr/local/include",
        "../..",
        "/users/adambalogh/Developer",
      ],

      "link_settings": {
        'libraries': [

          "/users/adambalogh/Developer/bazel-bin/audit/client/upload/liblocal_disk_storage.a",
          "/users/adambalogh/Developer/bazel-bin/audit/client/liblocal_file_browser.a",
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

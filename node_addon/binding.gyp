{
  "targets": [
    {
      "target_name": "upload",

      "sources": [ "upload.cc" ],

      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "/usr/local/include",
        "../..",
      ],

      "libraries": [
        '-lfile_tag',
        '-ltagger',
        '-lblock_tagger',
        '-lprf',
        '-llocal_disk_storage',
        '-L../../bazel-bin/audit/client',
        '-L/usr/local/lib',
        '-lcryptopp',
        '-lcrypto',
      ],

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
            "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
            "MACOSX_DEPLOYMENT_TARGET": "10.7"
          },
        }],
      ],

    }
  ]
}

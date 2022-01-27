{
  "targets": [
    {
      "target_name": "iow_sht7x",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ "./src/iow_sht7x.cpp", "./src/sht7x.cpp" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "./src/"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'link_settings': {'libraries': ['-liowkit']}
    }
  ]
}

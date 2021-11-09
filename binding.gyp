{
  'targets': [
    {
      'target_name': 'file_handler_info',
      'sources': ['src/addon.cc'],
      'include_dirs': [
        "<!@(node -p \"require('node-addon-api').include\")",
      ],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').gyp\")",
      ],
      'defines': [
        'NAPI_DISABLE_CPP_EXCEPTIONS',
        'NODE_ADDON_API_ENABLE_MAYBE',
      ],
      'conditions': [
        [
          'OS=="mac"',
          {
            'sources': ['src/impl_mac.mm'],
            'xcode_settings': {
              'OTHER_CPLUSPLUSFLAGS': ['-std=c++17'],
              'OTHER_LDFLAGS': ['-framework Foundation'],
            },
          },
          'OS=="win"',
          {
            'sources': ['src/impl_win.cc'],
            'defines': ['UNICODE'],
            'libraries': ['Shlwapi.lib'],
            'msvs_settings': {
              'VCCLCompilerTool': {
                'AdditionalOptions': [
                  '/std:c++17',
                ],
              },
            },
          },
          # else
          {
            'sources': ['src/impl_none.cc']
          }
        ],
      ],
    },
  ],
}

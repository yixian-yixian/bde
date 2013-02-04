{
  'variables': {
    'bsltf_sources': [
      'bsltf_allocbitwisemoveabletesttype.cpp',
      'bsltf_alloctesttype.cpp',
      'bsltf_bitwisemoveabletesttype.cpp',
      'bsltf_enumeratedtesttype.cpp',
      'bsltf_nonassignabletesttype.cpp',
      'bsltf_noncopyconstructibletesttype.cpp',
      'bsltf_nondefaultconstructibletesttype.cpp',
      'bsltf_nonequalcomparabletesttype.cpp',
      'bsltf_nontypicaloverloadstesttype.cpp',
      'bsltf_simpletesttype.cpp',
      'bsltf_stdtestallocator.cpp',
      'bsltf_templatetestfacility.cpp',
      'bsltf_testvaluesarray.cpp',
      'bsltf_uniontesttype.cpp',
    ],
    'bsltf_tests': [
      'bsltf_allocbitwisemoveabletesttype.t',
      'bsltf_alloctesttype.t',
      'bsltf_bitwisemoveabletesttype.t',
      'bsltf_enumeratedtesttype.t',
      'bsltf_nonassignabletesttype.t',
      'bsltf_noncopyconstructibletesttype.t',
      'bsltf_nondefaultconstructibletesttype.t',
      'bsltf_nonequalcomparabletesttype.t',
      'bsltf_nontypicaloverloadstesttype.t',
      'bsltf_simpletesttype.t',
      'bsltf_stdtestallocator.t',
      'bsltf_templatetestfacility.t',
      'bsltf_testvaluesarray.t',
      'bsltf_uniontesttype.t',
    ],
    'bsltf_tests_paths': [
      '<(PRODUCT_DIR)/bsltf_allocbitwisemoveabletesttype.t',
      '<(PRODUCT_DIR)/bsltf_alloctesttype.t',
      '<(PRODUCT_DIR)/bsltf_bitwisemoveabletesttype.t',
      '<(PRODUCT_DIR)/bsltf_enumeratedtesttype.t',
      '<(PRODUCT_DIR)/bsltf_nonassignabletesttype.t',
      '<(PRODUCT_DIR)/bsltf_noncopyconstructibletesttype.t',
      '<(PRODUCT_DIR)/bsltf_nondefaultconstructibletesttype.t',
      '<(PRODUCT_DIR)/bsltf_nonequalcomparabletesttype.t',
      '<(PRODUCT_DIR)/bsltf_nontypicaloverloadstesttype.t',
      '<(PRODUCT_DIR)/bsltf_simpletesttype.t',
      '<(PRODUCT_DIR)/bsltf_stdtestallocator.t',
      '<(PRODUCT_DIR)/bsltf_templatetestfacility.t',
      '<(PRODUCT_DIR)/bsltf_testvaluesarray.t',
      '<(PRODUCT_DIR)/bsltf_uniontesttype.t',
    ],
    'bsltf_pkgdeps': [
      '../bslalg/bslalg.gyp:bslalg',
      '../bslma/bslma.gyp:bslma',
      '../bslmf/bslmf.gyp:bslmf',
      '../bsls/bsls.gyp:bsls',
      '../bslscm/bslscm.gyp:bslscm',
    ],
  },
  'targets': [
    {
      'target_name': 'bsltf_sources',
      'type': 'none',
      'direct_dependent_settings': {
        'sources': [ '<@(bsltf_sources)' ],
        'include_dirs': [ '.' ],
      },
    },
    {
      'target_name': 'bsltf_tests_build',
      'type': 'none',
      'dependencies': [ '<@(bsltf_tests)' ],
    },
    {
      'target_name': 'bsltf_tests_run',
      'type': 'none',
      'dependencies': [ 'bsltf_tests_build' ],
      'sources': [ '<@(bsltf_tests_paths)' ],
      'rules': [
        {
          'rule_name': 'run_unit_tests',
          'extension': 't',
          'inputs': [ '<@(bsltf_tests_paths)' ],
          'outputs': [ '<(INTERMEDIATE_DIR)/<(RULE_INPUT_ROOT).t.ran' ],
          'action': [ '<(python_path)', '<(DEPTH)/tools/run_unit_tests.py',
                      '<(RULE_INPUT_PATH)',
                      '<@(_outputs)',
                      '--abi=<(ABI_bits)',
                      '--lib=<(library)'
          ],
          'msvs_cygwin_shell': 0,
        },
      ],
    },
    {
      'target_name': 'bsltf',
      'type': '<(library)',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)',
                        'bsltf_sources', ],
      'export_dependent_settings': [ '<@(bsltf_pkgdeps)' ],
      'direct_dependent_settings': { 'include_dirs': [ '.' ] },
      # Mac OS X empty LD_DYLIB_INSTALL_NAME causes executable and shared
      # libraries linking against dylib to store same path for use at runtime
      'xcode_settings': { 'LD_DYLIB_INSTALL_NAME': '' },
    },

    {
      'target_name': 'bsltf_allocbitwisemoveabletesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_allocbitwisemoveabletesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_alloctesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_alloctesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_bitwisemoveabletesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_bitwisemoveabletesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_enumeratedtesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_enumeratedtesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_nonassignabletesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_nonassignabletesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_noncopyconstructibletesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_noncopyconstructibletesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_nondefaultconstructibletesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_nondefaultconstructibletesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_nonequalcomparabletesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_nonequalcomparabletesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_nontypicaloverloadstesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_nontypicaloverloadstesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_simpletesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_simpletesttype.t.cpp' ],
    },
    {
      'target_name': 'bsltf_stdtestallocator.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_stdtestallocator.t.cpp' ],
    },
    {
      'target_name': 'bsltf_templatetestfacility.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_templatetestfacility.t.cpp' ],
    },
    {
      'target_name': 'bsltf_testvaluesarray.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_testvaluesarray.t.cpp' ],
    },
    {
      'target_name': 'bsltf_uniontesttype.t',
      'type': 'executable',
      'dependencies': [ '../bsl_deps.gyp:bsl_grpdeps',
                        '<@(bsltf_pkgdeps)', 'bsltf' ],
      'include_dirs': [ '.' ],
      'sources': [ 'bsltf_uniontesttype.t.cpp' ],
    },
  ],
}

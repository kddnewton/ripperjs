{
  "targets": [
    {
      "target_name": "ripperjs",
      "sources": ["src/ripperjs.cc"],
      "include_dirs": [
        "<!(sh -lc \"ruby -e \\\"puts RbConfig::CONFIG['rubyhdrdir']\\\"\")",
        "<!(sh -lc \"ruby -e \\\"puts RbConfig::CONFIG['rubyarchhdrdir']\\\"\")"
      ],
      "link_settings": {
        "libraries": [
          "<!(sh -lc \"ruby -e \\\"puts RbConfig::CONFIG.values_at('libdir', 'LIBRUBY').join('/')\\\"\")",
          "<!(sh -lc \"ruby -e \\\"puts RbConfig::CONFIG['LIBS']\\\"\")"
        ]
      }
    }
  ]
}

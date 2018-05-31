{
  "targets": [
    {
      "target_name": "ripperjs",
      "sources": ["src/ripperjs.cc"],
      "include_dirs": [
        "<!(bash -c \"ruby -e \\\"puts RbConfig::CONFIG['rubyhdrdir']\\\"\")",
        "<!(bash -c \"ruby -e \\\"puts RbConfig::CONFIG['rubyarchhdrdir']\\\"\")"
      ],
      "link_settings": {
        "libraries": [
          "<!(bash -c \"ruby -e \\\"puts RbConfig::CONFIG.values_at('libdir', 'LIBRUBY').join('/')\\\"\")",
          "<!(bash -c \"ruby -e \\\"puts RbConfig::CONFIG['LIBS']\\\"\")"
        ]
      }
    }
  ]
}

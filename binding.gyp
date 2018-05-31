{
  "targets": [
    {
      "target_name": "ripperjs",
      "sources": ["src/ripperjs.cc"],
      "include_dirs": [
        "<!(bin/ruby -e \"puts RbConfig::CONFIG['rubyhdrdir']\")",
        "<!(bin/ruby -e \"puts RbConfig::CONFIG['rubyarchhdrdir']\")"
      ],
      "link_settings": {
        "libraries": [
          "<!(bin/ruby -e \"puts RbConfig::CONFIG.values_at('libdir', 'LIBRUBY').join('/')\")",
          "<!(bin/ruby -e \"puts RbConfig::CONFIG['LIBS']\")"
        ]
      }
    }
  ]
}

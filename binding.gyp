{
  "targets": [
    {
      "target_name": "ripperjs",
      "sources": ["src/ripperjs.cc"],
      "include_dirs": [
        "<!(rvm default do ruby -e \"puts RbConfig::CONFIG['rubyhdrdir']\")",
        "<!(rvm default do ruby -e \"puts RbConfig::CONFIG['rubyarchhdrdir']\")"
      ],
      "link_settings": {
        "libraries": [
          "<!(rvm default do ruby -e \"puts RbConfig::CONFIG.values_at('libdir', 'LIBRUBY').join('/')\")",
          "<!(rvm default do ruby -e \"puts RbConfig::CONFIG['LIBS']\")"
        ]
      }
    }
  ]
}

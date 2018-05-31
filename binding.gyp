{
  "targets": [
    {
      "target_name": "ripperjs",
      "sources": ["src/ripperjs.cc"],
      "include_dirs": [
        "<!(which rvm && . $HOME/.rvm/scripts/rvm; ruby -e \"puts RbConfig::CONFIG['rubyhdrdir']\")",
        "<!(which rvm && . $HOME/.rvm/scripts/rvm; ruby -e \"puts RbConfig::CONFIG['rubyarchhdrdir']\")"
      ],
      "link_settings": {
        "libraries": [
          "<!(which rvm && . $HOME/.rvm/scripts/rvm; ruby -e \"puts RbConfig::CONFIG.values_at('libdir', 'LIBRUBY').join('/')\")",
          "<!(which rvm && . $HOME/.rvm/scripts/rvm; ruby -e \"puts RbConfig::CONFIG['LIBS']\")"
        ]
      }
    }
  ]
}

{
  "targets": [
    {
      "target_name": "ripperjs",
      "sources": ["src/ripperjs.cc"],
      "include_dirs": [
        "<!(which rvm && source $HOME/.rvm/scripts/rvm; ruby -e \"puts RbConfig::CONFIG['rubyhdrdir']\")",
        "<!(which rvm && source $HOME/.rvm/scripts/rvm; ruby -e \"puts RbConfig::CONFIG['rubyarchhdrdir']\")"
      ],
      "link_settings": {
        "libraries": [
          "<!(which rvm && source $HOME/.rvm/scripts/rvm; ruby -e \"puts RbConfig::CONFIG.values_at('libdir', 'LIBRUBY').join('/')\")",
          "<!(which rvm && source $HOME/.rvm/scripts/rvm; ruby -e \"puts RbConfig::CONFIG['LIBS']\")"
        ]
      }
    }
  ]
}

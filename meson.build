project('pacman', 'c',
  default_options: ['c_std=c2x', 'warning_level=3'],
  version: '1.alpha.1'
)

include = include_directories('include')

subdir('src')

ncursesw = dependency('ncursesw', required: true)

pacman = executable('pacman', sources,
  include_directories: include,
  dependencies: ncursesw
)

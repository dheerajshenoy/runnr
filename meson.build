project('Runnr', 'cpp',
  default_options: ['cpp_std=c++20',
    'buildtype=debug', 
    'cpp_args=-Wall -Wextra -Wpedantic -g3 -O0 -DDEBUG'
  ]
       )

raylib_dep = dependency('raylib', required: true)

sources = files(
    'src/main.cpp',
    'src/Runnr.cpp',
    'src/Player.cpp',
    'src/Floor.cpp',
    'src/MenuScene.cpp',
    'src/GameScene.cpp',
    'src/Button.cpp',
    'src/GameOverScene.cpp'

)

executable('runnr',
           sources: sources,
           dependencies: [
               raylib_dep
           ])

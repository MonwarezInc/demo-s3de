project('DemoSimple3DEngine','cpp', default_options : ['cpp_std=c++1z'])


simple3dengine_dep = dependency('simple3dengine',fallback : ['simple3dengine','simple3dengine_dep'])

simple3dengine_window_sdl2_dep = dependency('simple3dengine_window_sdl2',
                                            fallback: ['simple3dengine',
                                                      'simple3dengine_window_sdl2_dep'])

simple3dengine_input_sdl2_dep = dependency('simple3dengine_input_sdl2',
                                            fallback: ['simple3dengine',
                                                       'simple3dengine_input_sdl2_dep'])

executable('DemoSimple3DEngine',
           'src/main.cpp',
           dependencies:[simple3dengine_dep,
                         simple3dengine_window_sdl2_dep,
                         simple3dengine_input_sdl2_dep,],
           cpp_args: ['-DS3DE_USE_DSA=1'],
          )


cppcheck_target = run_target('cppcheck',command : ['tools/cppcheck.sh'])

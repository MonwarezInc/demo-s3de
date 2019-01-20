project('DemoSimple3DEngine','cpp', default_options : ['cpp_std=c++1z'])


simple3dengine_dep = dependency('simple3dengine',fallback : ['simple3dengine','simple3dengine_dep'])

executable('DemoSimple3DEngine',
           'src/main.cpp',
           dependencies:[simple3dengine_dep],
           cpp_args: ['-DS3DE_USE_DSA=1'],
          )


cppcheck_target = run_target('cppcheck.demo',command : ['tools/cppcheck.sh'])

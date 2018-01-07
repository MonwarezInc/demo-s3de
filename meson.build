project('DemoSimple3DEngine','cpp', default_options : ['cpp_std=c++1z'])

sdl2_dep = dependency('sdl2',required:true)
sdl2image_dep = dependency('SDL2_image',required:true)
assimp_dep = dependency('assimp',required:true)
glew_dep = dependency('glew',required:true)
glm_dep = dependency('glm',required:true)


simple3dengine_dep = dependency('simple3dengine',fallback : ['simple3dengine','simple3dengine_dep'])

executable('DemoSimple3DEngine','src/main.cpp',
    dependencies: [sdl2_dep,
                   sdl2image_dep,
                   assimp_dep,
                   glew_dep,
                   glm_dep,
                   simple3dengine_dep])


cppcheck_target = run_target('cppcheck.demo',command : ['tools/cppcheck.sh'])

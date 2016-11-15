cmake_minimum_required(VERSION 3.3)

#Configuration of project
#for the moment we will just do the exec (library will come later)
project(TestEngine)

set(EXECUTABLE_OUTPUT_PATH ./bin/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(${include_s3de})
link_directories(${lib_s3de})
# Add prefix library
link_directories(${CMAKE_PREFIX}/lib)

#Generate source file list
file(
    GLOB_RECURSE
    source_file
    src/*
)
set(Extra_CXX_Flags "-Wall -std=c++1z")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Extra_CXX_Flags}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(GL GL)
set(WITH_ASSERT TRUE CACHE BOOL "Enable Assert")
if (win)
    set(GL opengl32)
endif ()
#Declaration of executable
add_executable(
    TestEngine
    ${source_file}
)

#Configuration of link resolution
target_link_libraries(
    TestEngine
    Simple3DEngine
)
if(UNIX)
    include(FindPkgConfig)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image >=2.0.0)
    PKG_SEARCH_MODULE(ASSIMP REQUIRED assimp >= 3.0)
    PKG_SEARCH_MODULE(GLEW REQUIRED glew)
else ()
    message("Please Define the followings path")
    message("SDL2_INCLUDE_DIRS, SDL2IMAGE_INCLUDE_DIRS")
    message("SDL2_LIBRARIES, SDL2IMAGE_LIBRARIES")
    message("ASSIMP_INCLUDE_DIRS, ASSIMP_LIBRARIES")
    message("GLEW_INCLUDE_DIRS, GLEW_LIBRARIES")
endif ()
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

if (${WITH_ASSERT})
    message("Assert Enabled")
else()
    message("Assert Disabled")
    add_definitions(-DNDEBUG)
endif()

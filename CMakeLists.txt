cmake_minimum_required(VERSION 3.3)

#Configuration of project
#for the moment we will just do the exec (library will come later)
project(TestEngine)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXECUTABLE_OUTPUT_PATH ./bin/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(${include_s3de})
link_directories(${lib_s3de})
# Add prefix library
link_directories(${CMAKE_PREFIX}/lib)

option(asan "Build with asan enabled" ON)

#Generate source file list
set(
    source_file
    src/main.cpp
)
set(Extra_CXX_Flags "-Wall -std=c++1z")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Extra_CXX_Flags}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(GL GL)

if (win)
    set(GL opengl32)
endif ()
#Declaration of executable
add_executable(
    TestEngine
    ${source_file}
)

#Configuration of link resolution
if((${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
    target_link_libraries(${PROJECT_NAME} Simple3DEngined)
else()
    target_link_libraries(${PROJECT_NAME} Simple3DEngine)
endif()

if (asan)
  add_definitions("-fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

include(common.cmake)


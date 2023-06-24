cmake_minimum_required(VERSION 3.16.3)
project(tiny_libc C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_COMPILER_NAMES clang)
# set( CMAKE_VERBOSE_MAKEFILE on )

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(isDebug TRUE)
else()
  set(isDebug FALSE)
endif()

message("isDebug: ${isDebug}")

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(COMPILER_NAME "clang")
  set(COMPILER_CLANG 1)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(COMPILER_NAME "gcc")
  set(COMPILER_GCC 1)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(COMPILER_NAME "msvc")
  set(COMPILER_MSVC 1)
endif()

set(useSanitizers ${isDebug})
message("useSanitizers: ${useSanitizers}")

if(useSanitizers)
  message("using sanitizers")
  set(sanitizers "-fsanitize=address")
endif()

if(APPLE)
  add_compile_definitions(TORNADO_OS_MACOS)
  set(OS_MACOS 1)
elseif(UNIX)
  add_compile_definitions(TORNADO_OS_LINUX)
  set(OS_LINUX 1)
elseif(WIN32)
  add_compile_definitions(TORNADO_OS_WINDOWS)
  set(OS_WINDOWS 1)

endif()

if(isDebug)
  message("Debug build detected")
  set(CONFIGURATION_DEBUG 1)
endif()

include_directories(include)

file(GLOB_RECURSE lib_src FOLLOW_SYMLINKS "lib/*.c")

function(unixlike)
  message("unixlike")
endfunction()

if(OS_LINUX)
  message("Linux Detected!")
  unixlike()

elseif(OS_MACOS)
  message("MacOS detected!")
  unixlike()
endif()

add_library(tiny_libc ${lib_src})

set_target_properties(tiny_libc PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                           "${CMAKE_BINARY_DIR}/bin")

if(COMPILER_CLANG)
  target_compile_options(
    tiny_libc
    PRIVATE -Weverything -Werror -Wno-padded
            -Wno-declaration-after-statement # must be bug in clang? shows up
                                             # even if std=c99
  )
elseif(COMPILER_MSVC)
  target_compile_options(
    tiny_libc
    PRIVATE /F
            5000000
            /Wall
            /wd4668
            /wd4820
            /wd4100
            /wd4710
            /wd4711
            /wd5945
            /wd5105
            /wd5045
            /we4047
            /we4255
            /we4013
            /we4020
            /we4474
            /we4477
            /we4057
            /we4101
            /we4189) # /WX
else()
  target_compile_options(tiny_libc PRIVATE -Wall)
endif()

if(useSanitizers)
  target_link_libraries(tiny_libc ${sanitizers})
endif()

target_compile_options(tiny_libc PRIVATE -Wall -Wno-unused-variable
                                         ${sanitizers})

if(NOT isDebug)
  message("optimize!")
  target_compile_options(tiny_libc PRIVATE -O3) # -flto file format no
                                                # recognized
endif()

target_link_libraries(tiny_libc m)

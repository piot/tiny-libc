cmake_minimum_required(VERSION 3.16.3)
project(tiny_libc C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_COMPILER_NAMES clang)
# set( CMAKE_VERBOSE_MAKEFILE on )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(isDebug TRUE)
else()
    set(isDebug FALSE)
endif()

message("isDebug: ${isDebug}")

set(useSanitizers ${isDebug})
message("useSanitizers: ${useSanitizers}")

if (useSanitizers)
    message("using sanitizers")
    set(sanitizers "-fsanitize=address")
endif()

if (APPLE)
        add_compile_definitions(TORNADO_OS_MACOS)
        set(OS_MACOS 1)
elseif (UNIX)
        add_compile_definitions(TORNADO_OS_LINUX)
        set(OS_LINUX 1)
elseif (WIN32)
        add_compile_definitions(TORNADO_OS_WINDOWS)
        set(OS_WINDOWS 1)

endif()

if (isDebug)
        message("Debug build detected")
        set(CONFIGURATION_DEBUG 1)
endif()

add_compile_definitions(_POSIX_C_SOURCE=200112L)
add_compile_options(-Wall -Wextra -Wshadow -Wstrict-aliasing -ansi -pedantic -Wno-unused-function -Wno-unused-parameter)

include_directories(include)

file(GLOB_RECURSE lib_src FOLLOW_SYMLINKS
        "lib/*.c"
        )

function(unixlike)
        message("unixlike")
endfunction()

if (OS_LINUX)
        message("Linux Detected!")
        unixlike()

elseif (OS_MACOS)
        message("MacOS detected!")
        unixlike()
endif()


add_library(tiny_libc
        ${lib_src}
)

set_target_properties(tiny_libc
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (useSanitizers)
    target_link_libraries(tiny_libc ${sanitizers})
endif()


target_compile_options(tiny_libc PRIVATE -Wall -Wno-unused-variable ${sanitizers})

if (NOT isDebug)
    message("optimize!")
    target_compile_options(tiny_libc PRIVATE -O3) #  -flto file format no recognized
endif()

target_link_libraries(tiny_libc m)

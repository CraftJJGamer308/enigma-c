cmake_minimum_required(VERSION 3.28)

project(enigma-c)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(main)

# Compile every .c file in src/
file(GLOB SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

target_sources(main PRIVATE ${SRC_FILES})

# Headers are in src/
target_include_directories(main PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(main PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Standard
target_compile_options(main PRIVATE
    $<$<CONFIG:Standard>:-O3>
)

# Show Internal
target_compile_definitions(main PRIVATE
    $<$<CONFIG:ShowInternal>:SHOW_INTERNAL>
)
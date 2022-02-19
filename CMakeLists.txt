cmake_minimum_required(VERSION 3.18)

project(bbmenu VERSION 0.0.1 LANGUAGES C)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(
    -Werror
    -Wall
)

# Add files to project
file(GLOB PROJECT_SOURCES
    main.c
    BBmenu.c
    BBmenu.h
    BBmenuPrivate.h
    BBmenuDefs.h
    menu.uic
)

# Tell CMake to create the project executable
add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

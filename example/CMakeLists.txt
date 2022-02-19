cmake_minimum_required(VERSION 3.18)

project(bbmenu VERSION 0.0.1 LANGUAGES C)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(
    -Werror
    -Wall
)

# Command for copy menu init files to binary directory
add_custom_target(copy_menu_init_file
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_LIST_DIR}/menu.uic
    ${CMAKE_CURRENT_BINARY_DIR}/menu.uic
)

# Add files to project
file(GLOB PROJECT_SOURCES
    main.c
    menu.uic
    cmd_list_menu.h
    cmd_list_menu.c
    ../src/BBmenu.c
    ../src/BBmenu.h
    ../src/BBmenuPrivate.h
    ../src/BBmenuDefs.h
)

# Tell CMake to create the project executable
add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
# Tell CMake that we need the menu init files to build the project
add_dependencies(${CMAKE_PROJECT_NAME} copy_menu_init_file)

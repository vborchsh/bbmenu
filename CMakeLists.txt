cmake_minimum_required(VERSION 3.18)

project(bbmenu VERSION 0.0.1 LANGUAGES C)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc/resource/ui automatically when needed.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#add_definitions(-DQT_NO_DEBUG_OUTPUT)
#add_definitions(-DQT_NO_INFO_OUTPUT)

add_compile_options(
    #-Werror
    -Wall
)

# Add files to project
file(GLOB PROJECT_SOURCES
    main.c
    BBmenu.c
    BBmenu.h
    BBmenuPrivate.h
    BBmenuDefs.h
    )

# Tell CMake to create the project executable
add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

# Disable console when we run release on Windows
#if(CMAKE_BUILD_TYPE STREQUAL "Release")
#    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true)
#endif()

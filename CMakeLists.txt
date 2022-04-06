cmake_minimum_required(VERSION 3.20)
project(c_gui C)

set(CMAKE_C_STANDARD 99)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_executable(c_gui main.c sudo.c)
target_link_libraries(c_gui
        ${GTK4_LIBRARIES})

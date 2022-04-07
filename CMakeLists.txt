cmake_minimum_required(VERSION 3.20)
project(c_gui C)

set(CMAKE_C_STANDARD 99)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/builder.glade 
${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sudo.css
${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(c_gui main.c shuffle.c sudo.c)
target_link_libraries(c_gui
        ${GTK3_LIBRARIES})

#Author - Manjot Bal
#Date   - Jan 22, 2018

#This is a simple cmake file that can be used for 
#OpenGL and GLFW projects. Has been tested to work 
#on linux machines in MS.

#To use make sure to follow these requirments
#   Place this file in your folders root directory
#   - create a directory in the projects root folder called "src" which
#     holds all the source files (*.cpp)
    
#   - create a directory in the projects root folder called "inc" which 
#     holds all the header files (*.h)

#   - create a directory in the projects root folder called "rsc" which 
#     holds all the resource files (*.txt, *.glsl, *.jpg, ..etc)

#   - create a folder called "build" which be the folder that this
#     cmake file is run from

#How to use this file
#   - go into the "build" directory from the root directory
#   - run the command "cmake .."
#   - run the command "make"
#   - run the command "./<executable>" where executable is the name of
#     the executable file


cmake_minimum_required(VERSION 3.10.1)
project(springs)

# Change what is in the quotes to change exec name
set(EXEC "springs")

set (CMAKE_CXX_FLAGS "-Wall -g")
file(GLOB SOURCES "src/*.cpp")
add_executable(${EXEC} ${SOURCES})

#Find and link OpenGl and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 3.2 REQUIRED)

if(NOT ${OPENGL_FOUND})
    message("OpenGL not found")
endif()

include_directories(inc ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${EXEC} ${OPENGL_gl_LIBRARIES} glfw dl)

#Copy resource folder to build directory
# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    set (rsc "${CMAKE_SOURCE_DIR}/rsc")
    set (destination "${CMAKE_CURRENT_BINARY_DIR}/rsc")
    add_custom_command (
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${rsc} ${destination}
        DEPENDS ${destination}
        COMMENT "symbolic link resources folder from ${rsc} => ${destination}"
    )
endif()

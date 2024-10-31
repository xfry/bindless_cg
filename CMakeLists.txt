cmake_minimum_required(VERSION 3.16)

# Project name and version
project(Bindless-CG VERSION 1.0 LANGUAGES C CXX)  # Enable both C and CXX languages

# Set C and C++ compilers
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/dependencies)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/glad)
include_directories(${PROJECT_SOURCE_DIR}/src)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS 
                            system 
                            filesystem)
find_package(GLUT REQUIRED)

# Add executable
add_executable( Bindless-CG 
                src/main.cpp
                dependencies/glad/glad.h
                src/glad.c
                )

# Link libraries
target_link_libraries(Bindless-CG OpenGL::GL glfw Boost::system Boost::filesystem GLUT::GLUT)
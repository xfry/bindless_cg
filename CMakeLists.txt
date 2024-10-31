cmake_minimum_required(VERSION 3.16)

# Set the project name and version
project(Bindless-CG VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source and header directories
include_directories(${PROJECT_SOURCE_DIR}/header)
add_subdirectory(src)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Add the executable
add_executable(Bindless-CG main.cpp)

# Add any additional directories or libraries here
target_include_directories(Bindless-CG PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Bindless-CG PRIVATE OpenGL::GL GLUT::GLUT)
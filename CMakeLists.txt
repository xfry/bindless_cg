cmake_minimum_required(VERSION 3.10)
project(bindless_cg)

# Especificar el estándar de C++
set(CMAKE_CXX_STANDARD 17)

# Incluir directorios de encabezados
include_directories(include)

# Archivos fuente
file(GLOB SOURCES "src/*.cpp")

# Crear el ejecutable
add_executable(bindless_cg ${SOURCES})

# Vincular las librerías necesarias
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(bindless_cg OpenGL::GL glfw GLEW::GLEW)

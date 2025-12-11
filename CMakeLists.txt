cmake_minimum_required(VERSION 4.0)
project(tsbk03_project)

set(CMAKE_CXX_STANDARD 20)
add_executable(tsbk03_project main.cpp
        shader.cpp
        shader.h)

find_package(glad CONFIG REQUIRED)
target_link_libraries(tsbk03_project PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(tsbk03_project PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(tsbk03_project PRIVATE glm::glm)
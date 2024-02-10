cmake_minimum_required(VERSION 3.20)
project(blife)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/binRelease)


add_executable(${PROJECT_NAME}
				src/main.cpp
				src/Screen.hpp
				src/Screen.cpp
				)
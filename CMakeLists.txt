# CMake Version
cmake_minimum_required(VERSION 3.3)

# Constants
set(PROJECT_NAME "tmc")

# Project Name
project(${PROJECT_NAME})

# Executable
add_executable(
  ${PROJECT_NAME}
  src/Main.c
)
target_include_directories(${PROJECT_NAME} PUBLIC src)

# CMake Version
cmake_minimum_required(VERSION 3.3)

# Constants
set(PROJECT_NAME "tmc")

# Project Name
project(${PROJECT_NAME})

# Globs
file(GLOB HEADERS_FILES "src/Headers/*.h")
file(GLOB SOURCES_FILES "src/Sources/*.c")

# Executable
add_executable(
  ${PROJECT_NAME}
  src/Main.c
  ${HEADERS_FILES}
  ${SOURCES_FILES}
)
target_include_directories(${PROJECT_NAME} PUBLIC src)

cmake_minimum_required(VERSION 3.28)

project(noshell VERSION 0.1.0 LANGUAGES C)

add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Wshadow
)

add_executable(${PROJECT_NAME} main.c)

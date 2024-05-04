cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_STANDARD 23)

project(noshell VERSION 0.1.0 LANGUAGES C)

add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Wshadow
)


set(DEFAULT_SHELL "/bin/sh" CACHE STRING "Default shell to use as a fallback")

configure_file(noshell.h.in ${CMAKE_CURRENT_SOURCE_DIR}/noshell.h @ONLY)

add_executable(${PROJECT_NAME} main.c)

install(TARGETS ${PROJECT_NAME})

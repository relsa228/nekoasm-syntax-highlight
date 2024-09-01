cmake_minimum_required(VERSION 3.5.0)
project(nekoasm-syntax-highlihgt VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)


## collect lib sources by patterns

file(GLOB helloworld_SRC
     "*.h"
     "*.cpp"
     "./bases/lexical/*.cpp"
     "./bases/lexical/*.h"
     "./services/*.cpp"
     "./services/*.h"
)

add_executable(nekoasm-syntax-highlihgt main.cpp ${helloworld_SRC})


cmake_minimum_required(VERSION 3.14)
project(audecous)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3 -pthread")

add_executable(audecous main.cpp StepMemos.cpp StepMemos.h Memo.cpp Memo.h)
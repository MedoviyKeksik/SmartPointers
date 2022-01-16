cmake_minimum_required(VERSION 3.22)
project(SmartPointers)

set(CMAKE_CXX_STANDARD 20)

add_executable(SmartPointers main.cpp UniquePtr.h SharedPtr.h DefaultDelete.h)

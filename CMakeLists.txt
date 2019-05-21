cmake_minimum_required(VERSION 3.13)
project(nasp_tsp_greedy)

set(CMAKE_CXX_STANDARD 14)

add_executable(nasp_tsp_greedy main.cpp graf.h netezinski_graf.h greedy.h lista.h)
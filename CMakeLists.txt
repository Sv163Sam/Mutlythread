cmake_minimum_required(VERSION 3.23)
project(_3)

set(CMAKE_CXX_STANDARD 14)

add_executable(_3 main.cpp task_1.cpp task_2_l.cpp task_1.h task_2_l.h task_2_h.cpp task_2_m.cpp result.cpp result.h)

find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

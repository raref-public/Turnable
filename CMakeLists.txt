cmake_minimum_required(VERSION 3.30)
project(Turnable)

set(CMAKE_CXX_STANDARD 17)
# find ncurses
find_package(Curses REQUIRED)

add_executable(Turnable main.cpp buffer.cpp)


# Link ncurses to your executable
target_link_libraries(Turnable PRIVATE ${CURSES_LIBRARIES})
target_include_directories(Turnable PRIVATE ${CURSES_INCLUDE_DIRS})

target_include_directories(Turnable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
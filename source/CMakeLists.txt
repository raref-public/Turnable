cmake_minimum_required(VERSION 3.30)
project(Turnable)

set(CMAKE_CXX_STANDARD 17)

# SFML ROOT
set(SFML_ROOT "/usr")

# find ncurses
find_package(Curses REQUIRED)
# find sfml
find_package(SFML 2.6 COMPONENTS audio system REQUIRED)

add_executable(Turnable main.cpp buffer/buffer.cpp sounds/sounds.cpp)


# Link ncurses to your executable
target_link_libraries(Turnable PRIVATE ${CURSES_LIBRARIES} sfml-audio sfml-system)
target_include_directories(Turnable PRIVATE ${CURSES_INCLUDE_DIRS})

target_include_directories(Turnable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link SFML libraries
# Add include directories explicitly (not always necessary)
include_directories(${SFML_INCLUDE_DIR})
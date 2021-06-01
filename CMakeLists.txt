cmake_minimum_required(VERSION 3.16)

include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.251.tar.gz"
        SHA1 "5659b15dc0884d4b03dbd95710e6a1fa0fc3258d"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(WarShips)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_library(${PROJECT_NAME} STATIC
        sources/PlayerField.cpp
        sources/AIField.cpp
        sources/AI.cpp
        sources/PlacingShips.cpp
        sources/Menu.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>")

add_executable(demo demo/main.cpp)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-config
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )

target_link_libraries(demo ${CURSES_LIBRARIES})
target_link_libraries(demo ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.1)

project(untitled LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)
file(GLOB_RECURSE source_list "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_executable(${PROJECT_NAME} "main.cpp" "qml.qrc" ${source_list})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)

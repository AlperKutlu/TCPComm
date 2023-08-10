project(client)
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

find_package(jsoncpp REQUIRED)

add_executable(
    client
    src/client.cpp
)
target_link_libraries(client jsoncpp_lib)
target_link_directories(client PUBLIC ${jsoncpp_INCLUDE_DIRS})
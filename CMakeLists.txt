project(client)
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

find_package(jsoncpp REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(
    client
    src/client.cpp
)

target_link_libraries(client jsoncpp_lib nlohmann_json::nlohmann_json)
target_link_directories(client PUBLIC ${jsoncpp_INCLUDE_DIRS} ${nlohmann_json_INCLUDE_DIRS})
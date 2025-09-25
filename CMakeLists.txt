cmake_minimum_required(VERSION 3.19)
project(friendgd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/FriendSimulation.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${GEODE_SDK_PATH}/bindings
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    geode-sdk
)

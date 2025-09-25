cmake_minimum_required(VERSION 3.19)
project(friendgd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED GEODE_SDK_PATH AND DEFINED ENV{GEODE_SDK_PATH})
    set(GEODE_SDK_PATH "$ENV{GEODE_SDK_PATH}")
endif()

if(NOT GEODE_SDK_PATH)
    message(FATAL_ERROR "GEODE_SDK_PATH is not set. Set the variable or install the SDK with 'geode setup sdk'.")
endif()

if(NOT EXISTS "${GEODE_SDK_PATH}/cmake/Geode.cmake")
    message(FATAL_ERROR "Could not find Geode SDK CMake helpers in ${GEODE_SDK_PATH}. Make sure the SDK is installed correctly.")
endif()

include("${GEODE_SDK_PATH}/cmake/Geode.cmake")

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/FriendSimulation.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${GEODE_SDK_PATH}/bindings
)

setup_geode_mod(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    geode-sdk
)

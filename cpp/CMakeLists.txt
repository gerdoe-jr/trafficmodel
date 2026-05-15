cmake_minimum_required(VERSION 3.10)

project(trafficmodel VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE TRAFFIC_SRC src/*)

add_executable(${PROJECT_NAME} ${TRAFFIC_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)
target_compile_options(${PROJECT_NAME}
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

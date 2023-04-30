cmake_minimum_required(VERSION 3.0.0)
project(unit_test VERSION 0.1.0)

include(CPack)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ManagedArr)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Rope)


add_library(ManagedArr ManagedArr/ManagedArr.cpp)
add_library(Rope Rope/Rope.cpp)
add_executable(unit_test Rope/main.cpp)

target_include_directories(Rope      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ManagedArr)
target_include_directories(unit_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ManagedArr)
target_include_directories(unit_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Rope)

target_link_libraries(unit_test PRIVATE Rope)

cmake_minimum_required(VERSION 3.25)
project(track)

file(GLOB_RECURSE ${PROJECT_NAME}_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${PROJECT_NAME} OBJECT ${${PROJECT_NAME}_src})

target_compile_features(${PROJECT_NAME} PUBLIC c_std_99 cxx_std_20)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE sensors)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../sensors/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE utility)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../utility/inc)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../threads/inc)

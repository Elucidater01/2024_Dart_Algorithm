cmake_minimum_required(VERSION 3.25)
project(threads)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#file(GLOB_RECURSE ${PROJECT_NAME}_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
#message(STATUS "src files: ${${PROJECT_NAME}_src}")

#add_library(${PROJECT_NAME} OBJECT ${${PROJECT_NAME}_src})
#
#target_compile_features(${PROJECT_NAME} PUBLIC c_std_99 cxx_std_20)

#target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

cmake_minimum_required(VERSION 3.25)
project(Dart)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_NAME}_src)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_src})

add_subdirectory(src/utility)
add_subdirectory(src/capture)
add_subdirectory(src/sensors)
add_subdirectory(src/detect)
add_subdirectory(src/track)
add_subdirectory(src/mouse)


target_link_libraries(${PROJECT_NAME} PRIVATE utility)
target_include_directories(${PROJECT_NAME} PRIVATE src/utility/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE capture)
target_include_directories(${PROJECT_NAME} PRIVATE src/capture/inc)

target_include_directories(${PROJECT_NAME} PRIVATE src/threads/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE sensors)
target_include_directories(${PROJECT_NAME} PRIVATE src/sensors/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE detect)
target_include_directories(${PROJECT_NAME} PRIVATE src/detect/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE track)
target_include_directories(${PROJECT_NAME} PRIVATE src/track/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE mouse)
target_include_directories(${PROJECT_NAME} PRIVATE src/mouse/inc)


target_link_libraries(${PROJECT_NAME} PRIVATE -lpthread)
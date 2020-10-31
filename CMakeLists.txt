project(CONTAIN CXX)
cmake_minimum_required(VERSION 3.0.0)

add_subdirectory(extlib/)

file(GLOB CONTAIN_src "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(CONTAIN_exe WIN32 ${CONTAIN_src})
target_include_directories(CONTAIN_exe PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(CONTAIN_exe PRIVATE ${PROJECT_SOURCE_DIR}/extlib/EIGEN)
target_link_libraries(CONTAIN_exe sfml-graphics sfml-window sfml-system sfml-audio)
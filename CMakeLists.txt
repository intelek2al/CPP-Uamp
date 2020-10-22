cmake_minimum_required(VERSION 3.17)

project(uamp VERSION 1.0 LANGUAGES CXX)

set(TAGLIB_DIR ../3dparty/taglib/build)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(3dparty)
add_subdirectory(app)

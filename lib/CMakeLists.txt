cmake_minimum_required(VERSION 3.24)
project(VoxymoreCore)

add_subdirectory(glad)
target_link_libraries(VoxymoreCore PUBLIC glad)
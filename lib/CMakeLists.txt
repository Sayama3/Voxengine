cmake_minimum_required(VERSION 3.24)

project(VoxymoreCore)

add_subdirectory(glad)

add_subdirectory(glfw)

add_subdirectory(glm)

add_subdirectory(spdlog)

set(ASSIMP_WARNINGS_AS_ERRORS off)

add_subdirectory(assimp)

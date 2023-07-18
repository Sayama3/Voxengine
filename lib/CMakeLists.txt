cmake_minimum_required(VERSION 3.24)

add_subdirectory(glad)

add_subdirectory(glfw)

add_subdirectory(glm)

add_subdirectory(spdlog)

#set(IMGUI_GLFW ON)
set(IMGUI_OPENGL3 ON)
add_subdirectory(imgui)
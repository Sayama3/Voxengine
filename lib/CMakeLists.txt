cmake_minimum_required(VERSION 3.16)

include(FetchContent)

add_subdirectory(glad)

add_subdirectory(glfw)

set(GLM_ENABLE_CXX_20 ON)
add_subdirectory(glm)

add_subdirectory(spdlog)

set(IMGUI_GLFW ON)
set(IMGUI_OPENGL3 ON)
set(IMGUI_OPENGL3_LOADER ON)
add_subdirectory(imgui)
# Need to say to imgui where is glfw
target_link_libraries(imgui PUBLIC glfw)

add_subdirectory(entt)

add_subdirectory(yaml-cpp)

add_subdirectory(ImGuizmo)
target_link_libraries(ImGuizmo PUBLIC imgui)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
add_subdirectory(tracy)

add_subdirectory(hash-library)

#TODO: use "Fetch Content" instead of git submodules.

FetchContent_Declare(
    jolt
    GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics
    GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(jolt)


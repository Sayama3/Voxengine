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

add_subdirectory(hash-library)

#TODO: use "Fetch Content" instead of git submodules.

set(CROSS_PLATFORM_DETERMINISTIC ON)
set(ENABLE_ALL_WARNINGS OFF)

FetchContent_Declare(
        jolt
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics.git"
        GIT_TAG "v5.0.0"
        SOURCE_SUBDIR "Build"
)

set(TRACY_ENABLE ON)
set(TRACY_STATIC ON)
set(TRACY_ON_DEMAND ON)

FetchContent_Declare(
        Tracy
        GIT_REPOSITORY "https://github.com/wolfpld/tracy.git"
        GIT_TAG "v0.11.0"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(jolt Tracy)

set_target_properties(Jolt PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)
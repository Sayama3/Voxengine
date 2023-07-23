cmake_minimum_required(VERSION 3.24)

message("Voxymore::Core is a STATIC library.")

add_subdirectory(lib)

project(Voxymore VERSION 0.0.1 DESCRIPTION "The core library of the Voxymore Game Engine.")
set(LIBRARY_TARGET_NAME Core)

set(CMAKE_CXX_STANDARD 20)

option(VoxymoreCore_SHARED_BUILD "The ${VoxymoreCore} lib in shared mode" OFF)

set(SRC_FILES src/Application.cpp
        src/Math.cpp
        src/Logger.cpp
        include/Voxymore/Voxymore.hpp
        include/Voxymore/Application.hpp
        include/Voxymore/EntryPoint.hpp
        include/Voxymore/Logger.hpp
        include/Voxymore/Math.hpp
        include/Voxymore/stb_image.h
        include/Voxymore/SystemHelper.hpp
        include/Voxymore/Events/Event.hpp
        include/Voxymore/Events/KeyEvent.hpp
        include/Voxymore/Events/MouseEvent.hpp
        include/Voxymore/Events/ApplicationEvent.hpp
        include/Voxymore/Window.hpp
        include/Voxymore/Macros.hpp
        src/Layers/Layer.cpp
        include/Voxymore/Layers/Layer.hpp
        src/Layers/LayerStack.cpp
        include/Voxymore/Layers/LayerStack.hpp
        src/ImGui/ImGUILayer.cpp
        include/Voxymore/ImGUI/ImGUILayer.hpp
        include/Voxymore/Input.hpp
        include/Voxymore/Core.hpp
        include/Voxymore/KeyCodes.hpp
        include/Voxymore/MouseButtonCodes.hpp
        include/Voxymore/Renderer/Buffer.hpp
        src/Renderer/Buffer.cpp
        src/Renderer/Renderer.cpp
        include/Voxymore/Renderer/Renderer.hpp
        include/Voxymore/Renderer/Shader.hpp
        src/Renderer/Shader.cpp
        src/Renderer/VertexArray.cpp
        include/Voxymore/Renderer/VertexArray.hpp
        src/Renderer/RendererAPI.cpp
        include/Voxymore/Renderer/RendererAPI.hpp
        include/Voxymore/Renderer/RenderCommand.hpp
        src/Renderer/RenderCommand.cpp
        include/Voxymore/Renderer/Camera.hpp
        include/Voxymore/Renderer/PerspectiveCamera.hpp
        src/Renderer/PerspectiveCamera.cpp
        )

add_library(${LIBRARY_TARGET_NAME} STATIC ${SRC_FILES})

IF(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    add_compile_definitions(VXM_Debug)
    add_compile_definitions(Debug)
ENDIF()

add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

add_subdirectory(platform)

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
        VERSION       ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "include/Voxymore/Voxymore.hpp"
        PUBLIC cxx_std_20)


target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(${LIBRARY_TARGET_NAME} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC glm glfw  spdlog::spdlog_header_only imgui)
target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE glad)

message(STATUS "Created target ${LIBRARY_TARGET_NAME}.")
cmake_minimum_required(VERSION 3.25)

# message("Voxymore::Core is a STATIC library.")

set(LIBRARY_TARGET_NAME Core)

set(SRC_FILES src/Core/Application.cpp
        src/Core/Math.cpp
        src/Core/Logger.cpp
        include/Voxymore/Voxymore.hpp
        include/Voxymore/Core/Application.hpp
        include/Voxymore/Core/EntryPoint.hpp
        include/Voxymore/Core/Logger.hpp
        include/Voxymore/Core/Math.hpp
        vendor/stb_image.h
        include/Voxymore/Core/SystemHelper.hpp
        include/Voxymore/Events/Event.hpp
        include/Voxymore/Events/KeyEvent.hpp
        include/Voxymore/Events/MouseEvent.hpp
        include/Voxymore/Events/ApplicationEvent.hpp
        include/Voxymore/Core/Window.hpp
        include/Voxymore/Core/Macros.hpp
        src/Layers/Layer.cpp
        include/Voxymore/Layers/Layer.hpp
        src/Layers/LayerStack.cpp
        include/Voxymore/Layers/LayerStack.hpp
        src/ImGui/ImGUILayer.cpp
        include/Voxymore/ImGUI/ImGUILayer.hpp
        include/Voxymore/Core/Input.hpp
        include/Voxymore/Core/Core.hpp
        include/Voxymore/Core/KeyCodes.hpp
        include/Voxymore/Core/MouseButtonCodes.hpp
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
        src/Core/TimeStep.cpp
        include/Voxymore/Core/TimeStep.hpp
        include/Voxymore/Utils/Platform.hpp
        src/Core/Platform.cpp
        include/Voxymore/Core/SmartPointers.hpp
        src/Renderer/Texture.cpp
        include/Voxymore/Renderer/Texture.hpp
        src/Core/PerspectiveCameraController.cpp
        include/Voxymore/Core/PerspectiveCameraController.hpp
        src/Renderer/Material.cpp
        include/Voxymore/Renderer/Material.hpp
        include/Voxymore/Debug/Profiling.hpp
        src/Renderer/Framebuffer.cpp
        include/Voxymore/Renderer/Framebuffer.hpp
        src/Scene/Scene.cpp
        include/Voxymore/Scene/Scene.hpp
        include/Voxymore/Components/Components.hpp
        include/Voxymore/Scene/Entity.hpp
        src/Scene/SceneCamera.cpp
        include/Voxymore/Scene/SceneCamera.hpp
        include/Voxymore/Scene/ScriptableEntity.hpp
        src/Scene/SceneSerializer.cpp
        include/Voxymore/Scene/SceneSerializer.hpp
        src/Renderer/UniformBuffer.cpp
        include/Voxymore/Renderer/UniformBuffer.hpp
        src/Renderer/EditorCamera.cpp
        include/Voxymore/Renderer/EditorCamera.hpp
        src/Scene/SceneManager.cpp
        include/Voxymore/Scene/SceneManager.hpp
        src/Components/CustomComponent.cpp
        include/Voxymore/Components/CustomComponent.hpp
        include/Voxymore/Scene/Systems.hpp
        src/Scene/Systems.cpp
        include/Voxymore/Core/YamlHelper.hpp
        include/Voxymore/Core/FileSystem.hpp
        src/Core/FileSystem.cpp
        src/Renderer/GLTFHelper.hpp
        src/Renderer/GLTFHelper.cpp
        include/Voxymore/Renderer/Mesh.hpp
        src/Renderer/Mesh.cpp
        include/Voxymore/Renderer/Model.hpp
        src/Renderer/Model.cpp
        include/Voxymore/Components/ModelComponent.hpp
        src/Components/ModelComponent.cpp
        src/ThirdPartiesImplementor.cpp
        include/Voxymore/Core/Serializable.hpp
        src/Core/YamlHelper.cpp
        src/Project/Project.cpp
        include/Voxymore/Project/Project.hpp
        src/Project/ProjectSerializer.cpp
        include/Voxymore/Project/ProjectSerializer.hpp
        src/Core/UUID.cpp
        include/Voxymore/Core/UUID.hpp
        src/Scene/Entity.cpp
        src/ImGui/ImGuiLib.cpp
        include/Voxymore/ImGUI/ImGuiLib.hpp
        src/Assets/Assets.cpp
        include/Voxymore/Assets/Assets.hpp
        src/Physics/PhysicsLayer.cpp
        include/Voxymore/Physics/PhysicsLayer.hpp
        include/Voxymore/Physics/Particle.hpp
        src/Physics/Components/ParticleComponent.cpp
        include/Voxymore/Physics/Components/ParticleComponent.hpp
        src/Physics/Particle.cpp
        src/Components/PrimitiveComponent.cpp
        include/Voxymore/Components/PrimitiveComponent.hpp
        src/Physics/Components/AnchoredSpringComponent.cpp
        include/Voxymore/Physics/Components/AnchoredSpringComponent.hpp
        src/Physics/Systems/SpringForceSystem.cpp
        include/Voxymore/Physics/Systems/SpringForceSystem.hpp
        vendor/static_block.hpp
        src/Physics/Systems/BuoyancyForceSystem.cpp
        include/Voxymore/Physics/Systems/BuoyancyForceSystem.hpp
        src/Physics/Components/FloatingComponent.cpp
        include/Voxymore/Physics/Components/FloatingComponent.hpp
        src/Physics/Collisions/ParticleContact.cpp
        include/Voxymore/Physics/Collisions/ParticleContact.hpp
        include/Voxymore/Core/MultiThreading.hpp
        src/Physics/Collisions/ParticleContactResolver.cpp
        include/Voxymore/Physics/Collisions/ParticleContactResolver.hpp
        include/Voxymore/Scene/Entity.forward.hpp
        include/Voxymore/Scene/Entity.decl.hpp
        include/Voxymore/Scene/Entity.impl.hpp
        include/Voxymore/Scene/Scene.forward.hpp
        include/Voxymore/Scene/Scene.decl.hpp
        include/Voxymore/Scene/Scene.impl.hpp
)

add_library(${LIBRARY_TARGET_NAME} STATIC ${SRC_FILES})

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "[Rr][Ee][Ll][Ee][Aa][Ss][Ee]")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_RELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES "[Dd][Ii][Ss][Tt]")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_DIST)
endif()

target_compile_features( ${LIBRARY_TARGET_NAME} PUBLIC cxx_std_20)
if(VXM_USE_DOUBLE)
    message(STATUS "Voxymore::Core is using Double as the main floating number system.")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_DOUBLE)
else()
    message(STATUS "Voxymore::Core is using Float as the main floating number system.")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_FLOAT)
endif()

add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

add_subdirectory(platform)

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
        VERSION       ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "include/Voxymore/Voxymore.hpp"
        PUBLIC cxx_std_20)

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES ENABLE_EXPORTS True)


target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(${LIBRARY_TARGET_NAME} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC glm spdlog::spdlog_header_only imgui ImGuizmo EnTT::EnTT yaml-cpp::yaml-cpp tracy)
target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE glad glfw)

# REQUIRED to find vulkan correctly.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(Vulkan REQUIRED COMPONENTS SPIRV-Tools spirv-cross-core spirv-cross-glsl shaderc_combined)

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE Vulkan::shaderc_combined Vulkan::SPIRV-Cross-Core Vulkan::SPIRV-Cross-GLSL)

message(STATUS "Created target ${LIBRARY_TARGET_NAME}.")
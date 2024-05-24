cmake_minimum_required(VERSION 3.25)

# message("Voxymore::Core is a STATIC library.")

set(LIBRARY_TARGET_NAME Core)

set(SRC_FILES src/Core/Application.cpp
        src/Math/Math.cpp
        src/Core/Logger.cpp
        include/Voxymore/Voxymore.hpp
        include/Voxymore/Core/Application.hpp
        include/Voxymore/Core/EntryPoint.hpp
        include/Voxymore/Core/Logger.hpp
        include/Voxymore/Math/Math.hpp
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
        src/ImGui/ImGuiLayer.cpp
        include/Voxymore/ImGui/ImGuiLayer.hpp
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
        include/Voxymore/ImGui/ImGuiLib.hpp
        src/ParticlesPhysics/ParticlePhysicsLayer.cpp
        include/Voxymore/ParticlesPhysics/ParticlePhysicsLayer.hpp
        include/Voxymore/ParticlesPhysics/Particle.hpp
        src/ParticlesPhysics/Components/ParticleComponent.cpp
        include/Voxymore/ParticlesPhysics/Components/ParticleComponent.hpp
        src/ParticlesPhysics/Particle.cpp
        src/Components/PrimitiveComponent.cpp
        include/Voxymore/Components/PrimitiveComponent.hpp
        src/ParticlesPhysics/Components/AnchoredSpringComponent.cpp
        include/Voxymore/ParticlesPhysics/Components/AnchoredSpringComponent.hpp
        src/ParticlesPhysics/Systems/SpringForceSystem.cpp
        include/Voxymore/ParticlesPhysics/Systems/SpringForceSystem.hpp
        vendor/static_block.hpp
        src/ParticlesPhysics/Systems/BuoyancyForceSystem.cpp
        include/Voxymore/ParticlesPhysics/Systems/BuoyancyForceSystem.hpp
        src/ParticlesPhysics/Components/FloatingComponent.cpp
        include/Voxymore/ParticlesPhysics/Components/FloatingComponent.hpp
        src/ParticlesPhysics/Collisions/ParticleContact.cpp
        include/Voxymore/ParticlesPhysics/Collisions/ParticleContact.hpp
        include/Voxymore/Core/MultiThreading.hpp
        src/ParticlesPhysics/Collisions/ParticleContactResolver.cpp
        include/Voxymore/ParticlesPhysics/Collisions/ParticleContactResolver.hpp
        include/Voxymore/Scene/Entity.forward.hpp
        include/Voxymore/Scene/Entity.decl.hpp
        include/Voxymore/Scene/Entity.impl.hpp
        include/Voxymore/Scene/Scene.forward.hpp
        include/Voxymore/Scene/Scene.decl.hpp
        include/Voxymore/Scene/Scene.impl.hpp
        src/RigidbodiesPhysics/Rigidbody.cpp
        include/Voxymore/RigidbodiesPhysics/Rigidbody.hpp
        src/RigidbodiesPhysics/Components/RigidbodyComponent.cpp
        include/Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp
        src/RigidbodiesPhysics/RigidbodyPhysicsLayer.cpp
        include/Voxymore/RigidbodiesPhysics/RigidbodyPhysicsLayer.hpp
        src/RigidbodiesPhysics/Collisions/RigidbodyContact.cpp
        include/Voxymore/RigidbodiesPhysics/Collisions/RigidbodyContact.hpp
        src/RigidbodiesPhysics/Collisions/RBContactResolver.cpp
        include/Voxymore/RigidbodiesPhysics/Collisions/RBContactResolver.hpp
        src/Components/LightComponent.cpp
        include/Voxymore/Components/LightComponent.hpp
        include/Voxymore/Renderer/Light.hpp
        src/RigidbodiesPhysics/Systems/RigidbodySpringSystem.cpp
        include/Voxymore/RigidbodiesPhysics/Systems/RigidbodySpringSystem.hpp
        src/RigidbodiesPhysics/Components/RigidbodySpringComponent.cpp
        include/Voxymore/RigidbodiesPhysics/Components/RigidbodySpringComponent.hpp
        src/RigidbodiesPhysics/Systems/RigidbodyBuoyancySystem.cpp
        include/Voxymore/RigidbodiesPhysics/Systems/RigidbodyBuoyancySystem.hpp
        src/RigidbodiesPhysics/Components/RigidbodyFloatingComponent.cpp
        include/Voxymore/RigidbodiesPhysics/Components/RigidbodyFloatingComponent.hpp
        include/Voxymore/Math/BoundingObject.hpp
        src/Math/BoundingSphere.cpp
        include/Voxymore/Math/BoundingSphere.hpp
        include/Voxymore/RigidbodiesPhysics/Collisions/BroadCollisions.hpp
        src/Math/BoundingBox.cpp
        include/Voxymore/Math/BoundingBox.hpp
        src/RigidbodiesPhysics/Primitive.cpp
        include/Voxymore/RigidbodiesPhysics/Primitive.hpp
        src/RigidbodiesPhysics/Collisions/CollisionDetector.cpp
        include/Voxymore/RigidbodiesPhysics/Collisions/CollisionDetector.hpp
        src/RigidbodiesPhysics/Collisions/ColliderComponent.cpp
        include/Voxymore/RigidbodiesPhysics/Components/ColliderComponent.hpp
        include/Voxymore/Core/TypeHelpers.hpp
        src/Assets/Asset.cpp
        include/Voxymore/Assets/Asset.hpp
        src/Assets/AssetManager.cpp
        include/Voxymore/Assets/AssetManager.hpp
        src/Assets/AssetManagerBase.cpp
        include/Voxymore/Assets/AssetManagerBase.hpp
        src/Assets/RuntimeAssetManager.cpp
        include/Voxymore/Assets/RuntimeAssetManager.hpp
        src/Assets/EditorAssetManager.cpp
        include/Voxymore/Assets/EditorAssetManager.hpp
        include/Voxymore/Assets/AssetMetadata.hpp
        src/Assets/AssetImporter.cpp
        include/Voxymore/Assets/AssetImporter.hpp
        src/Assets/Importers/TextureImporter.cpp
        include/Voxymore/Assets/Importers/TextureImporter.hpp
        src/Core/Buffer.cpp
        include/Voxymore/Core/Buffer.hpp
        include/Voxymore/Assets/AssetField.hpp
        src/Assets/Importers/ShaderSerializer.cpp
        include/Voxymore/Assets/Importers/ShaderSerializer.hpp
        src/Assets/Importers/MeshImporter.cpp
        include/Voxymore/Assets/Importers/MeshImporter.hpp
        src/Assets/Importers/SceneImporter.cpp
        include/Voxymore/Assets/Importers/SceneImporter.hpp
        src/Assets/Importers/MaterialSerializer.cpp
        include/Voxymore/Assets/Importers/MaterialSerializer.hpp
        include/Voxymore/Assets/ImGui/MaterialImGui.hpp
        include/Voxymore/Assets/ImGui/MeshImGui.hpp
        include/Voxymore/Assets/ImGui/SceneImGui.hpp
        include/Voxymore/Assets/ImGui/ShaderImGui.hpp
        include/Voxymore/Assets/ImGui/TextureImGui.hpp
        src/Assets/ImGui/MaterialImGui.cpp
        src/Assets/ImGui/MeshImGui.cpp
        src/Assets/ImGui/SceneImGui.cpp
        src/Assets/ImGui/ShaderImGui.cpp
        src/Assets/ImGui/TextureImGui.cpp
        src/Renderer/Cubemap.cpp
        include/Voxymore/Renderer/Cubemap.hpp
        src/Math/Image.cpp
        include/Voxymore/Math/Image.hpp
)

add_library(${LIBRARY_TARGET_NAME} STATIC ${SRC_FILES})

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "[Rr][Ee][Ll][Ee][Aa][Ss][Ee]")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_RELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES "[Dd][Ii][Ss][Tt]")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_DIST)
endif()

target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_LOG)

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

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC glm spdlog::spdlog_header_only imgui ImGuizmo EnTT::EnTT yaml-cpp::yaml-cpp tracy hash-library)
target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE glad glfw)

# REQUIRED to find vulkan correctly.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(Vulkan REQUIRED COMPONENTS SPIRV-Tools spirv-cross-core spirv-cross-glsl shaderc_combined)

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE Vulkan::shaderc_combined Vulkan::SPIRV-Cross-Core Vulkan::SPIRV-Cross-GLSL)

message(STATUS "Created target ${LIBRARY_TARGET_NAME}.")
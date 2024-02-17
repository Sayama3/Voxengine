//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/Logger.hpp"

// Defining the "_USE_MATH_DEFINES" for visual studio in the main hpp
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif


#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Input.hpp"
#include "Voxymore/Core/MouseButtonCodes.hpp"
#include "Voxymore/Core/KeyCodes.hpp"
#include "Voxymore/Core/SmartPointers.hpp"

#include "Voxymore/Layers/Layer.hpp"

#include "Voxymore/ImGui/ImGuiLayer.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

#include "Voxymore/Project/Project.hpp"
#include "Voxymore/Project/ProjectSerializer.hpp"

#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Components/ModelComponent.hpp"
#include "Voxymore/Components/PrimitiveComponent.hpp"

#include "Voxymore/ParticlesPhysics/Collisions/ParticleContact.hpp"
#include "Voxymore/ParticlesPhysics/Components/AnchoredSpringComponent.hpp"
#include "Voxymore/ParticlesPhysics/Components/FloatingComponent.hpp"
#include "Voxymore/ParticlesPhysics/Components/ParticleComponent.hpp"
#include "Voxymore/ParticlesPhysics/Particle.hpp"
#include "Voxymore/ParticlesPhysics/ParticlePhysicsLayer.hpp"
#include "Voxymore/ParticlesPhysics/Systems/BuoyancyForceSystem.hpp"
#include "Voxymore/ParticlesPhysics/Systems/SpringForceSystem.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"
#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodyGravitySystem.hpp"

#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/SceneManager.hpp"
#include "Voxymore/Scene/SceneSerializer.hpp"

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Renderer/RenderCommand.hpp"
#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Renderer/Mesh.hpp"

#include "Voxymore/Renderer/Framebuffer.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include "Voxymore/Core/PerspectiveCameraController.hpp"

#include "Voxymore/Events/Event.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"

#include "Voxymore/Assets/Assets.hpp"
#include "Voxymore/Utils/Platform.hpp"

//#include "Voxymore/Core/EntryPoint.hpp"

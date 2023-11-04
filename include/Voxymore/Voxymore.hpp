//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Voxymore/Core/Macros.hpp"

// Defining the "_USE_MATH_DEFINES" for visual studio in the main hpp
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif


#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Input.hpp"
#include "Voxymore/Core/MouseButtonCodes.hpp"
#include "Voxymore/Core/KeyCodes.hpp"
#include "Voxymore/Core/SmartPointers.hpp"

#include "Voxymore/Layers/Layer.hpp"

#include "Voxymore/ImGUI/ImGUILayer.hpp"

#include "Voxymore/Project/Project.hpp"

#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Scene/Components.hpp"
#include "Voxymore/Scene/SceneSerializer.hpp"
#include "Voxymore/Scene/CustomComponent.hpp"
#include "Voxymore/Scene/GameplaySystem.hpp"

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Renderer/RenderCommand.hpp"
#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/Texture.hpp"


//#include "Voxymore/Core/EntryPoint.hpp"

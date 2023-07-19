//
// Created by ianpo on 04/07/2023.
//

#pragma once

// Defining the "_USE_MATH_DEFINES" for visual studio in the main hpp
#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES 1
#endif

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
    #define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include <Voxymore/Macros.hpp>
#include <Voxymore/Application.hpp>
#include <Voxymore/Layers/Layer.hpp>
#include <Voxymore/Logger.hpp>

#include <Voxymore/Input.hpp>
#include <Voxymore/KeyCodes.hpp>
#include <Voxymore/MouseButtonCodes.hpp>

#include <Voxymore/ImGUI/ImGUILayer.hpp>


#include <Voxymore/EntryPoint.hpp>

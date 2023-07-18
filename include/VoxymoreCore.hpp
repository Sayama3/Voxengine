//
// Created by ianpo on 04/07/2023.
//

#pragma once

// Defining the "_USE_MATH_DEFINES" for visual studio in the main hpp
#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif
#include "Macros.hpp"
#include "Application.hpp"
#include "Logger.hpp"
#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif
#include "ImGUI/ImGUILayer.hpp"


#include "EntryPoint.hpp"

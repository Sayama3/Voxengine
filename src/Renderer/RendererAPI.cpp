//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/RendererAPI.hpp"

namespace Voxymore::Core {
    //TODO: Set in an intelligent manner that change depending on the Graphics API.
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
} // Core
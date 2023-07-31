//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/RenderCommand.hpp"
#include "Voxymore/OpenGL/OpenGLRenderAPI.hpp"

namespace Voxymore::Core {
    Scope<RendererAPI> RenderCommand::s_RendererAPI = std::make_unique<OpenGLRenderAPI>();
}
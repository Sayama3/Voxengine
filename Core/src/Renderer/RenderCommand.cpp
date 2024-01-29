//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/RenderCommand.hpp"
#include "Voxymore/OpenGL/OpenGLRenderAPI.hpp"

namespace Voxymore::Core {
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRenderAPI();
}
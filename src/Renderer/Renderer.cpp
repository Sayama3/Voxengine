//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"

namespace Voxymore {
    namespace Core {
        RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;
        void Renderer::SetAPI(RendererAPI api) {
            //TODO: if the renderer was launched, change everything on the old one.
            s_RendererAPI = api;
            //TODO: Re-init everything for the new one.
        }
    } // Voxymore
} // Core
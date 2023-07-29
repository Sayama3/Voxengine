//
// Created by ianpo on 29/07/2023.
//

#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/OpenGL/OpenGLTexture2D.hpp"



namespace Voxymore {
    namespace Core {
        Ref<Texture2D> Core::Texture2D::Create(const std::string &path) {

            switch (Renderer::GetAPI()) {

                case RendererAPI::API::None:
                    VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
                    return nullptr;
                    break;
                case RendererAPI::API::OpenGL:
                    return std::make_shared<OpenGLTexture2D>(path);
                    break;
            }
            VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
            return nullptr;
        }
    } // Voxymore
} // Core
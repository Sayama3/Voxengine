//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Math.hpp"
#include "VertexArray.hpp"

namespace Voxymore {
    namespace Core {

        class RendererAPI {
        public:
            enum class API {
                None = 0,
                OpenGL = 1,
            };
        public:
            inline virtual ~RendererAPI() {}
            virtual void Init() = 0;
            virtual void SetClearColor(const glm::vec4& color) = 0;
            virtual void Clear() = 0;

            virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

            inline static API GetAPI() { return s_API; }
        private:
            static API s_API;
        };

        static inline std::string RendererAPIToString(RendererAPI::API api){
            switch (api) {

                case RendererAPI::API::None:
                    return "RendererAPI::None";
                case RendererAPI::API::OpenGL:
                    return "RendererAPI::OpenGL";
            }
            return "Unknown";
        }

        static inline std::ostream& operator<<(std::ostream& os, const RendererAPI::API& api)
        {
            return os << RendererAPIToString(api);
        }


    } // Voxymore
} // Core

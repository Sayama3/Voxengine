//
// Created by ianpo on 01/08/2023.
//

#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/OpenGL/OpenGLMaterial.hpp"


namespace Voxymore::Core {

    Ref<Material> Material::Create(Ref<Shader> &shader) {
        return CreateRef<OpenGLMaterial>(shader);
    }
} // Voxymore
// Core
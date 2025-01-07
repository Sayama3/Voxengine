//
// Created by ianpo on 07/01/2025.
//

#pragma once

#include <utility>

#include "Components.hpp"
#include "CustomComponent.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Buffer.hpp"


namespace Voxymore {
namespace Core {

    class ProcLightComponent : public Component<ProcLightComponent>
    {
    private:
        struct Light {
            glm::vec4 Color = {1,1,1,1};
            glm::vec4 Position = {0,0,0,1};
            uint32_t Seed;
        };
        struct GPUData {
            Light lightPositions[10];
            float timeStep;
            uint32_t lightCount = 10;
        };
    public:
        VXM_IMPLEMENT_COMPONENT(ProcLightComponent);
    public:
        ProcLightComponent();
        ~ProcLightComponent() = default;
        ProcLightComponent(const ProcLightComponent&);
        ProcLightComponent& operator=(const ProcLightComponent&);

    public:
        void DeserializeComponent(YAML::Node& node);
        void SerializeComponent(YAML::Emitter& out);
        bool OnImGuiRender();
    public:
        void Update(TimeStep ts);
        void Render();
    public:
        ShaderField m_RenderingShader;
        ComputeShaderField m_CalculateShader;
        Scope<GPUData> m_GPUReference;
        Ref<SSBO> m_GPUBuffer;
    };

    static_block{ProcLightComponent::RegisterComponent();}

} // Core
} // Voxymore

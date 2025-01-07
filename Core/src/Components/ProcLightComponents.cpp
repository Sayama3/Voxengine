//
// Created by ianpo on 07/01/2025.
//

#include "Voxymore/Components/ProcLightComponents.hpp"

#include "Voxymore/ImGui/ImGuiLib.hpp"
#include "Voxymore/Renderer/RenderCommand.hpp"


namespace Voxymore::Core {
    ProcLightComponent::ProcLightComponent() {
        m_GPUReference = std::make_unique<GPUData>();
        for (int i = 0; i < m_GPUReference->lightCount; ++i) {
            m_GPUReference->lightPositions[i].Position = {( rand() / float(RAND_MAX)) * 2.0f - 1.0f,(rand() / float(RAND_MAX)) * 2.0f - 1.0f,(rand() / float(RAND_MAX)) * 2.0f - 1.0f,1};
            m_GPUReference->lightPositions[i].Seed = rand();
        }
        m_GPUBuffer = SSBO::Create(sizeof(GPUData), SSBO::DynamicDraw);
        m_GPUBuffer->SetData(Buffer{m_GPUReference.get(), sizeof(GPUData)});
    }

    ProcLightComponent::ProcLightComponent(const ProcLightComponent & other)
    {
        m_RenderingShader = other.m_RenderingShader;
        m_CalculateShader = other.m_CalculateShader;
        m_GPUReference = std::make_unique<GPUData>();
        *m_GPUReference = *other.m_GPUReference;
        m_GPUBuffer = SSBO::Create(sizeof(GPUData), SSBO::DynamicDraw);
        m_GPUBuffer->SetData(Buffer{m_GPUReference.get(), sizeof(GPUData)});
    }

    ProcLightComponent & ProcLightComponent::operator=(const ProcLightComponent & other) {
        m_RenderingShader = other.m_RenderingShader;
        m_CalculateShader = other.m_CalculateShader;
        *m_GPUReference = *other.m_GPUReference;
        m_GPUBuffer->SetData(Buffer{m_GPUReference.get(), sizeof(GPUData)});
        return *this;
    }

    void ProcLightComponent::DeserializeComponent(YAML::Node& node)
    {
        m_RenderingShader = node["RenderingShader"].as<UUID>(NullAssetHandle);
        m_CalculateShader = node["CalculateShader"].as<UUID>(NullAssetHandle);
    }

    void ProcLightComponent::SerializeComponent(YAML::Emitter& out)
    {
        out << KEYVAL("RenderingShader", m_RenderingShader);
        out << KEYVAL("CalculateShader", m_CalculateShader);
    }

    bool ProcLightComponent::OnImGuiRender()
    {
        bool changed = false;
        changed |= ImGuiLib::DrawAssetField<Shader>("Rendering Shader", &m_RenderingShader);
        changed |= ImGuiLib::DrawAssetField<ComputeShader>("Calculate Shader", &m_CalculateShader);
        return changed;
    }

    void ProcLightComponent::Update(TimeStep ts) {
        if (!m_CalculateShader) return;
        m_GPUReference->timeStep = ts;
        m_GPUBuffer->SetData(Buffer{m_GPUReference.get(), sizeof(GPUData)});
        m_GPUBuffer->Bind(0);
        auto compute = m_CalculateShader.GetAsset();
        compute->Bind();
        compute->Dispatch(1,1,1);
        compute->Unbind();
        auto buffer = m_GPUBuffer->GetData(sizeof(GPUData), 0);
        *m_GPUReference = *buffer.As<GPUData>();
    }

    void ProcLightComponent::Render() {
        if (!m_RenderingShader) return;
        m_GPUBuffer->Bind(0);
        m_RenderingShader.GetAsset()->Bind();
        RenderCommand::GPUDraw(m_GPUReference->lightCount, 0, DrawMode::Points);
    }
} // Core
// Voxymore
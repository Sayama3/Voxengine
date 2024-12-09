//
// Created by Sayama on 05/12/2024.
//

#include "Voxymore/Terrain/Terrain.hpp"

#include "Voxymore/Renderer/Renderer.hpp"

namespace Voxymore::Core {
    Terrain::Terrain(uint32_t width, uint32_t depth, uint32_t height)
        : m_Width(width), m_Height(height), m_Depth(depth), m_CPUBuffer(m_Width * m_Height * m_Depth) {
        m_GraphicsInputBuffer = SSBO::Create(GetVoxelCount() * sizeof(Voxel), SSBO::DynamicRead);
        m_GraphicsOutputBuffer = SSBO::Create(sizeof(uint32_t) + GetVoxelCount() * MaxTrianglePerVoxel * SizeOfTriangle, SSBO::DynamicRead);
    }

    Terrain::~Terrain()
    {
    }

    void Terrain::Bind() {
        VXM_CORE_ASSERT(m_ComputeShaderField.IsValid());
        VXM_CORE_ASSERT(m_GraphicShaderField.IsValid());

        uint32_t triangleCount = 0u;
        m_GraphicsOutputBuffer->SetData<uint32_t>(&triangleCount, 0);
        m_GraphicsInputBuffer->SetVector(m_CPUBuffer, 0);

        m_GraphicsInputBuffer->Bind(m_SSBOInputIndex);
        m_GraphicsOutputBuffer->Bind(m_SSBOOutputIndex);
    }

    void Terrain::Unbind() {
        VXM_CORE_ASSERT(m_ComputeShaderField.IsValid());
        VXM_CORE_ASSERT(m_GraphicShaderField.IsValid());
        auto graphic = m_GraphicShaderField.GetAsset();
        graphic->Unbind();
    }

    void Terrain::Draw() {
        VXM_CORE_ASSERT(m_ComputeShaderField.IsValid());
        VXM_CORE_ASSERT(m_GraphicShaderField.IsValid());

        auto compute = m_ComputeShaderField.GetAsset();
        compute->Bind();
        compute->Dispatch(
            std::ceil(GetFWidth() / static_cast<float>(m_ThreadGroupCountX)),
            std::ceil(GetFHeight() / static_cast<float>(m_ThreadGroupCountY)),
            std::ceil(GetFDepth() / static_cast<float>(m_ThreadGroupCountZ))
        );
        auto graphic = m_GraphicShaderField.GetAsset();
        graphic->Bind();
        ScopeBuffer countBuffer = m_GraphicsOutputBuffer->GetData<uint32_t>(0u);
        uint32_t count = *(countBuffer.As<uint32_t>());
        Renderer::GPUDraw(count, 0);
    }

    void Terrain::SendDataToSSBO() {
    }

    void Terrain::FillVoxels() {
        for (int x = 1; x < m_Width -1; ++x) {
            for (int y = 1; y < m_Height / 2; ++y) {
                for (int z = 1; z < m_Depth - 1; ++z) {
                    m_CPUBuffer[GetIndex(x,y,z)] = 255u;
                }
            }
        }
    }

    uint64_t Terrain::GetIndex(uint32_t x, uint32_t y, uint32_t z) const
    {
        return x + y * m_Width + z * m_Width * m_Height;
    }

    bool Terrain::IsValid() const
    {
        return m_GraphicsInputBuffer && m_GraphicsOutputBuffer;
    }
} // Voxymore::Core

//
// Created by ianpo on 07/02/2025.
//

#include "Voxymore/Terrain/Terrain.hpp"

#include "Voxymore/Renderer/Renderer.hpp"

namespace Voxymore::Core {
    void Terrain::Initialize() {
        s_Chunks.insert_or_assign(Vec3Position{0}, Chunk{1});
        s_SSBO = SSBO::Create(VoxelCountInChunk * sizeof(Voxel), SSBO::DynamicDraw);
    }

    void Terrain::Update(const TimeStep ts) {
        if (!s_ComputeShader) return;
        auto compute = s_ComputeShader.GetAsset();
        if (!compute) return;
        compute->Bind();
        s_SSBO->Bind(0);
        compute->Dispatch(ChunkWidth.x / VoxelPerGroupThread.x, ChunkWidth.y / VoxelPerGroupThread.y, ChunkWidth.z / VoxelPerGroupThread.z);
        compute->Unbind();
    }

    void Terrain::RenderForward() {
        if (!s_RenderShader) return;
        auto render = s_RenderShader.GetAsset();
        if (!render) return;
        render->Bind();
        s_SSBO->Bind(0);
        Renderer::GPUDraw(VoxelCountInChunk,0,DrawMode::Triangles);
        render->Unbind();
    }

    void Terrain::Shutdown() {
        s_SSBO.reset();
        s_Chunks.clear();
    }

    void Terrain::SetRenderShader(ShaderField shader) {
        s_RenderShader = shader;
    }

    void Terrain::SetComputeShader(ComputeShaderField computeShader) {
        s_ComputeShader = computeShader;
    }
} // Voxymore::Core
//
// Created by ianpo on 07/02/2025.
//

#include "Voxymore/Terrain/Terrain.hpp"

namespace Voxymore::Core {
    void Terrain::Initialize() {
        s_Chunks.insert_or_assign(Vec3Position{0}, Chunk{1});
        s_SSBO = SSBO::Create(VoxelCountInChunk * sizeof(Voxel), SSBO::DynamicDraw);
    }

    void Terrain::Update() {
    }

    void Terrain::RenderForward() {
    }

    void Terrain::Shutdown() {
        s_SSBO.reset();
        s_RenderShader.reset();
        s_ComputeShader.reset();
        s_Chunks.clear();
    }
} // Voxymore::Core
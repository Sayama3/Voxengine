//
// Created by ianpo on 07/02/2025.
//

#pragma once

#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore::Core {

    class Terrain {
    public:
        using Vec3Size = glm::vec<3,uint32_t>;
        using Vec3Position = glm::vec<3,int64_t>;
        using Voxel = uint8_t;
        inline static constexpr Vec3Size ChunkWidth{32,32,32};
        inline static constexpr Vec3Size VoxelPerGroupThread{32,32,32};
        inline static constexpr uint32_t VoxelCountInChunk{ChunkWidth.x*ChunkWidth.y*ChunkWidth.z};
        using Chunk = std::array<Voxel, VoxelCountInChunk>;
    public:
        Terrain() = delete;
        ~Terrain() = delete;
        Terrain(const Terrain&) = delete;
        Terrain& operator=(const Terrain&) = delete;
    public:
        static void Initialize();
        static void Update(TimeStep ts);
        static void RenderForward();
        static void Shutdown();
    public:
        static void SetRenderShader(ShaderField shader);
        static void SetComputeShader(ComputeShaderField computeShader);
    private:
        inline static ShaderField s_RenderShader = nullptr;
        inline static ComputeShaderField s_ComputeShader = nullptr;
        inline static Ref<SSBO> s_SSBO = nullptr;
        inline static std::unordered_map<Vec3Position, Chunk> s_Chunks{{Vec3Position{0},{}}};
    };

} // Voxymore::Core

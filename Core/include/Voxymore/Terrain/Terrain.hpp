//
// Created by Sayama on 05/12/2024.
//

#pragma once

#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore {
    namespace Core {
        class Terrain {
        public:
            using Voxel = uint8_t;
            static constexpr uint32_t MaxTrianglePerVoxel = 5;
            static constexpr uint32_t SizeOfTriangle = sizeof(glm::vec3);
        public:
            Terrain(uint32_t width = 16, uint32_t depth = 16, uint32_t height = 64);
            ~Terrain();
        public:
            void Bind();
            void Unbind();
            void Draw();
            void SendDataToSSBO();
            void FillVoxels();
            bool IsValid() const;
        private:
            uint32_t GetVoxelCount() const {return m_Width*m_Height*m_Depth;}
            float GetFWidth() const {return static_cast<float>(m_Width);}
            float GetFHeight() const {return static_cast<float>(m_Height);}
            float GetFDepth() const {return static_cast<float>(m_Depth);}
		    uint64_t GetIndex(uint32_t x, uint32_t y, uint32_t z) const;
        private:
            std::vector<Voxel> m_CPUBuffer;
            Ref<SSBO> m_GraphicsInputBuffer;
            Ref<SSBO> m_GraphicsOutputBuffer;
        public:
            ComputeShaderField m_ComputeShaderField;
            ShaderField m_GraphicShaderField;
            uint32_t m_SSBOInputIndex = 0;
            uint32_t m_SSBOOutputIndex = 1;
            uint32_t m_ThreadGroupCountX = 8u, m_ThreadGroupCountY = 8u, m_ThreadGroupCountZ = 8u;
        private:
            union {
                struct {
                    uint32_t m_Width, m_Height, m_Depth;
                };
                glm::u32vec3 m_Size;
            };
        };
    } // Core
} // Voxymore

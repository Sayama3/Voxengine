//
// Created by ianpo on 02/08/2024.
//

#pragma once


#ifdef JPH_DEBUG_RENDERER

#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Mesh.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include <Jolt/Renderer/DebugRendererSimple.h>

namespace Voxymore::Core
{
	class PhysicsDebugRenderer final : public JPH::DebugRenderer
	{
	public:
		static inline std::unordered_map<uint32_t, AssetHandle> s_Materials{};
		static AssetHandle GetOrCreateMaterial(JPH::Color color);
	public:
		PhysicsDebugRenderer();

		/// Should be called every frame by the application to provide the camera position.
		/// This is used to determine the correct LOD for rendering.
		void SetCameraPos(Vec3 inCameraPos)
		{
			mCameraPos = inCameraPos;
			mCameraPosSet = true;
		}
	public:
		static inline Ref<Shader> s_Shader {nullptr};
	public:
		virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;

		virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow) override;

		virtual void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view &inString, JPH::ColorArg inColor, float inHeight) override;
	protected:
		/// Implementation of DebugRenderer interface
		virtual Batch CreateTriangleBatch(const JPH::DebugRenderer::Triangle *inTriangles, int inTriangleCount) override;
		virtual Batch CreateTriangleBatch(const JPH::DebugRenderer::Vertex *inVertices, int inVertexCount, const JPH::uint32 *inIndices, int inIndexCount) override;
		virtual void DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox &inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const JPH::DebugRenderer::GeometryRef &inGeometry, JPH::DebugRenderer::ECullMode inCullMode, JPH::DebugRenderer::ECastShadow inCastShadow, JPH::DebugRenderer::EDrawMode inDrawMode) override;

		/// Implementation specific batch object
		class BatchImpl final : public JPH::RefTargetVirtual
		{
		public:
			JPH_OVERRIDE_NEW_DELETE
			virtual void			AddRef() override			{ ++mRefCount; }
			virtual void			Release() override			{ if (--mRefCount == 0) delete this; }
			std::vector<Core::Vertex> mVertices;
			std::vector<uint32_t> mIndices;
		private:
			std::atomic<uint32_t>			mRefCount = 0;
		};

		/// Last provided camera position
		Vec3						mCameraPos;
		bool						mCameraPosSet = false;
	};

} // namespace Voxymore::Core

#endif

//
// Created by ianpo on 02/08/2024.
//

#include "Voxymore/Physics/PhysicsDebugRenderer.hpp"
#include "Voxymore/Physics/PhysicsTypeHelpers.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingBox.hpp"
#include "Voxymore/Debug/Gizmos.hpp"

#include <Jolt/Renderer/DebugRenderer.h>

namespace Voxymore::Core
{
	Ref<Mesh> GetOrCreateLine()
	{
		VXM_PROFILE_FUNCTION();
		static Ref<Mesh> mesh = nullptr;
		const uint16_t definition = 4;
		const float radius = 0.025;
		if(!mesh)
		{
			Vec3 a{0,0,-0.5},b{0,0,+0.5};
			Vec3 normal{0,1,0};
			Vec2 texCoord{0,0};
			Vec4 color{1,1,1,1};
			mesh = CreateRef<Mesh>(std::initializer_list<Core::Vertex>{{a,normal,texCoord,color},{b,normal,texCoord,color}});
			mesh->SetDrawMode(DrawMode::Lines);
		}
		return mesh;
	}
	void PhysicsDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
	{
		VXM_PROFILE_FUNCTION();
		// TODO: Do something as that's way to slow
		Vec3 a{inFrom.GetX(),	inFrom.GetY(),	inFrom.GetZ()};
		Vec3 b{inTo.GetX(), 	inTo.GetY(),	inTo.GetZ()};

		Vec3 ab = b-a;
		Real lenAB = Math::Magnitude(ab);
		ab /= lenAB;
		Vec3 up{-ab.z, ab.x, -ab.y};

		Ref<Mesh> m = GetOrCreateLine();
		m->SetMaterial(GetOrCreateMaterial(inColor));
		Gizmos::DrawMesh(m, Math::TRS((a+b) * static_cast<Real>(0.5), glm::quatLookAt(ab, up), {1,1,Math::Max(lenAB, Real(0.001))}));
	}


	void PhysicsDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 a{inV1.GetX(), inV1.GetY(), inV1.GetZ()},b{inV2.GetX(), inV2.GetY(), inV2.GetZ()},c{inV3.GetX(), inV3.GetY(), inV3.GetZ()};
		Vec4 color = Convert(inColor);
		Vec3 normal;
		{
			Vec3 u = b - a;
			Vec3 v = c - a;

			normal.x = (u.y * v.z) - (u.z * v.y);
			normal.y = (u.z * v.x) - (u.x * v.z);
			normal.z = (u.x * v.y) - (u.y * v.x);
		}
		Ref<Mesh> m = CreateRef<Mesh>(std::initializer_list<Voxymore::Core::Vertex>{{a,normal, Vec2{0,0},color},{b,normal, Vec2{0,0},color},{c,normal, Vec2{0,0},color}});
		m->SetMaterial(GetOrCreateMaterial(inColor));
		Gizmos::DrawMesh(m);
	}

	void PhysicsDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view &inString, JPH::ColorArg inColor, float inHeight)
	{
		// TODO: Implement 3D Text Rendering.
		//VXM_CORE_TRACE("[PhysicsDebugRender] (Pos: {}, {}, {}) - {}", inPosition.GetX(), inPosition.GetY(), inPosition.GetZ(), inString);
	}

	PhysicsDebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch(const PhysicsDebugRenderer::Triangle *inTriangles, int inTriangleCount)
	{
		VXM_PROFILE_FUNCTION();
		BatchImpl *batch = new BatchImpl;
		if (inTriangles == nullptr || inTriangleCount == 0)
			return batch;

		batch->mVertices.reserve(batch->mVertices.size() + (inTriangleCount * 3));
		batch->mIndices.reserve(batch->mIndices.size() + (inTriangleCount * 3));
		for (uint32_t i = 0; i < inTriangleCount; ++i) {
			const PhysicsDebugRenderer::Triangle& triangle = inTriangles[i];
			const auto newIndex = batch->mVertices.size();
			batch->mVertices.push_back(Convert(triangle.mV[0]));
			batch->mVertices.push_back(Convert(triangle.mV[1]));
			batch->mVertices.push_back(Convert(triangle.mV[2]));
			batch->mIndices.push_back(newIndex + 0);
			batch->mIndices.push_back(newIndex + 1);
			batch->mIndices.push_back(newIndex + 2);
		}
		return batch;
	}

	PhysicsDebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch(const PhysicsDebugRenderer::Vertex *inVertices, int inVertexCount, const JPH::uint32 *inIndices, int inIndexCount)
	{
		VXM_PROFILE_FUNCTION();
		BatchImpl *batch = new BatchImpl;
		if (inVertices == nullptr || inVertexCount == 0 || inIndices == nullptr || inIndexCount == 0)
			return batch;

		const uint32_t offsetIndex = batch->mVertices.size();
		batch->mVertices.reserve(batch->mVertices.size() + inVertexCount);
		for (uint32_t i = 0; i < inVertexCount; ++i) {
			batch->mVertices.push_back(Convert(inVertices[i]));
		}

		batch->mIndices.reserve(batch->mIndices.size() + inIndexCount);
		for (uint32_t i = 0; i < inIndexCount; ++i) {
			batch->mIndices.push_back(offsetIndex + inIndices[i]);
		}

		return batch;
	}

	void PhysicsDebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox &inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const PhysicsDebugRenderer::GeometryRef &inGeometry, PhysicsDebugRenderer::ECullMode inCullMode, PhysicsDebugRenderer::ECastShadow inCastShadow, PhysicsDebugRenderer::EDrawMode inDrawMode)
	{
		VXM_PROFILE_FUNCTION();
		const LOD *lod = inGeometry->mLODs.data();
		if (mCameraPosSet)
			lod = &inGeometry->GetLOD(Convert(mCameraPos), inWorldSpaceBounds, inLODScaleSq);

		// Draw the batch
		const BatchImpl* batch = static_cast<const BatchImpl *>(lod->mTriangleBatch.GetPtr());

		const Ref<Mesh> m = CreateRef<Mesh>(batch->mVertices, batch->mIndices, Convert(inWorldSpaceBounds));
		m->SetMaterial(GetOrCreateMaterial(inModelColor));
		Gizmos::DrawMesh(m, Convert(inModelMatrix), Convert(inModelColor), 0, false, inDrawMode == EDrawMode::Wireframe);
	}

	PhysicsDebugRenderer::PhysicsDebugRenderer()
	{
		Initialize();
	}

	AssetHandle PhysicsDebugRenderer::GetOrCreateMaterial(JPH::Color color)
	{
		static_assert(sizeof(JPH::Color) == sizeof(uint32_t));
		union {
			JPH::Color col;
			uint32_t id;
		};
		col = color;

		if(s_Materials.contains(id)) {
			if (s_Shader) {
				MaterialField mat = s_Materials[id];
				if(mat) mat.GetAsset()->ChangeShader(s_Shader);
			}
		} else {
			VXM_CORE_ASSERT(s_Shader, "No shader for the Physics Debug Renderer.");
			Ref<Material> mat;
			if(s_Shader) mat = Project::GetActive()->GetEditorAssetManager()->CreateMemoryAsset<Material>(s_Shader);
			else mat = Project::GetActive()->GetEditorAssetManager()->CreateMemoryAsset<Material>();
			mat->GetMaterialsParameters().PbrMetallicRoughness.BaseColorFactor = Convert(color);
			s_Materials[id] = mat->Handle;
		}
		return s_Materials[id];
	}
}// namespace Voxymore::Core
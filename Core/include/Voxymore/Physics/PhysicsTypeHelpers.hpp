//
// Created by ianpo on 02/08/2024.
//

#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Mesh.hpp"

namespace Voxymore::Core
{
	inline Vec3 Convert(JPH::Vec3 val) {return {val.GetX(),val.GetY(),val.GetZ()};}
	inline Vec4 Convert(JPH::Vec4 val) {return {val.GetX(),val.GetY(),val.GetZ(),val.GetW()};}
	inline Quat Convert(JPH::Quat val) {return Quat::wxyz(val.GetW(), val.GetX(), val.GetY(), val.GetZ());}
	inline Mat4 Convert(JPH::Mat44 inModelMatrix) {return {
				inModelMatrix(0,0),inModelMatrix(1,0),inModelMatrix(2,0),inModelMatrix(3,0),
				inModelMatrix(0,1),inModelMatrix(1,1),inModelMatrix(2,1),inModelMatrix(3,1),
				inModelMatrix(0,2),inModelMatrix(1,2),inModelMatrix(2,2),inModelMatrix(3,2),
				inModelMatrix(0,3),inModelMatrix(1,3),inModelMatrix(2,3),inModelMatrix(3,3)
		};}
	inline Vertex Convert(JPH::DebugRenderer::Vertex vert) {
		return {
			{vert.mPosition.x,vert.mPosition.y,vert.mPosition.z},
			{vert.mNormal.x,vert.mNormal.y,vert.mNormal.z},
			{vert.mUV.x,vert.mUV.y},
			{Real(vert.mColor.r) / Real(255),Real(vert.mColor.g) / Real(255),Real(vert.mColor.b) / Real(255),Real(vert.mColor.a) / Real(255)}
		};
	}

	inline BoundingBox Convert(JPH::AABox aabb) {
		return {
			Convert(aabb.mMin),
			Convert(aabb.mMax)
		};
	}

	inline JPH::Vec3 Convert(Vec3 val) {return {val.x,val.y,val.z};}
	inline JPH::Vec4 Convert(Vec4 val) {return {val.x,val.y,val.z,val.w};}
	inline JPH::Quat Convert(Quat val) {return {val.x, val.y, val.z, val.w};}

}

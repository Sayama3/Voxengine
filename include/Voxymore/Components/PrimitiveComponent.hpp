//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Renderer/Mesh.hpp"


namespace Voxymore::Core
{
	class PrimitiveComponent
	{
		VXM_IMPLEMENT_COMPONENT(PrimitiveComponent);
	public:
		inline PrimitiveComponent() = default;
		inline ~PrimitiveComponent() = default;
		PrimitiveComponent(Primitive::Type primitiveType);

		void Load();
		bool IsLoaded() const;
		Ref<Mesh> GetMesh();
		bool IsDirty() const;
		Primitive::Type GetType() const;
		void SetType(Primitive::Type type);

	private:
		Primitive::Type PrimitiveType;
		Ref<Mesh> m_Mesh = nullptr;
		bool m_IsDirty = true;
	};
}
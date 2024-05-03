//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Renderer/Mesh.hpp"


namespace Voxymore::Core
{
	class PrimitiveComponent : public Component<PrimitiveComponent>
	{
		VXM_IMPLEMENT_COMPONENT(PrimitiveComponent);
	public:
		void DeserializeComponent(YAML::Node& node);
		void SerializeComponent(YAML::Emitter& out);
		bool OnImGuiRender();

		inline PrimitiveComponent() = default;
		inline ~PrimitiveComponent() = default;
		PrimitiveComponent(PrimitiveMesh::Type primitiveType);

		void Load();
		bool IsLoaded() const;
		bool IsValid() const;
		Ref<Mesh> GetMesh();
		bool IsDirty() const;
		PrimitiveMesh::Type GetType() const;
		void SetType(PrimitiveMesh::Type type);

	private:
		PrimitiveMesh::Type PrimitiveType;
		MaterialField m_Material;
		Ref<Mesh> m_Mesh = nullptr;
		bool m_IsDirty = true;
	};

	VXM_CREATE_COMPONENT(PrimitiveComponent);
}
//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Components/PrimitiveComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"


namespace Voxymore::Core
{
	void PrimitiveComponent::DeserializeComponent(YAML::Node &node)
	{
		VXM_PROFILE_FUNCTION();
		auto &pc = *this;

		auto PrimitiveTypeNode = node["PrimitiveType"];
		if (PrimitiveTypeNode.IsDefined())
		{
			pc.SetType(static_cast<PrimitiveMesh::Type>(PrimitiveTypeNode.as<int>()));
		}
		else
		{
			pc.SetType(PrimitiveMesh::Type::Square);
			VXM_CORE_WARNING("We didn't found the field '{0}'. Initializing it at {1}", "PrimitiveType", PrimitiveMesh::GetTypeString(pc.PrimitiveType));
		}

		m_Material = node["Material"].as<MaterialField>();
	}

	void PrimitiveComponent::SerializeComponent(YAML::Emitter &out)
	{
		VXM_PROFILE_FUNCTION();
		auto& pc = *this;

		out << KEYVAL("PrimitiveType", pc.PrimitiveType);
		out << KEYVAL("Material", m_Material);
	}

	bool PrimitiveComponent::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;

		auto& pc = *this;

		std::unordered_map<PrimitiveMesh::Type, std::string> strings = PrimitiveMesh::GetTypesString();
		std::vector<PrimitiveMesh::Type> types = PrimitiveMesh::GetAllTypes();

		PrimitiveMesh::Type current = pc.PrimitiveType;

		if(ImGui::BeginCombo("Primitive", strings[current].c_str()))
		{
			for(auto& t: types)
			{
				const bool is_selected = (t == current);

				if(ImGui::Selectable(strings[t].c_str(), is_selected))
				{
					pc.SetType(t);
					changed |= pc.IsDirty();
				}

				if(is_selected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		changed |= ImGuiLib::DrawAssetField("Material", &m_Material);

		return changed;
	}

	void PrimitiveComponent::Load()
	{
		VXM_PROFILE_FUNCTION();
		m_Mesh = PrimitiveMesh::CreateOrphan(PrimitiveType, m_Material);

		m_IsDirty = false;
	}

	bool PrimitiveComponent::IsLoaded() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Mesh != nullptr;
	}

	bool PrimitiveComponent::IsValid() const
	{
		VXM_PROFILE_FUNCTION();
		return IsLoaded() && m_Material;
	}

	Ref<Mesh> PrimitiveComponent::GetMesh()
	{
		VXM_PROFILE_FUNCTION();
		return m_Mesh;
	}

	bool PrimitiveComponent::IsDirty() const
	{
		VXM_PROFILE_FUNCTION();
		return m_IsDirty;
	}

	PrimitiveMesh::Type PrimitiveComponent::GetType() const
	{
		VXM_PROFILE_FUNCTION();
		return PrimitiveType;
	}

	void PrimitiveComponent::SetType(PrimitiveMesh::Type type)
	{
		VXM_PROFILE_FUNCTION();
		m_IsDirty |= PrimitiveType != type;
		PrimitiveType = type;
	}

	PrimitiveComponent::PrimitiveComponent(PrimitiveMesh::Type primitiveType) : PrimitiveType(PrimitiveMesh::Type::Cube), m_IsDirty(true)
	{
	}
}
//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Components/PrimitiveComponent.hpp"


namespace Voxymore::Core
{
	VXM_CREATE_LIGHT_COMPONENT(PrimitiveComponent);

	void PrimitiveComponent::DeserializeComponent(YAML::Node &node)
	{
		auto &pc = *this;

		auto PrimitiveTypeNode = node["PrimitiveType"];
		if (PrimitiveTypeNode.IsDefined())
		{
			pc.SetType(static_cast<Primitive::Type>(PrimitiveTypeNode.as<int>()));
		}
		else
		{
			pc.SetType(Primitive::Type::Square);
			VXM_CORE_WARNING("We didn't found the field '{0}'. Initializing it at {1}", "PrimitiveType", Primitive::GetTypeString(pc.PrimitiveType));
		}
	}

	void PrimitiveComponent::SerializeComponent(YAML::Emitter &out)
	{
		auto& pc = *this;

		out << KEYVAL("PrimitiveType", pc.PrimitiveType);
	}

	bool PrimitiveComponent::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;

		auto& pc = *this;

		std::unordered_map<Primitive::Type, std::string> strings = Primitive::GetTypesString();
		std::vector<Primitive::Type> types = Primitive::GetAllTypes();

		Primitive::Type current = pc.PrimitiveType;

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

		return changed;
	}

	void PrimitiveComponent::Load()
	{
		VXM_PROFILE_FUNCTION();
		m_Mesh = Primitive::CreateOrphan(PrimitiveType);
		m_IsDirty = false;
	}

	bool PrimitiveComponent::IsLoaded() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Mesh != nullptr;
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

	Primitive::Type PrimitiveComponent::GetType() const
	{
		VXM_PROFILE_FUNCTION();
		return PrimitiveType;
	}

	void PrimitiveComponent::SetType(Primitive::Type type)
	{
		VXM_PROFILE_FUNCTION();
		m_IsDirty |= PrimitiveType != type;
		PrimitiveType = type;
	}

	PrimitiveComponent::PrimitiveComponent(Primitive::Type primitiveType) : PrimitiveType(Primitive::Type::Cube), m_IsDirty(true)
	{
	}
}
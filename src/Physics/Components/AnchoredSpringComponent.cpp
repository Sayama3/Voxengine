//
// Created by ianpo on 06/01/2024.
//

#include "Voxymore/Physics/Components/AnchoredSpringComponent.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"

#define DeserializeField(node, fieldVariable, fieldName, type, defaultValue)														\
auto VXM_COMBINE(fieldVariable, Node) = node[fieldName]; 																			\
if(VXM_COMBINE(fieldVariable, Node).IsDefined())																						\
{																																		\
fieldVariable = VXM_COMBINE(fieldVariable, Node).as<type>();																		\
}																																		\
else {																																	\
	fieldVariable = defaultValue;																									\
		VXM_CORE_WARNING("We didn't found the field '{0}'. Initializing it at {1}", #fieldName, Math::to_string(fieldVariable));	\
}

namespace Voxymore::Core
{
	VXM_CREATE_LIGHT_COMPONENT(AnchoredSpringComponent);
	void AnchoredSpringComponent::DeserializeComponent(YAML::Node& node)
	{
		DeserializeField(node, SpringConstant, "SpringConstant", Real, 1.0);
		DeserializeField(node, RestLength, "RestLength", Real, 2.0);

		EntitiesConnected.clear();
		auto EntitiesConnectedNode = node["EntitiesConnected"];
		if(EntitiesConnectedNode.IsSequence())
		{
			for(auto child : EntitiesConnectedNode)
			{
				UUID scene = child["SceneId"].as<uint64_t>();
				UUID entity = child["EntityId"].as<uint64_t>();
				EntitiesConnected.emplace_back(entity, scene);
			}
		}

		EntitiesNameHelper.clear();
		auto EntitiesNameHelperNode = node["EntitiesNameHelper"];
		if(EntitiesConnectedNode.IsSequence())
		{
			for(auto child : EntitiesConnectedNode)
			{
				EntitiesNameHelper.emplace_back(child.as<std::string>());
			}
		}
	}

	void AnchoredSpringComponent::SerializeComponent(YAML::Emitter& out)
	{
		out << KEYVAL("SpringConstant", SpringConstant);
		out << KEYVAL("RestLength", RestLength);
		out << YAML::BeginSeq;
		{
			for (auto& field: EntitiesConnected)
			{
				out << YAML::BeginMap;
				{
					out << KEYVAL("SceneId", field.SceneId);
					out << KEYVAL("EntityId", field.EntityId);
					out << YAML::EndMap;
				}
			}
			out << YAML::EndSeq;
		}
		out << YAML::BeginSeq;
		{
			for (auto& name: EntitiesNameHelper)
			{
				out << name;
			}
			out << YAML::EndSeq;
		}
	}

	bool AnchoredSpringComponent::OnImGuiRender()
	{
		bool changed = false;
		changed |= ImGuiLib::DragReal("Spring Constant", &SpringConstant, 0.01, 0.0, 0.0, "%.2f");
		changed |= ImGuiLib::DragReal("Rest Length", &RestLength, 0.01, 0.0, 0.0, "%.2f");

		if(ImGui::BeginListBox("Entities Connected"))
		{
			for (int i = 0; i < EntitiesConnected.size(); ++i)
			{
				const bool item_selected = false;
				if (ImGui::Selectable(EntitiesNameHelper[i].c_str(), item_selected))
				{
				}

				if (item_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}

		return changed;
	}

}

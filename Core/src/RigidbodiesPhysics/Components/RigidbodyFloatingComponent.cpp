//
// Created by ianpo on 19/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyFloatingComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

#define DeserializeField(node, fieldVariable, fieldName, type, defaultValue) fieldVariable = node[fieldName].as<type>(defaultValue);

namespace Voxymore
{
	namespace Core
	{
		void RigidbodyFloatingComponent::DeserializeComponent(YAML::Node& node) {
			DeserializeField(node, LocalPosition, "LocalPosition", Vec3, VXM_DEFAULT_LOCAL_POSITION);
			DeserializeField(node, MaxDepth, "MaxDepth", Real, VXM_DEFAULT_MAX_DEPTH);
			DeserializeField(node, Volume, "Volume", Real, VXM_DEFAULT_VOLUME);
			if(node["WaterHeight"].IsDefined()) {
				WaterHeight = node["WaterHeight"].as<Real>();
			} else{
				WaterHeight = VXM_DEFAULT_WATER_HEIGHT;
			}
			if(node["LiquidDensity"].IsDefined()) {
				LiquidDensity = node["LiquidDensity"].as<Real>();
			} else{
				LiquidDensity = VXM_DEFAULT_LIQUID_DENSITY;
			}
		}
		void RigidbodyFloatingComponent::SerializeComponent(YAML::Emitter& out) {
			out << KEYVAL("LocalPosition", LocalPosition);
			out << KEYVAL("MaxDepth", MaxDepth);
			out << KEYVAL("Volume", Volume);
			if(WaterHeight.has_value()) {
				out << KEYVAL("WaterHeight", WaterHeight.value());
			}
			if(LiquidDensity.has_value()) {
				out << KEYVAL("LiquidDensity", LiquidDensity.value());
			}
		}
		bool RigidbodyFloatingComponent::OnImGuiRender() {
			bool changed = false;
			changed |= ImGuiLib::DrawVec3Control("Local Position", LocalPosition);
			changed |= ImGuiLib::DragReal("Max Depth", &MaxDepth, 0.001, 0, REAL_MAX);
			if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("The maximum submersion depth of an object before it generates its maximum buoyancy force.");
			}
			changed |= ImGuiLib::DragReal("Volume", &Volume, 0.001, 0, REAL_MAX);
			if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("The volume of the object.");
			}

			bool hasWaterHeight = WaterHeight.has_value();
			if(ImGui::Checkbox("Water Height", &hasWaterHeight))
			{
				changed = true;
				if(!hasWaterHeight) {
					WaterHeight = {};
				} else {
					WaterHeight = VXM_DEFAULT_WATER_HEIGHT;
				}
			}
			if(hasWaterHeight) {
				ImGui::SameLine();
				//			ImGui::BeginDisabled(!hasWaterHeight);
				changed |= ImGuiLib::DragReal("##Water_Height_Value", &WaterHeight.value(), 0.01);
				if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
					ImGui::SetTooltip("The height (on the global Y axis) of the water of this object.\n"
									  "Can be override to use this one instead of the global one..");
				}
				//			ImGui::EndDisabled();
			}
			bool hasLiquidDensity = LiquidDensity.has_value();
			if(ImGui::Checkbox("Liquid Density", &hasLiquidDensity))
			{
				changed = true;
				if(!hasLiquidDensity) {
					LiquidDensity = {};
				} else {
					LiquidDensity = VXM_DEFAULT_LIQUID_DENSITY;
				}
			}
			if(hasLiquidDensity) {
				ImGui::SameLine();
				//			ImGui::BeginDisabled(!hasLiquidDensity);
				changed |= ImGuiLib::DragReal("##Liquid_Density_Value", &LiquidDensity.value());
				if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
					ImGui::SetTooltip("The density of the liquid. Pure water has a density of 1000 (kg/m3).\n"
									  "Can be override to use this one instead of the global one.");
				}
				//			ImGui::EndDisabled();
			}
			return changed;
		}
	}// namespace Core
}// namespace Voxymore
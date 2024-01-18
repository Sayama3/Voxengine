//
// Created by ianpo on 18/01/2024.
//

#include "Voxymore/Physics/Components/FloatingComponent.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"

namespace Voxymore
{
	namespace Core
	{
		void FloatingComponent::DeserializeComponent(YAML::Node& node) {
			if(node["MaxDepth"].IsDefined()) {
				MaxDepth = node["MaxDepth"].as<Real>();
			} else{
				MaxDepth = 1;
			}
			if(node["Volume"].IsDefined()) {
				Volume = node["Volume"].as<Real>();
			} else{
				Volume = 1;
			}
			if(node["WaterHeight"].IsDefined()) {
				WaterHeight = node["WaterHeight"].as<Real>();
			} else{
				WaterHeight = {};
			}
			if(node["LiquidDensity"].IsDefined()) {
				LiquidDensity = node["LiquidDensity"].as<Real>();
			} else{
				LiquidDensity = {};
			}
		}
		void FloatingComponent::SerializeComponent(YAML::Emitter& out) {
			out << KEYVAL("MaxDepth", MaxDepth);
			out << KEYVAL("Volume", Volume);
			if(WaterHeight.has_value()) {
				out << KEYVAL("WaterHeight", WaterHeight.value());
			}
			if(LiquidDensity.has_value()) {
				out << KEYVAL("LiquidDensity", LiquidDensity.value());
			}
		}
		bool FloatingComponent::OnImGuiRender() {
			bool changed = false;
			changed |= ImGuiLib::DragReal("Max Depth", &MaxDepth);
			if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("The maximum submersion depth of an object before it generates its maximum buoyancy force.");
			}
			changed |= ImGuiLib::DragReal("Volume", &Volume);
			if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("The volume of the object.");
			}

			bool hasWaterHeight = WaterHeight.has_value();
			if(ImGui::Checkbox("##HasWaterHeight", &hasWaterHeight))
			{
				changed = true;
				if(!hasWaterHeight) {
					WaterHeight = {};
				} else {
					WaterHeight = 0.0;
				}
			}
			ImGui::SameLine();
			ImGui::BeginDisabled(!hasWaterHeight);
			changed |= ImGuiLib::DragReal("Water Height", &WaterHeight.value());
			if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("The height (on the global Y axis) of the water of this object.\n"
								  "Can be override to use this one instead of the global one..");
			}
			ImGui::EndDisabled();

			bool hasLiquidDensity = LiquidDensity.has_value();
			if(ImGui::Checkbox("##HasLiquidDensity", &hasLiquidDensity))
			{
				changed = true;
				if(!hasLiquidDensity) {
					LiquidDensity = {};
				} else {
					LiquidDensity = 1000.0;
				}
			}
			ImGui::SameLine();
			ImGui::BeginDisabled(!hasLiquidDensity);
			changed |= ImGuiLib::DragReal("Liquid Density", &LiquidDensity.value());
			if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("The density of the liquid. Pure water has a density of 1000 (kg/m3).\n"
								  "Can be override to use this one instead of the global one.");
			}
			ImGui::EndDisabled();

			return changed;
		}
	}// namespace Core
}// namespace Voxymore
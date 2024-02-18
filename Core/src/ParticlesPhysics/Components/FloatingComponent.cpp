//
// Created by ianpo on 18/01/2024.
//

#include "Voxymore/ParticlesPhysics/Components/FloatingComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include "Voxymore/ParticlesPhysics/Systems/BuoyancyForceSystem.hpp"


namespace Voxymore::Core
{
	void FloatingComponent::DeserializeComponent(YAML::Node& node) {
		if(node["MaxDepth"].IsDefined()) {
			MaxDepth = node["MaxDepth"].as<Real>();
		} else{
			MaxDepth = VXM_DEFAULT_MAX_DEPTH;
		}
		if(node["Volume"].IsDefined()) {
			Volume = node["Volume"].as<Real>();
		} else{
			Volume = VXM_DEFAULT_VOLUME;
		}
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
} // namespace Voxymore::Core

#include "Voxymore/Editor/Panels/SystemPanel.hpp"
#include "Voxymore/Scene/SceneManager.hpp"
#include <imgui.h>
#include <imgui_internal.h>

namespace Voxymore::Editor
{
    void SystemPanel::OnImGuiRender()
    {
		VXM_PROFILE_FUNCTION();
		if(m_Open) {
			if (ImGui::Begin("Systems", &m_Open)) {
				m_SceneIds = SceneManager::GetSceneIds();
				m_SceneNames.clear();
				m_SceneNames.reserve(m_SceneIds.size());
				for (auto id: m_SceneIds) {
					m_SceneNames.push_back(SceneManager::GetScene(id)->GetName());
				}
				auto systemNames = SystemManager::GetSystemsName();

				for (const auto &name: systemNames) {
					DrawSystem(name);
				}
			}
			ImGui::End();
		}
    }

    void SystemPanel::DrawSystem(const std::string &name)
    {
		VXM_PROFILE_FUNCTION();
        std::hash<std::string> strHash;
        uint64_t hash = strHash(name);
        const ImGuiTreeNodeFlags treeNodeFlags =
                ImGuiTreeNodeFlags_DefaultOpen |
                ImGuiTreeNodeFlags_AllowItemOverlap |
                ImGuiTreeNodeFlags_SpanAvailWidth |
                ImGuiTreeNodeFlags_FramePadding |
                ImGuiTreeNodeFlags_Framed;

        auto& sysScenes = SystemManager::GetSystemScenes(name);

        ImVec2 contentAvailable = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)hash, treeNodeFlags, "%s", name.c_str());
        ImGui::PopStyleVar();

        ImGui::SameLine(contentAvailable.x - lineHeight * 0.5f);
        bool enable = SystemManager::IsActive(name);
        bool shouldSave = false;
        if(ImGui::Checkbox("Enable", &enable))
        {
            shouldSave = true;
            SystemManager::SetActive(name, enable);
        }

        if(open)
        {
			auto sys = SystemManager::GetSystem(name);
			if(!sys->RunOnAllScenes()) {
				ImGui::Text("Scenes");
				ImVec2 tableSize = ImGui::GetContentRegionAvail();

				VXM_CHECK(!m_SceneIds.empty(), "The ::Voxymore::Core::SceneManager is empty...");

				if (!m_SceneIds.empty() && ImGui::BeginTable("##ScenesTable", tableSize.x > 0 ? glm::min((int) tableSize.x % 250, (int) m_SceneNames.size()) : 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
				{
					for (int i = 0; i < m_SceneIds.size(); ++i)
					{
						std::string& sceneName = m_SceneNames[i];
						UUID sceneId = m_SceneIds[i];

						bool sceneIsActive = std::find(sysScenes.begin(), sysScenes.end(), sceneId) != sysScenes.end();
						ImGui::TableNextColumn();
						bool changed = ImGui::Selectable(sceneName.c_str(), &sceneIsActive); // FIXME-TABLE: Selection overlap
						shouldSave |= changed;
						if (changed) {
							if (sceneIsActive) SystemManager::AddSceneToSystemIfNotExist(name, sceneId);
							else SystemManager::RemoveSceneFromSystem(name, sceneId);
						}
					}
					ImGui::EndTable();
				}
			}

            shouldSave |= sys->OnImGuiRender();

            ImGui::TreePop();
        }

        if(shouldSave)
        {
            SystemManager::SaveSystem(name);
        }
    }
}


//
// Created by ianpo on 06/02/2025.
//

#include "Voxymore/Editor/Panels/LogsPanel.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor {

    static ImVec4 GetColor(Log::Level level) {
        ImVec4 color{1,1,1,1};
        switch (level) {
            case Core::Log::Debug:
                color = ImVec4(0.8f, 0.88f, 0.95f, 1.0f);
                break;
            case Core::Log::Trace:
                color = ImVec4(0.41f, 0.63f, 0.77f, 1.0f);
                break;
            case Core::Log::Info:
                color = ImVec4(0.03f, 0.37f, 0.58f, 1.0f);
                break;
            case Core::Log::Warning:
                color = ImVec4(0.9f, 0.61f, 0.18f, 1.0f);
                break;
            case Core::Log::Error:
                color = ImVec4(0.77f, 0.3f, 0.15f, 1.0f);
                break;
            case Core::Log::Critical:
                color = ImVec4(0.65f, 0.16f, 0.13f, 1.0f);
                break;
        }
        return color;
    }

    void LogsPanel::Clear()
    {
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void LogsPanel::OnImGuiRender() {
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();

        for (auto it = Core::Log::end() - 1 ; it != Core::Log::end(); --it) {
            if (Filter.IsActive() && !Filter.PassFilter(it->message.c_str())) continue;
            ImGui::PushStyleColor(ImGuiCol_Text, GetColor(it->level));
            ImGui::TextWrapped(it->message.c_str());
            ImGui::PopStyleColor();
        }
    }
}

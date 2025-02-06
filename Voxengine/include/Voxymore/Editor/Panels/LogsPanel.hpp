//
// Created by ianpo on 06/02/2025.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include "Panel.hpp"

namespace Voxymore::Editor {

    class LogsPanel : public Panel<LogsPanel> {
    public:
        VXM_IMPLEMENT_PANEL("Logs Panel");
    public:
        virtual void OnImGuiRender() override;
    private:
        void Clear();
    private:
        ImGuiTextFilter     Filter;
        ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        bool                AutoScroll;  // Keep scrolling if already at the bottom.
    };

}

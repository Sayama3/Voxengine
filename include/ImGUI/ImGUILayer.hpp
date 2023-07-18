//
// Created by ianpo on 18/07/2023.
//

#pragma once

#include "Layers/Layer.hpp"

namespace Voxymore::Core {

    class VXM_API ImGUILayer : public Layer {
    public:
        ImGUILayer();
        ~ImGUILayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& event) override;
    private:
        float m_Time = 0.0f;
    };

} // Core

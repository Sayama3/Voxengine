//
// Created by ianpo on 12/02/2025.
//

#pragma once

#include "Voxymore/Layers/Layer.hpp"

namespace Voxymore::Core {

    class TerrainLayer final : public Layer {
    public:
        TerrainLayer();
        ~TerrainLayer() override;
    public:
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(TimeStep ts) override;
        void OnEvent(Event &event) override;
        void OnImGuiRender() override;
    };

}

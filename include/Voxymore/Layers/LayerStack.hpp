//
// Created by ianpo on 18/07/2023.
//

#pragma once

#include "Layer.hpp"
#include "Voxymore/Core.hpp"

namespace Voxymore::Core {

    class VXM_API LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        inline std::vector<Layer*>::iterator begin() {return m_Layers.begin();}
        inline std::vector<Layer*>::iterator end() {return m_Layers.end();}

    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };

} // Layers
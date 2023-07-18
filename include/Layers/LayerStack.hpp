//
// Created by ianpo on 18/07/2023.
//

#pragma once

#include "Layer.hpp"

namespace Voxymore::Core::Layers {

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

        inline bool InsertEqualEnd(){
            return m_LayerInsert == end();
        }
    private:
        std::vector<Layer*> m_Layers;
        std::vector<Layer*>::iterator m_LayerInsert;
    };

} // Layers

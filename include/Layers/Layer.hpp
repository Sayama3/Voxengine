//
// Created by ianpo on 18/07/2023.
//

#pragma once

#include "Events/Event.hpp"

// We shouldn't search layer by name!!

namespace Voxymore::Core {
    class VXM_API Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };

} // Layers

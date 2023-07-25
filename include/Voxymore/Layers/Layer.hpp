//
// Created by ianpo on 18/07/2023.
//

#pragma once

#include "Voxymore/Events/Event.hpp"
#include "Voxymore/Core.hpp"
#include "Voxymore/Core/TimeStep.hpp"

// We shouldn't search layer by name!!

namespace Voxymore::Core {
    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(TimeStep timeStep) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };

} // Layers

//
// Created by ianpo on 08/10/2023.
//

#pragma once

#include <utility>

#include "CustomComponent.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Renderer/Model.hpp"

namespace Voxymore::Core
{
    
    // ======== ModelComponent ========
    class ModelComponent : public Component<ModelComponent>
	{
    	VXM_IMPLEMENT_COMPONENT(ModelComponent);
    public:
		void DeserializeComponent(YAML::Node& node);
		void SerializeComponent(YAML::Emitter& out);
		bool OnImGuiRender();

		[[nodiscard]] inline Ref<Model> GetModel() const {VXM_CORE_ASSERT(m_Model, "Model is not loaded"); return m_Model.GetAsset();}
		[[nodiscard]] bool IsValid() const;
    private:
        ModelField m_Model;
    };

	VXM_CREATE_COMPONENT(ModelComponent)
}
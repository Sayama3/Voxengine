//
// Created by ianpo on 17/02/2024.
//

#pragma once

#include <Voxymore/Voxymore.hpp>

#define VXM_IMPLEMENT_PANEL(name) public:\
inline static std::string StaticGetName() { return name; } \
inline virtual std::string GetName() override { return StaticGetName(); }

namespace Voxymore::Editor
{
	class BasePanel
	{
	public:
		typedef Core::UUID PanelHandle;
	public:

		inline virtual ~BasePanel() {}

		virtual uint64_t GetTypeID() = 0;
		virtual std::string GetName() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnImGuizmo(const float* viewMatrix, const float* projectionMatrix);

		PanelHandle GetHandle() const { return m_ID;}
	private:
		PanelHandle m_ID;
	};

	template<class SubPanel>
	class Panel : public BasePanel
	{
	public:
		inline virtual uint64_t GetTypeID() override { return typeid(SubPanel).hash_code(); };
		inline static Core::Ref<BasePanel> CreatePanel() { return Core::CreateRef<SubPanel>(); }
	};

} // namespace Voxymore::Editor


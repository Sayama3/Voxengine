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
		typedef uint64_t PanelHandle;
	private:
		static PanelHandle s_BaseId;
	public:

		BasePanel();
		BasePanel(PanelHandle id);
		inline virtual ~BasePanel() {}

		virtual uint64_t GetTypeID() = 0;
		virtual std::string GetName() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void BeginPanel();
		virtual void EndPanel();
		virtual void OnImGuizmo(const float* viewMatrix, const float* projectionMatrix);

		inline bool IsOpen() const {return m_Open;}
		inline void Open() {m_Open = true;}
		inline void Close() {m_Open = false;}
		inline void SetOpen(bool isOpen) {m_Open = isOpen;}
		inline PanelHandle GetHandle() const { return m_ID;}
		inline void SetHandle(PanelHandle handle) {m_ID = handle;}
	private:
		PanelHandle m_ID;
		bool m_Open = true;
	};

	template<class SubPanel>
	class Panel : public BasePanel
	{
	private:
		static PanelHandle s_Id;
	public:
		inline Panel<SubPanel>() : BasePanel(s_Id++) {}

		inline static uint64_t StaticGetTypeID() { return typeid(SubPanel).hash_code(); };
		inline virtual uint64_t GetTypeID() override { return StaticGetTypeID(); };
		inline static Core::Ref<BasePanel> CreatePanel() { return Core::CreateRef<SubPanel>(); }
	};

	template<class SubPanel>
	BasePanel::PanelHandle Panel<SubPanel>::s_Id = 0u;

} // namespace Voxymore::Editor


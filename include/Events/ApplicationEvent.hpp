//
// Created by ianpo on 14/07/2023.
//

#ifndef VOXYMORE_APPLICATIONEVENT_HPP
#define VOXYMORE_APPLICATIONEVENT_HPP

#include "Events/Event.hpp"

namespace Voxymore::Core {

    class VXM_API WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
                : m_Width(width), m_Height(height) {}

        unsigned int GetWidth() const { return m_Width; }
        unsigned int GetHeight() const { return m_Height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::WindowResize)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    private:
        unsigned int m_Width, m_Height;
    };

    class VXM_API WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(EventType::WindowClose)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    };

    class VXM_API AppTickEvent : public Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppTick)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    };

    class VXM_API AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    };

    class VXM_API AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppRender)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    };
}
#endif //VOXYMORE_APPLICATIONEVENT_HPP

//
// Created by ianpo on 14/07/2023.
//

#ifndef VOXYMORE_MOUSEEVENT_HPP
#define VOXYMORE_MOUSEEVENT_HPP

#include "Event.hpp"

namespace Voxymore::Core {

    class VXM_API MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(const float x, const float y)
                : m_MouseX(x), m_MouseY(y) {}

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
    private:
        float m_MouseX, m_MouseY;
    };

    class VXM_API MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset)
                : m_XOffset(xOffset), m_YOffset(yOffset) {}

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
    private:
        float m_XOffset, m_YOffset;
    };

    class VXM_API MouseButtonEvent : public Event
    {
    public:
        int GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput | EventCategory::EventCategoryMouseButton)
    protected:
        MouseButtonEvent(const int button)
                : m_Button(button) {}

        int m_Button;
    };

    class VXM_API MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(const int button)
                : MouseButtonEvent(button) {}

        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
    };

    class VXM_API MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(const int button)
                : MouseButtonEvent(button) {}

        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
    };

}
#endif //VOXYMORE_MOUSEEVENT_HPP

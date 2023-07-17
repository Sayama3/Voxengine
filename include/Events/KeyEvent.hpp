//
// Created by ianpo on 14/07/2023.
//

#ifndef VOXYMORE_KEYEVENT_HPP
#define VOXYMORE_KEYEVENT_HPP

#include "VoxymoreCore.hpp"
#include "Event.hpp"

namespace Voxymore::Core {
    namespace Events {
        class KeyEvent : public Event {
        public:
            inline int GetKeyCode() const { return m_KeyCode; }
            EVENT_CLASS_CATEGORY(EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard)
        protected:
            KeyEvent(int keycode) : m_KeyCode(keycode) {}

            int m_KeyCode;
        };

        class KeyPressedEvent : public KeyEvent {
        public:
            KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

            inline int GetRepeatCount() const { return m_RepeatCount; }
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
                return ss.str();
            }

            EVENT_CLASS_TYPE(EventType::KeyPressed)

        private:
            int m_RepeatCount;
        };

        class KeyReleasedEvent : public KeyEvent
        {
        public:
            KeyReleasedEvent(const int keycode) : KeyEvent(keycode) {}

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyReleasedEvent: " << m_KeyCode;
                return ss.str();
            }

            EVENT_CLASS_TYPE(EventType::KeyReleased)
        };
    }
}

#endif //VOXYMORE_KEYEVENT_HPP

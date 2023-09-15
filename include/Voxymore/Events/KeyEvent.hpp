//
// Created by ianpo on 14/07/2023.
//

#ifndef VOXYMORE_KEYEVENT_HPP
#define VOXYMORE_KEYEVENT_HPP

#include "Event.hpp"
#include "Voxymore/Core/KeyCodes.hpp"

namespace Voxymore::Core {
        class KeyEvent : public Event {
        public:
            inline Key GetKeyCode() const { return m_KeyCode; }
            EVENT_CLASS_CATEGORY(EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard)
        protected:
            inline KeyEvent(Key keycode) : m_KeyCode(keycode) {}
            inline KeyEvent(int keycode) : m_KeyCode(static_cast<Key>(keycode)) {}

			Key m_KeyCode;
        };

        class KeyPressedEvent : public KeyEvent {
        public:
			KeyPressedEvent(const Key keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}
			KeyPressedEvent(const int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

            inline int GetRepeatCount() const { return m_RepeatCount; }
            inline std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyPressedEvent: " << static_cast<int>(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
                return ss.str();
            }

            EVENT_CLASS_TYPE(EventType::KeyPressed)

        private:
            int m_RepeatCount;
        };

        class KeyReleasedEvent : public KeyEvent
        {
        public:
			KeyReleasedEvent(const Key keycode) : KeyEvent(keycode) {}
			KeyReleasedEvent(const int keycode) : KeyEvent(keycode) {}

            inline std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyReleasedEvent: " << static_cast<int>(m_KeyCode);
                return ss.str();
            }

            EVENT_CLASS_TYPE(EventType::KeyReleased)
        };

        class KeyTypedEvent : public KeyEvent
        {
        public:
			KeyTypedEvent(const Key keycode) : KeyEvent(keycode) {}
			KeyTypedEvent(const int keycode) : KeyEvent(keycode) {}

            inline std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyTypedEvent: " << static_cast<int>(m_KeyCode);
                return ss.str();
            }

            EVENT_CLASS_TYPE(EventType::KeyTyped)
        };

}

#endif //VOXYMORE_KEYEVENT_HPP

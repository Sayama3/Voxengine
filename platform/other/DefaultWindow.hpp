//
// Created by ianpo on 14/07/2023.
//

#pragma once
#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>
#include "Window.hpp"

namespace Voxymore {
    namespace Core {

        class DefaultWindow : public Window {
        public:
            DefaultWindow(const WindowProps& props);
            virtual ~DefaultWindow();

            void OnUpdate() override;

            inline unsigned int GetWidth() const override { return m_Data.Width; }
            inline unsigned int GetHeight() const override { return m_Data.Height; }


            inline void SetEventCallback(const EventCallBackFn& callback) override { m_Data.EventCallback = callback; }
            void SetVSync(bool enabled) override;
            bool IsVSync() const override;
        private:
            virtual void Init(const WindowProps& props);
            virtual void Shutdown();
        private:
            GLFWwindow* m_Window;
            struct DefaultData
            {
                std::string Title;
                unsigned int Width, Height;
                bool VSync;

                EventCallBackFn  EventCallback;
            };
            DefaultData m_Data;
        };

    } // Voxymore
} // Core

//
// Created by ianpo on 14/07/2023.
//

#ifndef VOXYMORE_WINDOW_HPP
#define VOXYMORE_WINDOW_HPP

#include "Voxymore/Events/Event.hpp"
#include "Core.hpp"
#include "Voxymore/Renderer/GraphicsContext.hpp"

namespace Voxymore::Core {
    enum class CursorState {
        None,
        Locked,
    };

    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Voxymore Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
                : Title(title), Width(width), Height(height)
        {

        }
    };

    // Interface for a desktop window.
    class Window
    {
    public:
        using EventCallBackFn = std::function<void(Voxymore::Core::Event&)>;

        virtual ~Window() {}

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        //Window Attributes
        virtual void SetEventCallback(const EventCallBackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;
        virtual bool IsMinify() const = 0;
        virtual void SetCursorState(CursorState cursorState) = 0;

        virtual void* GetNativeWindow() = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    protected:
        GraphicsContext* m_Context;
    };
}

#endif //VOXYMORE_WINDOW_HPP

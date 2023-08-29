/*Copyright 2022 Kevin Poly*/
#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "../Events/Event.h"

struct WindowProps {
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string& title = "Caisy", unsigned int width = 1280, unsigned int height = 720)
        : Title(title), Width(width), Height(height)
    {
    }
};

class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowProps& props);
        virtual ~Window();

        void OnUpdate();

        inline unsigned int GetWidth() const {return m_Data.Width;}
        inline unsigned int GetHeight() const {return m_Data.Height;}

        inline void SetEventCallback(const EventCallbackFn& callback) {m_Data.EventCallback = callback;}

        static Window* Create(const WindowProps& props = WindowProps());

        inline void* GetNativeWindow() const { return m_Window; }
    private:
        GLFWwindow* m_Window;

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;

        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
};

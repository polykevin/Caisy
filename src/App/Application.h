/*Copyright 2022 Kevin Poly*/
#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "../Events/Event.h"
#include "../Events/ApplicationEvent.h"
#include "WindowClass.h"
#include "ObjectStack.h"

class Application 
{
    public:
        Application();
        ~Application();
        void Run();

        void OnEvent(Event& e);

        inline Window& GetWindow() { return *m_Window; }
        inline static Application* Get() { return s_Instance; }

        std::string file;
    private:
        static Application*  s_Instance;
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        ObjectStack m_ObjectStack;
};

/*Copyright 2022 Kevin Poly*/
#include "../Renderer/VAO/VAO.h"
#include "../Renderer/VBO/VBO.h"
#include "../Renderer/Shader/Shader.h"
#include "../TextEditor/TextEditor.h"
#include "../Events/Input.h"
#include "Application.h"
#include <iostream>
#include <stdlib.h>
#include <ft2build.h>
#include <GLFW/glfw3.h>
#include FT_FREETYPE_H

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application()
{
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Application::~Application()
{
    delete Input::Get();
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    for (auto it = m_ObjectStack.end(); it != m_ObjectStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
}

void Application::Run()
{
    TextEditor::TextRenderer object1;
    m_ObjectStack.PushObject(&object1);

    if (!file.empty())
        object1.LoadFile(file);

    // Render Loop
    while(m_Running) {
        // Render
        glClearColor(0.949f, 0.925f, 0.921f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        object1.Run();

        m_Window->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return true;
}

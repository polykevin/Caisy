/*Copyright 2022 Kevin Poly*/
#pragma once

#include <utility>

class Input {
    public:
        inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
        inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        inline static Input* Get() { return s_Instance; }
    protected:
        bool IsKeyPressedImpl(int keycode);
		bool IsMouseButtonPressedImpl(int button);
		std::pair<float, float> GetMousePositionImpl();
		float GetMouseXImpl();
		float GetMouseYImpl();
    private:
        static Input* s_Instance;
};

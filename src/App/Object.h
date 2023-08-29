/*Copyright 2022 Kevin Poly*/
#pragma once
#include "../Events/Event.h"

class Object {
    public:
        virtual void OnEvent(Event& e) {}
};

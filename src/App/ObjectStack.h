/*Copyright 2022 Kevin Poly*/
#pragma once
#include <vector>
#include "Object.h"

class ObjectStack {
    public:
        ObjectStack();
        ~ObjectStack();

        void PushObject(Object* object);
        void PopObject(Object* object);

        std::vector<Object*>::iterator begin() { return m_Objects.begin(); }
        std::vector<Object*>::iterator end() { return m_Objects.end(); }
    private:
        std::vector<Object*> m_Objects;
        unsigned int m_ObjectIndex = 0;

};


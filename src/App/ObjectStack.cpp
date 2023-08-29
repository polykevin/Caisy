/*Copyright 2022 Kevin Poly*/
#include "ObjectStack.h"

ObjectStack::ObjectStack() {
}

ObjectStack::~ObjectStack() {
    for (Object* object : m_Objects)
        delete object;
}

void ObjectStack::PushObject(Object* object) {
    m_Objects.emplace(m_Objects.begin() + m_ObjectIndex, object);
    m_ObjectIndex++;
}

void ObjectStack::PopObject(Object* object) {
		auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
		if (it != m_Objects.end())
		{
			m_Objects.erase(it);
			m_ObjectIndex--;
		}
}

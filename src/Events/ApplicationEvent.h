/*Copyright 2022 Kevin Poly*/
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "Event.h"

class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_Width(width), m_Height(height) {}

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }
        
        static EventType GetStaticType() { return EventType::WindowResize; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "WindowResizeEvent"; }

        virtual int GetCategoryFlags() const override { return EventCategoryApplication; }
    private:
            unsigned int m_Width, m_Height;
};



class WindowCloseEvent : public Event

{
    public:
        WindowCloseEvent() {}

        static EventType GetStaticType() { return EventType::WindowClose; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "WindowCloseEvent"; }

        virtual int GetCategoryFlags() const override { return EventCategoryApplication; }
};

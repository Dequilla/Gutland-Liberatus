#pragma once

#include <functional>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


enum class StateType;

namespace  Kengine
{
    enum class EventType
    {
        KeyDown       = sf::Event::KeyPressed,
        KeyUp         = sf::Event::KeyReleased,
        MButtonDown   = sf::Event::MouseButtonPressed,
        MButtonUp     = sf::Event::MouseButtonReleased,
        MouseWheel    = sf::Event::MouseWheelMoved,
        WindowResized = sf::Event::Resized,
        GainedFocus   = sf::Event::GainedFocus,
        LostFocus     = sf::Event::LostFocus,
        MouseEntered  = sf::Event::MouseEntered,
        MouseLeft     = sf::Event::MouseLeft,
        Closed        = sf::Event::Closed,
        TextEntered   = sf::Event::TextEntered,
        Keyboard      = sf::Event::Count + 1, Mouse, Joystick
    };

    struct EventInfo
    {
        EventInfo()
        {
            m_code = 0;
        }
        EventInfo(int event)
        {
            m_code = event;
        }

        union
        {
            int m_code;
        };
    };

    using Events = std::vector < std::pair < EventType, EventInfo >>;

    struct EventDetails
    {
        EventDetails(const std::string & bindName) :
            m_name(bindName)
        {
            Clear();
        }

        std::string  m_name;

        sf::Vector2i m_size;
        sf::Uint32   m_textEntered;
        sf::Vector2i m_mouse;
        int          m_mouseWheelDelta;
        int          m_keyCode; // Single key code.

        void         Clear()
        {
            m_size            = sf::Vector2i(0, 0);
            m_textEntered     = 0;
            m_mouse           = sf::Vector2i(0, 0);
            m_mouseWheelDelta = 0;
            m_keyCode         = -1;
        }
    };

    struct Binding
    {
        Binding(const std::string & name) :
            m_name(name), m_details(name), c(0)
        {
        }

        void BindEvent(EventType type, EventInfo info = EventInfo())
        {
            m_events.emplace_back(type, info);
        }

        Events       m_events;
        std::string  m_name;
        int          c; // Count of events that are happening.

        EventDetails m_details;
    };

    using Bindings = std::unordered_map < std::string, Binding* >;

    using CallbackContainer = std::unordered_map <
                              std::string, std::function < void(EventDetails*) >>;


    using Callbacks = std::unordered_map <
                      StateType, CallbackContainer >;

    struct EventManager
    {
        EventManager();
        ~EventManager();

        bool AddBinding(Binding *binding);
        bool RemoveBinding(std::string name);
        void SetFocus(const bool& focus);

        void SetCurrentState(const StateType& state);

        // Needs to be defined in the header!
        template < class T >
        bool AddCallback(StateType state, const std::string& name,
                         void (T::*func)(EventDetails*), T* instance)
        {
            auto itr = m_callbacks.emplace(
                state, CallbackContainer()).first;

            auto temp = std::bind(func, instance,
                                  std::placeholders::_1);
            return itr->second.emplace(name, temp).second;
        }

        bool RemoveCallback(StateType state, const std::string& name)
        {
            // Itterate through our callbacks to find the state pair in the first map
            auto itr = m_callbacks.find(state);

            // If the state isn't in our map
            if (itr == m_callbacks.end())
            {
                return false;
            }

            auto itr2 = itr->second.find(name);
            if (itr2 == itr->second.end())
            {
                return false;
            }

            // Erase the callback by its name
            itr->second.erase(name);

            // If everything is successful
            return true;
        }

        void         HandleEvent(sf::Event& event);
        void         Update();

        sf::Vector2i GetMousePos(sf::RenderWindow* window = nullptr)
        {
            return(window ? sf::Mouse::getPosition(*window) :
                   sf::Mouse::getPosition());
        }

private:
        Bindings  m_bindings;
        Callbacks m_callbacks;
        StateType m_currentState;
        bool      m_hasFocus;

        void      LoadBindings();
    };
}

#include "Textbox.h"

#include <iostream>

namespace Kengine
{
Textbox::Textbox()
{
    Setup(5, 9, 200, sf::Vector2f(0, 0));
}

Textbox::Textbox(int visible, int charSize, int width, sf::Vector2f screenPos)
{
    Setup(visible, charSize, width, screenPos);
}

Textbox::~Textbox()
{
    Clear();
}

void Textbox::Setup(int visible, int charSize, int width, sf::Vector2f screenPos)
{
    m_numVisible = visible;
    sf::Vector2f offset(2.0f, 2.0f);

    if (!m_font.loadFromFile("liberation-mono.ttf"))
    {
        std::cout << "Error loading font!" << std::endl;
    }

    m_content.setFont(m_font);
    m_content.setString("");
    m_content.setCharacterSize(charSize);
    m_content.setColor(sf::Color::White);
    m_content.setPosition(screenPos + offset);
}

void Textbox::Add(std::string message)
{
    m_messages.push_back(message);
    if (m_messages.size() < 6)
    {
        return;
    }
    m_messages.erase(m_messages.begin());
}

void Textbox::Clear()
{
    m_messages.clear();
}

void Textbox::Render(sf::RenderWindow & window)
{
    std::string content;

    for (auto &itr : m_messages)
    {
        content.append(itr + "\n");
    }

    if (content != "")
    {
        m_content.setString(content);
        window.draw(m_backdrop);
        window.draw(m_content);
    }
}
}

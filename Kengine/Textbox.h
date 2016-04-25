#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

namespace Kengine
{
    using MessageContainer = std::vector < std::string >;

    struct Textbox
    {
        Textbox();
        Textbox(int visible, int charSize, int width, sf::Vector2f screenPos);
        ~Textbox();

        void               Setup(int visible, int charSize, int width, sf::Vector2f screenPos);
        void               Add(std::string message);
        void               Clear();

        void               Render(sf::RenderWindow& window);

private:
        MessageContainer   m_messages;
        int                m_numVisible;

        sf::RectangleShape m_backdrop;
        sf::Font           m_font;
        sf::Text           m_content;
    };
}

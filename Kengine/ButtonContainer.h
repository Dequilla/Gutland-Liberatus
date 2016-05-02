#pragma once

#include <unordered_map>
#include <functional>

#include "Button.h"
#include "EventManager.h"



namespace Kengine
{

	class ButtonContainer
	{

	public:
		ButtonContainer();
		~ButtonContainer();

		void update(sf::Vector2i mousePos, sf::RenderWindow* window);
		void draw();

		std::string getActiveButton();
		Kengine::Button* getButton(std::string name);

		void addButton(std::string name, sf::Vector2f position, sf::Vector2f size, sf::Color textColor, sf::Color backgroundColor, sf::Color hoverColor);

	private:
		std::unordered_map<std::string, Kengine::Button>	m_buttons;
		sf::Font											m_font;
		sf::RenderWindow*									m_window;
		sf::Vector2i										m_mousePos;
	};

}
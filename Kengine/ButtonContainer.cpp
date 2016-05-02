#include "ButtonContainer.h"

namespace Kengine
{
	ButtonContainer::ButtonContainer()
	{
		if (!m_font.loadFromFile("media/Fonts/Steamwreck.otf"))
		{
			std::cout << "Could not load fonts/AYearWithoutRain.ttf!" << std::endl;
		}
	}

	ButtonContainer::~ButtonContainer()
	{
	
	}

	void ButtonContainer::update(sf::Vector2i mousePos, sf::RenderWindow* window)
	{
		m_window = window;
		m_mousePos = mousePos;
		
		//Check buttons
		for (auto &itr : m_buttons)
		{
			if (itr.second.contains(m_mousePos)) itr.second.setActive(true);
			else itr.second.setActive(false);
		}
	}
	void ButtonContainer::draw()
	{
		//Draw buttons
		for (auto &itr : m_buttons) {
			m_window->draw(itr.second);
		}
	}

	std::string ButtonContainer::getActiveButton()
	{
		for (auto &itr : m_buttons)
		{		
			//if the button is active return its key/name
			if (itr.second.isActive())
			{
				return itr.first;
			}
		}

		return "";
	}

	Kengine::Button* ButtonContainer::getButton(std::string name)
	{
		return &m_buttons.find(name)->second;
	}

	void ButtonContainer::addButton(std::string name, sf::Vector2f position, sf::Vector2f size, sf::Color textColor, sf::Color backgroundColor, sf::Color hoverColor)
	{

		std::cout << "Added button" << std::endl;

		Kengine::Button temp_button;

		temp_button.setFont(m_font);
		temp_button.setString(name);
		temp_button.setName(name);
		temp_button.setPosition(position);
		temp_button.setSize(size);
		temp_button.setTextColor(textColor);
		temp_button.setBackgroundColor(backgroundColor);
		temp_button.saveBackgroundColor(backgroundColor);
		temp_button.setHoverColor(hoverColor);

		m_buttons.emplace(name, temp_button);
	}
};
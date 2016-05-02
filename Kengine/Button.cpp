#include "Button.h"

namespace Kengine
{
	Button::Button()
	{
	
	}

	Button::Button(sf::Font &font, sf::Vector2f size)
	{
		this->setSize(size);
		this->setFont(font);	
	}

	void Button::setSize(const sf::Vector2f size)
	{
		m_size = size;
		m_body.setSize(m_size);
		this->update();
	}

	void Button::setString(const std::string string)
	{
		this->m_text.setString(string);
	}

	void Button::setFont(sf::Font & font)
	{
		m_font = & font;
		m_text.setFont(*m_font);
		this->update();
	}

	void Button::setTextSize(const int size)
	{
		m_text.setCharacterSize(size);
	}

	void Button::setOptimalTextSize()
	{
		if (m_size.x > 0 && m_size.y > 0 && m_text.getString() != "")
		{
			if (m_text.getGlobalBounds().width < m_size.x / 4 * 3 && m_text.getGlobalBounds().height < m_size.y - 10.0f)
			{
				while (m_text.getGlobalBounds().width < m_size.x / 4 * 3 && m_text.getGlobalBounds().height < m_size.y - 10.f)
				{
					m_text.setCharacterSize(m_text.getCharacterSize() + 1);
				}
				if (m_text.getGlobalBounds().width < m_size.x / 4 * 3 || m_text.getGlobalBounds().height < m_size.y - 10.f)
				{
					m_text.setCharacterSize(m_text.getCharacterSize() - 1);
				}
			}
			else
			{
				while (m_text.getGlobalBounds().width > m_size.x / 4 * 3 && m_text.getGlobalBounds().height > m_size.y - 10.f)
				{
					m_text.setCharacterSize(m_text.getCharacterSize() - 1);
				}
				if (m_text.getGlobalBounds().width > m_size.x / 4 * 3 || m_text.getGlobalBounds().height > m_size.y - 10.f)
				{
					m_text.setCharacterSize(m_text.getCharacterSize() + 1);
				}
			}
			m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2);
			m_text.setPosition(m_size.x / 2, m_size.y / 2);
		}
	}

	void Button::setVisible(bool visible)
	{
		m_visible = visible;
	}

	void Button::events(sf::Event & event)
	{
		if (m_clicked == true) m_clicked = false;
	

		if (event.type == sf::Event::MouseButtonPressed && m_mouseCursorOver)
		{
			m_buttonPressed = true;
		}
		else if (m_buttonPressed && event.type == sf::Event::MouseButtonReleased)
		{
			m_buttonPressed = false;
			if (m_mouseCursorOver)
			{
				m_clicked = true;
			}
		}
	}

	void Button::update()
	{
		setOptimalTextSize();
	}

	void Kengine::Button::setSpriteSheetLeft(const std::string & path)
	{
		m_spriteSheetLeft->LoadSheet(path);
	}

	void Kengine::Button::setSpriteSheetMiddle(const std::string & path)
	{
		m_spriteSheetMiddle->LoadSheet(path);
	}

	void Kengine::Button::setSpriteSheetRight(const std::string & path)
	{
		m_spriteSheetRight->LoadSheet(path);
	}

	void Kengine::Button::setActive(bool tof)
	{
		//tof = TRUE or FALSE
		m_active = tof;

		if (m_active == true)
		{
			setBackgroundColor(m_hoverColor);
		}
		else
		{
			setBackgroundColor(m_backgroundColor);
		}
	}

	void Button::setBackgroundColor(const sf::Color & color)
	{
		this->m_body.setFillColor(color);
	}

	void Kengine::Button::saveBackgroundColor(const sf::Color & color)
	{
		this->m_backgroundColor = color;
	}

	void Button::setTextColor(const sf::Color & color)
	{	
		this->m_text.setColor(color);
	}

	void Kengine::Button::setHoverColor(const sf::Color & color)
	{
		m_hoverColor = color;
	}

	void Button::setOutlineThickness(float thickness)
	{
		this->m_body.setOutlineThickness(thickness);
	}

	void Button::setOutlineColor(const sf::Color & color)
	{
		this->m_outlineColor = color;
		this->m_body.setOutlineColor(color);
	}

	bool Kengine::Button::contains(sf::Vector2i point)
	{
		sf::FloatRect temp = getLocalBounds();

		float sizeX = m_size.x;
		float sizeY = m_size.y;

		if (point.x >= getPosition().x &&
			point.x <= getPosition().x + sizeX &&
			point.y >= getPosition().y &&
			point.y <= getPosition().y + sizeY)
		{
			return true;
		}
		return false;
	}

	void Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		if(m_visible)
		{
			states.transform *= getTransform();
			target.draw(m_body, states);
			target.draw(m_text, states);
		}
	} 
}    
#pragma once

#include "SFML/Graphics.hpp"

namespace Kengine
{
	class Button : public sf::Drawable, public sf::Transformable
	{
	public:
		Button() {};
		Button(sf::Font &font, const sf::Vector2f size = sf::Vector2f(200,50));

		void setSize(const sf::Vector2f size);
		void setString(const std::string string);
		void setFont(sf::Font &font);
		void setTextSize(const int size);

		bool clicked() { return this->m_clicked; };
		void events(sf::Event &event);
		void handler(sf::RenderWindow &window);
		void update();

		void setOverAction(bool active = true);
		void setOverBackgroundColor(const sf::Color &color);
		void setOverTextColor(const sf::Color &color);
		void setOverOutlineColor(const sf::Color &color);

		void setBackgroundColor(const sf::Color &color);
		void setTextColor(const sf::Color &color);
		void setOutlineThickness(float thickness);
		void setOutlineColor(const sf::Color &color);

		std::string getString() { return m_text.getString(); }
		sf::FloatRect getLocalBounds() { return m_body.getLocalBounds(); }

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool					 	 m_overActive;
		bool						 m_mouseCursorOver;
		bool						 m_buttonPressed;
		bool						 m_clicked;

		sf::Color					 m_overBackgroundColor;
		sf::Color					 m_overTextColor;
		sf::Color					 m_overOutlineColor;
		sf::Color					 m_backgroundColor;
		sf::Color					 m_textColor;
		sf::Color					 m_outlineColor;
		sf::RectangleShape			 m_body;
		sf::Vector2f				 m_size;
		sf::Text					 m_text;
		sf::Font				    *m_font;
	};

}
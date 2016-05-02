#pragma once

#include "SFML/Graphics.hpp"
#include "SpriteSheet.h"

namespace Kengine
{
	class Button : public sf::Drawable, public sf::Transformable
	{
	public:
		Button();
		Button(sf::Font &font, const sf::Vector2f size = sf::Vector2f(200,50));

		void setSize(const sf::Vector2f size);
		void setString(const std::string string);
		void setFont(sf::Font &font);
		void setTextSize(const int size);
		void setOptimalTextSize();
		void setVisible(bool visible);

		bool clicked() { return this->m_clicked; };
		void events(sf::Event &event);
		void update();

		void setSpriteSheetLeft(const std::string& path);
		void setSpriteSheetMiddle(const std::string& path);
		void setSpriteSheetRight(const std::string& path);
		void setActive(bool tof);

		void setBackgroundColor(const sf::Color &color);
		void saveBackgroundColor(const sf::Color &color);
		void setTextColor(const sf::Color &color);
		void setHoverColor(const sf::Color &color);
		void setOutlineThickness(float thickness);
		void setOutlineColor(const sf::Color &color);

		void setName(std::string name) { m_name = name; }

		std::string getString() { return m_text.getString(); }
		sf::FloatRect getLocalBounds() { return m_body.getLocalBounds(); }
		bool contains(sf::Vector2i point);
		bool isActive() { return m_active; }

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool						 m_mouseCursorOver;
		bool						 m_buttonPressed;
		bool						 m_clicked;

		bool						m_active;
		bool						m_visible = true;

		std::string					m_name;
		Kengine::SpriteSheet*		m_spriteSheetLeft;
		Kengine::SpriteSheet*		m_spriteSheetMiddle;
		Kengine::SpriteSheet*		m_spriteSheetRight;


		sf::Color					 m_backgroundColor;
		sf::Color					 m_textColor;
		sf::Color					 m_outlineColor;
		sf::Color					 m_hoverColor;
		sf::RectangleShape			 m_body;
		sf::Vector2f				 m_size;
		sf::Text					 m_text;
		sf::Font				    *m_font;
	};

}
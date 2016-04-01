#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class DebugOverlay
{
public:
	DebugOverlay()
	{
		m_debug = false;
	}

	void Add(sf::Drawable* drawable)
	{
		m_drawables.push_back(drawable);
	}

	void Draw(sf::RenderWindow* window)
	{
		while (m_drawables.begin() != m_drawables.end())
		{
			window->draw(*m_drawables.back());
			delete m_drawables.back();
			m_drawables.pop_back();
		}
	}

	bool Debug() { return m_debug; }
	void SetDebug(const bool& val) { m_debug = val; }

private:
	std::vector<sf::Drawable*> m_drawables;
	bool m_debug;
};
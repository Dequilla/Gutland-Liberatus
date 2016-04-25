#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "TextureManager.h"
#include "DirectionalAnimation.h"
#include <unordered_map>
namespace Kengine
{
    using Animations = std::unordered_map < std::string, BaseAnimation* >;

    class SpriteSheet
    {
public:
        SpriteSheet(TextureManager * textMgr);
        ~SpriteSheet();

        void CropSprite(const sf::IntRect& rect);
        // ... Basic setters/getters
        sf::Vector2i GetSpriteSize() const;
        sf::Vector2f GetSpritePosition() const;
        Direction GetDirection() const;

        void SetSpriteSize(const sf::Vector2i& size);
        void SetSpritePosition(const sf::Vector2f& pos);
        void SetDirection(const Direction& dir);
        void SetSpriteFrame(const int& frame);

        bool LoadSheet(const std::string& file);
        void ReleaseSheet();

        BaseAnimation* GetCurrentAnimation();
        bool SetAnimation(const std::string& name,
                          const bool& play = false,
                          const bool& loop = false);

        void Update(const float& dt);
        void Draw(sf::RenderWindow* window);

private:
        std::string   m_texture;
        sf::Sprite    m_sprite;
        sf::Vector2i  m_spriteSize;
        sf::Vector2f  m_spriteScale;
        Direction     m_direction;
        std::string   m_animType;
        Animations    m_animations;
        BaseAnimation * m_animationCurrent;
        TextureManager* m_textureManager;
    };
}

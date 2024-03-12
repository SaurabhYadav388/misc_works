#pragma once

#include <SFML/Graphics.hpp>

class Target
{
public:
	Target();
	void setPosition(const float& posX, const float& posY);
	sf::Vector2f getPosition();
	sf::Vector2f getSize();
	bool isHit(sf::Vector2f arrowHeadPosition);
	int pointsEarned(sf::Vector2f arrowHeadPosition);

	void move(const float& offsetX, const float& offsetY);
	void draw(sf::RenderTarget& target);
private:
	
	sf::Texture targetTexture;
	sf::Sprite targetSpriteShape;

	float targetMoveSpeed;
};
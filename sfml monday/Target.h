#pragma once

#include <SFML/Graphics.hpp>

class Target
{
public:
	Target();
	void setPosition(const float& posX, const float& posY);
	sf::Vector2f getPosition();
	float getRadius();
	bool isHit(sf::Vector2f arrowHeadPosition);
	void move(const float& offsetX, const float& offsetY);
	void draw(sf::RenderTarget& target);
private:
	sf::CircleShape targetShape;
	sf::CircleShape testShape;//////

	float targetMoveSpeed;
};
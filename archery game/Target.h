#include <SFML/Graphics.hpp>

#pragma once
class Target
{
public:
	Target();
	void setPosition(const float& posX,const float& posY);
	sf::Vector2f getPosition();
	float getRadius();
	void move(const float& offsetX,const float& offsetY);
	void draw(sf::RenderTarget& target);
private:
	sf::CircleShape targetShape;
	float targetMoveSpeed;
};


#pragma once

#include <SFML/Graphics.hpp>

class Arrow
{
public:
	Arrow();
	void setPosition(const float& posX, const float& posY);
	void setProjAngle(const float& _angle);
	void setProjSpeed(const float& _speed);

	void setRotation(const float& rotation);

	sf::Vector2f getPosition();
	float getProjAngle();
	float getProjSpeed();

	sf::Vector2f getArrowHeadPosition();

	void moveArrow(float time, sf::Vector2f initPos);

	void draw(sf::RenderTarget& target);
private:
	sf::ConvexShape arrowShape;
	float projSpeed;
	float projAngle;

};
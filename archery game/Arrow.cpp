#include "Arrow.h"

Arrow::Arrow()
{
    arrowShape.setPointCount(7);

    arrowShape.setPoint(0, sf::Vector2f(0, 1));
    arrowShape.setPoint(1, sf::Vector2f(30, 1));
    arrowShape.setPoint(2, sf::Vector2f(35, 3));
    arrowShape.setPoint(3, sf::Vector2f(40, 0));
    arrowShape.setPoint(4, sf::Vector2f(35, -3));
    arrowShape.setPoint(5, sf::Vector2f(30, -1));
    arrowShape.setPoint(6, sf::Vector2f(0, -1));
    arrowShape.setFillColor(sf::Color::Green);


    arrowShape.setPosition(100, 300);
    arrowShape.setOrigin(sf::Vector2f(40, 0)); //origin as arrow head

    arrowShape.scale(3, 3);
}

void Arrow::setPosition(const float& posX,const  float& posY)
{
	arrowShape.setPosition(posX, posY);
}

void Arrow::setRotation(const float& angle)
{
	arrowShape.setRotation(angle);
}

sf::Vector2f Arrow::getPosition()
{
	return arrowShape.getGlobalBounds().getPosition();
}

float Arrow::getRotation()
{
	return arrowShape.getRotation();
}

sf::Vector2f Arrow::getArrowHeadPosition()
{
    return arrowShape.getTransform().transformPoint(arrowShape.getPoint(3)); //third index point is the head
}

void Arrow::draw(sf::RenderTarget& target)
{
    target.draw(arrowShape);
}



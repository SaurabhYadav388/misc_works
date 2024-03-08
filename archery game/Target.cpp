#include "Target.h"

Target::Target()
{
    targetShape.setRadius(100.f);
    targetShape.setOrigin(100.f, 100.f); // Set origin to the center
    targetShape.setPosition(800, 300);
    targetShape.setFillColor(sf::Color::White);
}

void Target::setPosition(const float& posX,const float& posY)
{
    targetShape.setPosition(posX, posY);
}

sf::Vector2f Target::getPosition()
{
    return targetShape.getGlobalBounds().getPosition();
}

float Target::getRadius()
{
    return targetShape.getRadius();
}

void Target::move(const float& offsetX,const float& offsetY)
{
    targetShape.move(offsetX, offsetY);
}

void Target::draw(sf::RenderTarget& target)//target is window
{
    target.draw(targetShape);
}

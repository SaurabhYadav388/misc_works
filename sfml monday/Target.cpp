#include "Target.h"
/*https://github.com/spiniferx/BowAndArrow/tree/master/Assets/bowandarrow/Textures*/
Target::Target()
{
    targetShape.setRadius(100.f);
    targetShape.setOrigin(100.f, 100.f); // Set origin to the center
    targetShape.setPosition(800, 300);
    targetShape.setFillColor(sf::Color::White);
    targetShape.setScale({ 0.5f,1.f });
    ////////////
    testShape.setRadius(100.f);
    testShape.setOrigin(100.f, 100.f); // Set origin to the center
    testShape.setPosition(800, 300);
    testShape.setFillColor(sf::Color::Green);
    

    //////////

    targetMoveSpeed = 0;
}

void Target::setPosition(const float& posX, const float& posY)
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

bool Target::isHit(sf::Vector2f arrowHeadPosition)
{
    float arrowX = arrowHeadPosition.x;
    float arrowY = arrowHeadPosition.y;

    sf::Vector2f centre = targetShape.getTransform().transformPoint(targetShape.getOrigin());
    /*
    if ((arrowX - centre.x) * (arrowX - centre.x) + (arrowY - centre.y) * (arrowY - centre.y) <= getRadius() * getRadius() )
        return true;*/

    float x = arrowX;
    float y = arrowY;
    float h = centre.x;
    float k = centre.y;
    float a = targetShape.getGlobalBounds().width;//getRadius()/2.f;
    float b = targetShape.getGlobalBounds().height;// getRadius();

    if (((x - h) / a) * ((x - h) / a) + ((y - k) / b) * ((y - k) / b) <= 1.f)
        return true;
    
    return false;
}

void Target::move(const float& offsetX, const float& offsetY)
{
    targetShape.move(offsetX, offsetY);
}

void Target::draw(sf::RenderTarget& target)//target is window
{
    target.draw(testShape);
    target.draw(targetShape);
    
}

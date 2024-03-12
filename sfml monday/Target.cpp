#include "Target.h"
/* https://github.com/spiniferx/BowAndArrow/tree/master/Assets/bowandarrow/Textures */
Target::Target()
{
    targetTexture.loadFromFile("./targetImg.png");
    targetSpriteShape.setTexture(targetTexture);

    targetSpriteShape.setOrigin({ targetSpriteShape.getGlobalBounds().getSize().x / 2.f,targetSpriteShape.getGlobalBounds().getSize().y / 2.f });
    targetSpriteShape.setScale({ 0.5f,1.f });

    targetSpriteShape.setPosition(800, 200);

    targetMoveSpeed = 0;
}

void Target::setPosition(const float& posX, const float& posY)
{
    targetSpriteShape.setPosition(posX, posY);
}

sf::Vector2f Target::getPosition()
{
    return targetSpriteShape.getGlobalBounds().getPosition();
}

sf::Vector2f Target::getSize()
{
    return targetSpriteShape.getGlobalBounds().getSize();
}


bool Target::isHit(sf::Vector2f arrowHeadPosition)
{
    sf::Vector2f centre = targetSpriteShape.getTransform().transformPoint(targetSpriteShape.getOrigin());
    
    float x = arrowHeadPosition.x;
    float y = arrowHeadPosition.y;
    float h = centre.x;
    float k = centre.y;
    //decrease width of target to 30% to make arrow appear inside target
    float a = (targetSpriteShape.getGlobalBounds().width /2.f ) * 0.30f;
    float b = targetSpriteShape.getGlobalBounds().height /2.f;

    if (((x - h) / a) * ((x - h) / a) + ((y - k) / b) * ((y - k) / b) <= 1.f)
        return true;

    return false;
}

void Target::move(const float& offsetX, const float& offsetY)
{
    targetSpriteShape.move(offsetX, offsetY);
}


int Target::pointsEarned(sf::Vector2f arrowHeadPosition)
{
    /*
    float aY = arrowHeadPosition.y ;
    float centreY = targetSpriteShape.getTransform().transformPoint(targetSpriteShape.getOrigin()).y;
    if((aY-centreY))
    
    */
    return 0;
}

void Target::draw(sf::RenderTarget& target)//target is window
{
    
    target.draw(targetSpriteShape);

}
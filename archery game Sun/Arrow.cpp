#include "Arrow.h"
#include<iostream>
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

    projSpeed = 0.f;
    projAngle = 0.f;
}

void Arrow::setPosition(const float& posX, const  float& posY)
{
    arrowShape.setPosition(posX, posY);
}

void Arrow::setProjAngle(const float& _angle)
{
    projAngle=_angle;
}

void Arrow::setProjSpeed(const float& _speed)
{
    projSpeed = _speed;
}

void Arrow::setRotation(const float& _rotation)
{
    arrowShape.setRotation(_rotation);
}

sf::Vector2f Arrow::getPosition()
{
    return arrowShape.getGlobalBounds().getPosition();
}

float Arrow::getProjAngle()
{
    return projAngle;
}

float Arrow::getProjSpeed()
{
    return projSpeed;
}

sf::Vector2f Arrow::getArrowHeadPosition()
{
    return arrowShape.getTransform().transformPoint(arrowShape.getPoint(3)); //third index point is the head
}

void Arrow::moveArrow(float time, sf::Vector2f initPos)
{
    float changeX = projSpeed * time * std::cosf(projAngle);
    float changeY = projSpeed * time * std::sinf(projAngle) - 0.5f * 9.8f * time * time;
    
    float nextX = initPos.x + changeX;
    float nextY = initPos.y - changeY;
    float rotateAngle = atan2f(nextY - getArrowHeadPosition().y, nextX - getArrowHeadPosition().x);//new-old
    arrowShape.setRotation((rotateAngle) * (180.0 / 3.14159));
    //first rotate then changePos!!
    setPosition(nextX, nextY);
}

void Arrow::draw(sf::RenderTarget& target)
{
    target.draw(arrowShape);
}

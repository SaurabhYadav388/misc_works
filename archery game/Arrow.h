
#include <SFML/Graphics.hpp>

#pragma once
class Arrow
{
public:
  Arrow();
  void setPosition(const float& posX,const float& posY);
  void setRotation(const float& angle);
  sf::Vector2f getPosition();
  float getRotation();
  sf::Vector2f getArrowHeadPosition();

  void draw(sf::RenderTarget& target);
private:
  sf::ConvexShape arrowShape;
  float speed;
  float rotation;

};


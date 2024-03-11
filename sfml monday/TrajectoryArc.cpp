#include "TrajectoryArc.h"
#include "Arrow.h"
#include<iostream>
TrajectoryArc::TrajectoryArc() :trajectoryLine(sf::LineStrip)
{

}

void TrajectoryArc::updateTrajectoryLine(float& initialXPos, const sf::Vector2i& mousePos, Arrow& arrow)
{
    clearTrajectory();

    float angle = atan2f(arrow.getPosition().y - mousePos.y, mousePos.x - arrow.getPosition().x);//
    float projAngle = -angle;

    //update arrow angle and shape rotation //should seprate
    arrow.setProjAngle(projAngle);
    arrow.setRotation((-projAngle) * (180.0 / 3.14159));//-ve for anti clock

    float speed = 0.5f * (initialXPos - mousePos.x);//.5f random constant ,something relative wrt change in pos 
    arrow.setProjSpeed(speed);
    //std::cout << "sp:" << speed<<" \n";
    //std::cout << "angle: "<<(angle/3.1459) * 180 <<"\n";

    // Simulate the projectile path
    for (float t = 0; t <= 10.0f; t += 0.001f) {
        float x = speed * t * std::cosf(projAngle);
        float y = speed * t * std::sinf(projAngle) - 0.5f * 9.8f * t * t;

        sf::Vector2f arrowheadPosition = arrow.getArrowHeadPosition();
        //std::cout << "1:" << arrow.getPosition().x<<"\n"<<"2:"<<arrowheadPosition.x<<"\n";
        trajectoryLine.append(sf::Vertex(sf::Vector2f(arrowheadPosition.x + x, arrowheadPosition.y - y), sf::Color::Red));

    }
}

void TrajectoryArc::clearTrajectory()
{
    trajectoryLine.clear();
}

const sf::VertexArray& TrajectoryArc::getTrajectoryVertexArray()
{
    return trajectoryLine;
}

void TrajectoryArc::draw(sf::RenderTarget& target)
{
    target.draw(trajectoryLine);
}
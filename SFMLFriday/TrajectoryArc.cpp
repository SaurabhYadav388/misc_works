#include "TrajectoryArc.h"
#include "Arrow.h"

TrajectoryArc::TrajectoryArc():trajectoryLine(sf::LineStrip)
{
	
}

void TrajectoryArc::updateTrajectoryLine(float& angle, float& speed, float& initialXPos, const sf::Vector2i& mousePos, Arrow& arrow)
{
    clearTrajectory();

    angle = atan2f(mousePos.y - arrow.getPosition().y, mousePos.x - arrow.getPosition().x);

    speed = 0.5f * (initialXPos - mousePos.x);//.5f random constant ,something relative wrt change in pos 
    //std::cout << "angle: "<<(angle/3.1459) * 180 <<"\n";
    arrow.setRotation((-angle) * (180.0 / 3.14159));//-ve for anticlockwise

    // Simulate the projectile path
    for (float t = 0; t <= 10.0f; t += 0.001f) {
        float x = speed * t * std::cosf(angle);
        float y = speed * t * std::sinf(angle) - 0.5f * 9.8f * t * t;
        sf::Vector2f arrowheadPosition = arrow.getArrowHeadPosition();
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

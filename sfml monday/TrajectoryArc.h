#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>

#include "Arrow.h"

class TrajectoryArc
{
public:
	TrajectoryArc();
	void updateTrajectoryLine(float& initialXPos, const sf::Vector2i& mousePos, Arrow& arrow);
	void clearTrajectory();
	const sf::VertexArray& getTrajectoryVertexArray();
	void draw(sf::RenderTarget& target);
private:
	sf::VertexArray trajectoryLine;//cant call the constructor here
};

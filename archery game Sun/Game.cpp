#include "Game.h"

void Game::initGame()
{
    float startMousePosX = 0;////
    float moveTime = 0.f;
    float TargMoveSp = .01f;
}


void Game::onMousePressed()
{
    startMousePosX = sf::Mouse::getPosition(window).x;
}

void Game::whileMousePressed()
{
    trajectoryArc.updateTrajectoryLine( startMousePosX, sf::Mouse::getPosition(window), arrow);
}

void Game::onMouseReleased()
{
    //mouseIsClicked = false;//flags should be in main
    //startArrowProjectile = true;
    initArrowPosition = arrow.getArrowHeadPosition();
}

int Game::isCollision()
{
    sf::Vector2f arrowheadPosition = arrow.getArrowHeadPosition();

    if ((arrowheadPosition.x >= (target.getPosition().x + target.getRadius())
        && arrowheadPosition.x <= target.getPosition().x + target.getRadius() * 2
        && arrowheadPosition.y >= (target.getPosition().y)
        && arrowheadPosition.y <= target.getPosition().y + target.getRadius() * 2)
        || (arrowheadPosition.x >= window.getSize().x || arrowheadPosition.y >= window.getSize().y)
        )
        return 1;
    else
        return 0;
}

void Game::onGameEnd()
{
}

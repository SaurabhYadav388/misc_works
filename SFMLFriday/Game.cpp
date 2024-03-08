#include "Game.h"

void Game::initGame()
{
    float startMousePosX = 0;////
    bool mouseIsClickedFlag = false;
    float angle = 0;
    float speed = 0;
    float moveTime = 0.f;
    bool startArrowProjectile = false;
    int projItr = 0;
    float TargMoveSp = .01f;
    bool isGameOver = false;
}

void Game::GameLoop(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed)
        {
            startMousePosX = sf::Mouse::getPosition(window).x;
            mouseIsClickedFlag = true;
        }

        if (mouseIsClickedFlag == true)
        {
            whileMouseClicked(sf::Mouse::getPosition(window));
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            mouseIsClickedFlag = false;
            startArrowProjectile = true;
        }

    }

}

void Game::whileMouseClicked(sf::Vector2i currMousePosition )
{
    trajectoryArc.updateTrajectoryLine(angle, speed, startMousePosX, currMousePosition, arrow);
}

#include "Game.h"

void Game::initGame()
{
    float startMousePosX = 0;////
    arrowTimeInstance = 0.f;
    TargetMoveSpeed = .02f;
    arrow.setPosition(100, 300);
    arrow.setProjSpeed(0.f);
    arrow.setProjAngle(0.f);
    arrow.setRotation(0.f);
    trajectoryArc.clearTrajectory();
    target.setPosition(800, 300); 
    mouseIsClicked = false;
    startArrowProjectile = false;
    initArrowPosition = arrow.getArrowHeadPosition();
}

void Game::runGame()
{
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Archery!!");
    
    while (window.isOpen()) {
        sf::Event  event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                startMousePosX = sf::Mouse::getPosition(window).x;
                mouseIsClicked = true;
            }

            if (mouseIsClicked == true)
            {
                trajectoryArc.updateTrajectoryLine(startMousePosX, sf::Mouse::getPosition(window), arrow);

            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                mouseIsClicked = false;
                startArrowProjectile = true;
                
            }

        }

        updateGame(window.getSize());

        window.clear();
        target.draw(window);
        arrow.draw(window);
        trajectoryArc.draw(window);
        scoreCard.draw(window);//error????
        window.display();

    }
}

void Game::updateGame(sf::Vector2u windowSize)
{
    sf::Vector2f arrowheadPosition = arrow.getArrowHeadPosition();
    //if target collsion
    if (
        target.isHit(arrowheadPosition)
        
        /*(arrowheadPosition.x >= (target.getPosition().x + target.getRadius())
        && arrowheadPosition.x <= target.getPosition().x + target.getRadius() * 2
        && arrowheadPosition.y >= (target.getPosition().y)
        && arrowheadPosition.y <= target.getPosition().y + target.getRadius() * 2)*/
        
        )
    {
        std::cout << "arrow target collide\n";
        //score update here instead
        toGameRestart = true;
    }
    //arrow out of window
    else if (arrowheadPosition.x >= windowSize.x || arrowheadPosition.y == windowSize.y)
    {
        std::cout << "out of bounds\n";
        isGameOver = true;
    }
    else
    {
        if (startArrowProjectile)
        {
            arrow.moveArrow(arrowTimeInstance, initArrowPosition);
            arrowTimeInstance += 0.001f;
        }

        if (target.getPosition().y <= 0 || target.getPosition().y + target.getRadius() * 2 >= windowSize.y)
            TargetMoveSpeed *= -1;
        //target.move(0, TargetMoveSpeed);

    }

    if (isGameOver)
    {
        std::cout << "gameover\n";
        //also reset score
        scoreCard.setScore(0);//also reset score
        sleep(sf::milliseconds(1000));//////
        initGame();
        isGameOver = false;
    }
    if (toGameRestart)
    {
        std::cout << "restart\n";
        //NO score reset here
        scoreCard.increaseScore(1);
        sleep(sf::milliseconds(1000));///////
        initGame();
        toGameRestart = false;
    }
}










//void Game::onMousePressed()
//{
//    startMousePosX = sf::Mouse::getPosition(window).x;
//}
//
//void Game::whileMousePressed()
//{
//    trajectoryArc.updateTrajectoryLine(startMousePosX, sf::Mouse::getPosition(window), arrow);
//}
//
//void Game::onMouseReleased()
//{
//    mouseIsClicked = false;
//    startArrowProjectile = true;
//    initArrowPosition = arrow.getArrowHeadPosition();
//}
//
//int Game::isCollision()
//{
//    sf::Vector2f arrowheadPosition = arrow.getArrowHeadPosition();
//
//    if ((arrowheadPosition.x >= (target.getPosition().x + target.getRadius())
//        && arrowheadPosition.x <= target.getPosition().x + target.getRadius() * 2
//        && arrowheadPosition.y >= (target.getPosition().y)
//        && arrowheadPosition.y <= target.getPosition().y + target.getRadius() * 2)
//        //|| (arrowheadPosition.x >= window.getSize().x || arrowheadPosition.y >= window.getSize().y)
//        )
//        return 1;
//    else
//        return 0;
//}
//
//void Game::onGameEnd()
//{
//}
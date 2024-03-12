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
    //target.setPosition(0, 0); 
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

            //save current mousepressed position
            if (event.type == sf::Event::MouseButtonPressed)
            {
                startMousePosX = sf::Mouse::getPosition(window).x;
                mouseIsClicked = true;
            }

            //calculate trajectory while mouse pressed and dragged
            if (mouseIsClicked == true)
            {
                trajectoryArc.updateTrajectoryLine(startMousePosX, sf::Mouse::getPosition(window), arrow);
            }

            //start the projectile when mousepress released
            if (event.type == sf::Event::MouseButtonReleased)
            {
                mouseIsClicked = false;
                startArrowProjectile = true;
            }

        }

        //update the game
        updateGame(window.getSize());

        //display new update to screen
        window.clear();
        target.draw(window);
        arrow.draw(window);
        trajectoryArc.draw(window);
        scoreCard.draw(window);
        window.display();
    }
}

void Game::updateGame(sf::Vector2u windowSize)
{
    sf::Vector2f arrowheadPosition = arrow.getArrowHeadPosition();
    //if target collsion
    if (target.isHit(arrowheadPosition))
    {
        std::cout << "arrow target collide\n";
        scoreCard.increaseScore(1);
        toGameRestart = true;
    }
    //arrow out of window
    else if (arrowheadPosition.x >= windowSize.x || arrowheadPosition.y >= windowSize.y)
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

        if (target.getPosition().y <= 0 || target.getPosition().y + target.getSize().y >= windowSize.y)
            TargetMoveSpeed *= -1;
        target.move(0, TargetMoveSpeed);

    }

    if (isGameOver)
    {
        std::cout << "gameover\n";
        //also reset score
        scoreCard.setScore(0);
        sleep(sf::milliseconds(1000));//////
        initGame();
        isGameOver = false;
    }
    if (toGameRestart)
    {
        std::cout << "restart\n";        
        sleep(sf::milliseconds(1000));///////
        initGame();
        toGameRestart = false;
    }
}
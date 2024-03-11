#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>

#include "Arrow.h"
#include "Target.h"
#include "TrajectoryArc.h"

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1000, 600), "Archery!!");
    //window.setFramerateLimit(600);//

    Arrow arrow;
    Target target;
    TrajectoryArc trajectoryArc;
    float initialXPos;
    bool mouseIsClicked = false;
    float moveTime = 0.f;
    bool startArrowProjectile = false;
    float TargMoveSp = .01f;
    bool isGameOver = false;
    sf::Vector2f initPos;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                initialXPos = Mouse::getPosition(window).x;
                mouseIsClicked = true;//flags in main only
            }

            if (mouseIsClicked == true)
            {
                trajectoryArc.updateTrajectoryLine(initialXPos, Mouse::getPosition(window), arrow);

            }

            if (event.type == Event::MouseButtonReleased)
            {
                mouseIsClicked = false;
                startArrowProjectile = true;
                initPos = arrow.getArrowHeadPosition();
            }

        }


        //make variable here to
        //iscollision then assign isgameover

        //check collision
        Vector2f arrowheadPosition = arrow.getArrowHeadPosition();

        if ((arrowheadPosition.x >= (target.getPosition().x + target.getRadius())
            && arrowheadPosition.x <= target.getPosition().x + target.getRadius() * 2
            && arrowheadPosition.y >= (target.getPosition().y)
            && arrowheadPosition.y <= target.getPosition().y + target.getRadius() * 2)
            || (arrowheadPosition.x >= window.getSize().x || arrowheadPosition.y >= window.getSize().y)
            )
        {
            std::cout << "collide\n";
            isGameOver = true;
        }
        else
        {
            if (startArrowProjectile )
            {
                
                arrow.moveArrow(moveTime, initPos);
                moveTime += 0.001f;//should be only in main
            }


            if (target.getPosition().y <= 0 || target.getPosition().y + target.getRadius() * 2 >= window.getSize().y)
                TargMoveSp *= -1;
            target.move(0, TargMoveSp);

        }

        if (isGameOver)
        {
            std::cout << "gameover\n";
            initialXPos = 0;
            mouseIsClicked = false;
            moveTime = 0.f;
            startArrowProjectile = false;
            TargMoveSp = .01f;
            arrow.setPosition(100, 300);///hrdcoded
            arrow.setProjAngle(0);
            arrow.setRotation(0.f);
            trajectoryArc.clearTrajectory();
            
            sleep(sf::milliseconds(1000));///
            isGameOver = false;
        }




        window.clear();

        target.draw(window);

        arrow.draw(window);

        trajectoryArc.draw(window);


        window.display();
    }

    return 0;
}
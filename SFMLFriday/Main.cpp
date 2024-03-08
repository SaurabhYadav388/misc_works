
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

/*
in main
game;
game .init();

inside init() initialize then run game.run()->game loop
*/

int main()
{
    RenderWindow window(VideoMode(1000, 600), "Archery!!");
    //window.setFramerateLimit(600);//

    Arrow arrow;
    Target target;
    TrajectoryArc trajectoryArc;

    float initialXPos;

    bool mouseIsClicked = false;
    float angle = 0;
    float speed = 0;
    float moveTime = 0.f;
    bool startArrowProjectile = false;
    int projItr = 0;

    float TargMoveSp = .01f;

    bool isGameOver = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                initialXPos = Mouse::getPosition(window).x;
                mouseIsClicked = true;
            }

            if (mouseIsClicked == true)
            {
                trajectoryArc.updateTrajectoryLine(angle, speed, initialXPos, Mouse::getPosition(window), arrow);
            }

            if (event.type == Event::MouseButtonReleased)
            {
                mouseIsClicked = false;
                startArrowProjectile = true;
            }

        }



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
            if (startArrowProjectile && projItr < trajectoryArc.getTrajectoryVertexArray().getVertexCount())
            {
                arrow.setPosition( trajectoryArc.getTrajectoryVertexArray()[projItr].position.x, trajectoryArc.getTrajectoryVertexArray()[projItr].position.y);

                if (projItr > 0)
                {
                    float rotateAngle = atan2f(
                        trajectoryArc.getTrajectoryVertexArray()[projItr].position.y - trajectoryArc.getTrajectoryVertexArray()[projItr - 1].position.y,
                        trajectoryArc.getTrajectoryVertexArray()[projItr].position.x - trajectoryArc.getTrajectoryVertexArray()[projItr - 1].position.x
                    );
                    arrow.setRotation((rotateAngle) * (180.0 / 3.14159));
                }

                projItr++;
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
            angle = 0;
            speed = 0;
            moveTime = 0.f;
            startArrowProjectile = false;
            projItr = 0;

            TargMoveSp = .01f;
            arrow.setPosition(100, 300);///hrdcoded
            arrow.setRotation(0);
            trajectoryArc.clearTrajectory();

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
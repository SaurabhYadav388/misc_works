
#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>

#include "Arrow.h"
#include "Target.h"

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

    VertexArray trajectoryLine(LineStrip);


    float initialXPos;
    bool mouseIsClicked = false;
    float angle = 0;
    float speed = 0;
    float moveTime = 0.f;
    bool startArrowProjectile = false;
    int f = 0;

    float TargMoveSp = .05f;

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
                trajectoryLine.clear();

                angle = atan2f( Mouse::getPosition(window).y - arrow.getPosition().y,
                                Mouse::getPosition(window).x - arrow.getPosition().x   );
                

                speed = 0.5f * (initialXPos - Mouse::getPosition(window).x);//.2f random constant ,something relative change in pos 
                //std::cout << "angle: "<<(angle/3.1459) * 180 <<"\n";
                arrow.setRotation( (-angle) * (180.0 / 3.14159));//-ve for anticlockwise

                // Simulate the projectile path
                for (float t = 0; t <= 10.0f; t += 0.001f) {
                    float x = speed * t * std::cosf(angle);
                    float y = speed * t * std::sinf(angle) - 0.5f * 9.8f * t * t;
                    Vector2f arrowheadPosition = arrow.getArrowHeadPosition();
                    trajectoryLine.append(Vertex(Vector2f(arrowheadPosition.x + x, arrowheadPosition.y - y), Color::Red));

                }
            }

            if (event.type == Event::MouseButtonReleased)
            {
                mouseIsClicked = false;
                startArrowProjectile = true;
            }
            
        }

        

        //check collision
        Vector2f arrowheadPosition = arrow.getArrowHeadPosition();
        
        if( (arrowheadPosition.x >= (target.getPosition().x + target.getRadius())
            && arrowheadPosition.x <= target.getPosition().x+target.getRadius()*2
            && arrowheadPosition.y >= (target.getPosition().y)
            && arrowheadPosition.y <= target.getPosition().y + target.getRadius() * 2)  
            || (arrowheadPosition.x>=window.getSize().x || arrowheadPosition.y>=window.getSize().y)
          )
        {
            std::cout << "collide\n";
            isGameOver=true;
        }
        else
        {
            if (startArrowProjectile && f < trajectoryLine.getVertexCount())
            {
                arrow.setPosition(trajectoryLine[f].position.x, trajectoryLine[f].position.y);

                if (f > 0)
                {
                    float rotateAngle = atan2f(
                        trajectoryLine[f].position.y - trajectoryLine[f - 1].position.y,
                        trajectoryLine[f].position.x - trajectoryLine[f - 1].position.x
                    );
                    arrow.setRotation((rotateAngle) * (180.0 / 3.14159));
                }

                f++;
            }


            if (target.getPosition().y <= 0 || target.getPosition().y + target.getRadius() * 2 >= window.getSize().y)
                TargMoveSp *= -1;
            target.move(0, TargMoveSp);

        }

        if (isGameOver)
        {
            std::cout << "gameover\n";
            initialXPos=0;
            mouseIsClicked = false;
            angle = 0;
            speed = 0;
            moveTime = 0.f;
            startArrowProjectile = false;
            f = 0;

            TargMoveSp = .05f;
            arrow.setPosition(100, 300);///hrdcoded
            arrow.setRotation(0);
            trajectoryLine.clear();

            isGameOver = false;
        }

        


        window.clear();

        target.draw(window);

        arrow.draw(window);

        window.draw(trajectoryLine);
        

        window.display();
    }

    return 0;
}
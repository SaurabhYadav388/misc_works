
#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1000, 600), "SFML works!");
    //window.setFramerateLimit(600);//

    ConvexShape arrow;
    arrow.setPointCount(7);

    arrow.setPoint(0, Vector2f(0, 1));
    arrow.setPoint(1, Vector2f(30, 1));
    arrow.setPoint(2, Vector2f(35, 3));
    arrow.setPoint(3, Vector2f(40, 0));
    arrow.setPoint(4, Vector2f(35, -3));
    arrow.setPoint(5, Vector2f(30, -1));
    arrow.setPoint(6, Vector2f(0, -1));
    arrow.setFillColor(Color::Green);

    
    arrow.setPosition(100, 300);
    arrow.setOrigin(Vector2f(40, 0));///////// origin as arrow head

    arrow.scale(3, 3);

    VertexArray trajectoryLine(LineStrip);


    CircleShape outerCircle;
    outerCircle.setRadius(100.f);
    outerCircle.setOrigin(100.f, 100.f); // Set origin to the center
    outerCircle.setPosition(800, 300);
    outerCircle.setFillColor(Color::White); // Change color as needed

    float initialXPos;
    bool mouseIsClicked = false;
    float angle = 0;
    float speed = 0;
    float moveTime = 0.f;
    bool startArrowProjectile = false;
    int f = 0;

    float TargMoveSp = .05f;

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
                

                speed = 0.2f * (initialXPos - Mouse::getPosition(window).x);//.2f random constant ,something relative change in pos 
                //std::cout << "angle: "<<(angle/3.1459) * 180 <<"\n";
                arrow.setRotation( (-angle) * (180.0 / 3.14159));//-ve for anticlockwise

                // Simulate the projectile path
                for (float t = 0; t <= 10.0f; t += 0.001f) {
                    float x = speed * t * std::cosf(angle);
                    float y = speed * t * std::sinf(angle) - 0.5f * 9.8f * t * t;
                    Vector2f arrowheadPosition = arrow.getTransform().transformPoint(arrow.getPoint(3));
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
        Vector2f arrowheadPosition = arrow.getTransform().transformPoint(arrow.getPoint(3));
        
        if (arrowheadPosition.x >= (outerCircle.getGlobalBounds().getPosition().x + outerCircle.getRadius())
            && arrowheadPosition.x <= outerCircle.getGlobalBounds().getPosition().x+outerCircle.getRadius()*2
            && arrowheadPosition.y >= (outerCircle.getGlobalBounds().getPosition().y + outerCircle.getRadius())
            && arrowheadPosition.y <= outerCircle.getGlobalBounds().getPosition().y + outerCircle.getRadius() * 2
            )
        {
            ;
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


            if (outerCircle.getGlobalBounds().getPosition().y <= 0 || outerCircle.getGlobalBounds().getPosition().y + outerCircle.getRadius() * 2 >= window.getSize().y)
                TargMoveSp *= -1;
            outerCircle.move(0, -TargMoveSp);

        }

        


        window.clear();

        //draw target first
        window.draw(outerCircle);

        // Draw arrow
        window.draw(arrow);

        // Draw trajectory line
        window.draw(trajectoryLine);

        

        window.display();
    }

    return 0;
}

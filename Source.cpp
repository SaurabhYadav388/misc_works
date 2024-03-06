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
    window.setFramerateLimit(60);//
    
    /*CircleShape shape(50.0f);
    shape.setFillColor(Color::Green);
    float dirX=cosf(3.14f/2.0f);
    float dirY=sinf(3.14f/2.0f);
    float velocity = 10.0f;*/

    //********Shooting projectiles in SFML / C++ in direction of mouse
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
    
    arrow.scale(3, 3);

    //game loop
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))//listen to event in window
        {
            if (event.type == Event::Closed)
                window.close();
        }
        //Update here...then later draw processs
        //shape.move(velocity*dirX,velocity*dirY);
        //arrow.rotate(10);

        //if(Mouse::getPosition(window).y>= 0 + arrow.getGlobalBounds().height && Mouse::getPosition(window).y <= window.getSize().y - arrow.getGlobalBounds().height)
            //arrow.setPosition(arrow.getPosition().x, Mouse::getPosition(window).y);
        //std::cout << "arrow pos:" << arrow.getPosition().x << " & " << arrow.getPosition().y<<"\n";
        arrow.rotate(5.f);
            
        //Clear to draw new frame
        window.clear();

        //Draw stuffs...
        //window.draw(shape);
        window.draw(arrow);

        //finished drawing
        window.display();
    }

    return 0;
}
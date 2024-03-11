#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>

#include "Game.h"

using namespace sf;

int main()
{
    
    Game game;
    game.initGame();
    game.runGame();
    return 0;
}

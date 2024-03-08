#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>

#include "Arrow.h"
#include "Target.h"
#include "TrajectoryArc.h"

class Game
{
public:
    //Game();
    void initGame();
    void GameLoop(sf::RenderWindow& window);

    void whileMouseClicked(sf::Vector2i currMousePosition);

private:
    Arrow arrow;
    Target target;
    TrajectoryArc trajectoryArc;

    float startMousePosX;

    bool mouseIsClickedFlag;
    float angle;
    float speed;
    float moveTime;
    bool startArrowProjectile;
    int projItr;
    float TargMoveSp;
    bool isGameOver;
};


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
#include "Scorecard.h"

class Game
{
public:
    void initGame();
    void runGame();
    void updateGame(sf::Vector2u windowSize);

private:
    Arrow arrow;
    Target target;
    TrajectoryArc trajectoryArc;
    Scorecard scoreCard;

    float startMousePosX;

    float arrowTimeInstance;
    float TargetMoveSpeed;

    sf::Vector2f initArrowPosition;

    bool mouseIsClicked = false;
    bool startArrowProjectile = false;
    bool toGameRestart = false;
    bool isGameOver = false;

};
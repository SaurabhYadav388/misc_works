#pragma once
#include <SFML/Graphics.hpp>

class Scorecard {
public:
    Scorecard();
    void setScore(int newScore);
    void increaseScore(int points);
    void draw(sf::RenderWindow& window);
    
private:
    int score;
    sf::Text scoreText;
    sf::Font myFont;
};


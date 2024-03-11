#include "Scorecard.h"
#include <SFML/Graphics.hpp>
#include <iostream>
Scorecard::Scorecard() {

    if (!myFont.loadFromFile("./Metropolian-Display.ttf"))
    {
        std::cout << "cant open font\n";
    }    
    scoreText.setFont(myFont);
    scoreText.setCharacterSize(20);
    scoreText.setPosition({10.f,10.f});
    scoreText.setFillColor(sf::Color::Yellow);
    setScore(0);
}

void Scorecard::setScore(int newScore) {
    score = newScore;
    scoreText.setString("Score: " + std::to_string(score));
}

void Scorecard::increaseScore(int points) {
    score += points;
    setScore(score);
}

void Scorecard::draw(sf::RenderWindow& window) {
    window.draw(scoreText);
    
}
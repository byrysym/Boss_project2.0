#pragma once

#include <SFML/Graphics.hpp>

class Menu {
public:
    enum MenuOption { Play, HighScore, Exit };

    Menu();

    void draw(sf::RenderWindow& window);
    void handleInput(const sf::Event& event, bool& isPlaying);

    MenuOption getSelectedOption() const;

private:
    sf::Font mFont;
    sf::Text mOptions[3];
    int mSelectedIndex;
};
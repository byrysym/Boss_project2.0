#include "Menu.h"

Menu::Menu() : mSelectedIndex(0) {
    mFont.loadFromFile("D:/STUp-Thomas-Noa-main/ShootThemUp/assets/Venus Rising Rg.otf");//path à modifier

    std::string optionTexts[] = { "Play", "High Score", "Exit" };
    for (int i = 0; i < 3; ++i) {
        mOptions[i].setFont(mFont);
        mOptions[i].setString(optionTexts[i]);
        mOptions[i].setCharacterSize(30);
        mOptions[i].setPosition(300, 200 + i * 50);

        if (i == mSelectedIndex) {
            mOptions[i].setFillColor(sf::Color::Red);
        }
        else {
            mOptions[i].setFillColor(sf::Color::White);
        }
    }
}

void Menu::draw(sf::RenderWindow& window) {
    for (const auto& option : mOptions) {
        window.draw(option);
    }
}

void Menu::handleInput(const sf::Event& event, bool& isPlaying) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            mOptions[mSelectedIndex].setFillColor(sf::Color::White);
            mSelectedIndex = (mSelectedIndex + 2) % 3;
            mOptions[mSelectedIndex].setFillColor(sf::Color::Red);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            mOptions[mSelectedIndex].setFillColor(sf::Color::White);
            mSelectedIndex = (mSelectedIndex + 1) % 3;
            mOptions[mSelectedIndex].setFillColor(sf::Color::Red);
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            if (mSelectedIndex == MenuOption::Play) {
                isPlaying = true;
            }
            else if (mSelectedIndex == MenuOption::Exit) {
                exit(0);
            }
        }
    }
}

Menu::MenuOption Menu::getSelectedOption() const {
    return static_cast<MenuOption>(mSelectedIndex);
}
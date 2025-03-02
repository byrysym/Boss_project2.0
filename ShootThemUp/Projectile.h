#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(const sf::Vector2f& position, float rotation);
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isOffscreen() const;
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape mShape;
    sf::Vector2f mVelocity;
    float mSpeed;
};

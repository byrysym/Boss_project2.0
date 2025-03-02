#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Projectile.h"

class EnemyShip {
public:
    EnemyShip(const sf::Vector2f& position, const sf::Vector2f& velocity);
    void update(sf::Time deltaTime, const sf::Vector2f& playerPosition);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isOffscreen() const;
    bool canShoot() const;
    void resetShootClock();
    std::vector<Projectile> shoot(const sf::Vector2f& playerPosition);

    void takeDamage(int damage);
    bool isDestroyed() const;
    int getHealth() const;

private:
    sf::ConvexShape mShape;
    sf::Vector2f mVelocity;
    sf::Clock mShootClock;
    float mShootInterval;
    float getAngleToPlayer(const sf::Vector2f& playerPosition) const;
    int mHealth;
};
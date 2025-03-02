#include "EnemyShip.h"
#include <cmath>

EnemyShip::EnemyShip(const sf::Vector2f& position, const sf::Vector2f& velocity)
    : mVelocity(velocity), mShootInterval(2.0f), mHealth(1)
{
    mShape.setPointCount(3);
    mShape.setPoint(0, sf::Vector2f(0, -15));
    mShape.setPoint(1, sf::Vector2f(-10, 15));
    mShape.setPoint(2, sf::Vector2f(10, 15));
    mShape.setFillColor(sf::Color::Red);
    mShape.setPosition(position);
}

void EnemyShip::update(sf::Time deltaTime, const sf::Vector2f& playerPosition) {
    mShape.move(mVelocity * deltaTime.asSeconds());

    float angle = getAngleToPlayer(playerPosition);
    mShape.setRotation(angle);
}

void EnemyShip::draw(sf::RenderWindow& window) const {
    window.draw(mShape);
}

bool EnemyShip::isOffscreen() const {
    sf::Vector2f pos = mShape.getPosition();
    return (pos.x < -50 || pos.x > 1550 || pos.y < -50 || pos.y > 1050);
}

sf::FloatRect EnemyShip::getBounds() const {
    return mShape.getGlobalBounds();
}

bool EnemyShip::canShoot() const {
    return mShootClock.getElapsedTime().asSeconds() >= mShootInterval;
}

void EnemyShip::resetShootClock() {
    mShootClock.restart();
}

float EnemyShip::getAngleToPlayer(const sf::Vector2f& playerPosition) const {
    sf::Vector2f pos = mShape.getPosition();
    sf::Vector2f direction = playerPosition - pos;
    return std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
}

std::vector<Projectile> EnemyShip::shoot(const sf::Vector2f& playerPosition) {
    std::vector<Projectile> projectiles;
    if (canShoot()) {
        sf::Vector2f pos = mShape.getPosition();
        float angle = getAngleToPlayer(playerPosition);
        projectiles.emplace_back(pos, angle);
        resetShootClock();
    }
    return projectiles;
}

void EnemyShip::takeDamage(int damage) {
    mHealth -= damage;
}

bool EnemyShip::isDestroyed() const {
    return mHealth <= 0;
}

int EnemyShip::getHealth() const {
    return mHealth;
}
#include "Asteroid.h"

Asteroid::Asteroid(const sf::Vector2f& position, const sf::Vector2f& velocity)
    : mVelocity(velocity), mHealth(1) 
{
    float radius = 20.0f;
    mShape.setRadius(radius);
    mShape.setFillColor(sf::Color(169, 169, 169)); 
    mShape.setPosition(position);
    mShape.setOrigin(radius, radius);
}

void Asteroid::update(sf::Time deltaTime) {
    mShape.move(mVelocity * deltaTime.asSeconds());
}

void Asteroid::draw(sf::RenderWindow& window) const {
    window.draw(mShape);
}

sf::FloatRect Asteroid::getBounds() const {
    return mShape.getGlobalBounds();
}

bool Asteroid::isOffscreen() const {
    sf::Vector2f pos = mShape.getPosition();
    return (pos.x < -50 || pos.x > 1550 || pos.y < -50 || pos.y > 1050);
}

void Asteroid::takeDamage(int damage) {
    mHealth -= damage;
}

bool Asteroid::isDestroyed() const {
    return mHealth <= 0;
}

int Asteroid::getHealth() const {
    return mHealth;
}
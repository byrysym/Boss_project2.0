#include "Projectile.h"
#include <cmath>

Projectile::Projectile(const sf::Vector2f& position, float rotation)
    : mSpeed(800.0f)
{
    mShape.setSize(sf::Vector2f(5.0f, 10.0f));
    mShape.setFillColor(sf::Color::Yellow);
    mShape.setPosition(position);
    mShape.setRotation(rotation);

    float angleRad = rotation * 3.14159f / 180.0f;
    mVelocity = sf::Vector2f(std::cos(angleRad) * mSpeed, std::sin(angleRad) * mSpeed);
}

void Projectile::update(sf::Time deltaTime) {
    mShape.move(mVelocity * deltaTime.asSeconds());
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(mShape);
}

bool Projectile::isOffscreen() const {
    sf::Vector2f pos = mShape.getPosition();
    return (pos.x < 0 || pos.x > 1500 || pos.y < 0 || pos.y > 1000);
}

sf::FloatRect Projectile::getBounds() const {
    return mShape.getGlobalBounds();
}

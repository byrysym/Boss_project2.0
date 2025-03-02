#include "Player.h"

Player::Player()
    : mVelocity(300, 0),
    mSpeed(100.0f),
    mMaxSpeed(500.0f),
    mFriction(0.98f),
    mHealth(3),
    mHasShield(false),
    mScoreMultiplierActive(false),
    mRotationSpeed(450.0f),
    mIsMovingForward(false),
    mShootInterval(0.5f) {  

   
    mShape.setPointCount(3);
    mShape.setPoint(0, sf::Vector2f(0, -20));  
    mShape.setPoint(1, sf::Vector2f(-30, 20)); 
    mShape.setPoint(2, sf::Vector2f(30, 20));  
    mShape.setFillColor(sf::Color::Green);
    mShape.setPosition(380, 280); 
}

void Player::handleInput(const sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Z) {
            mIsMovingForward = true;
        }
        if (event.key.code == sf::Keyboard::Q) {
            mShape.rotate(-mRotationSpeed * 0.016f);  
        }
        if (event.key.code == sf::Keyboard::D) {
            mShape.rotate(mRotationSpeed * 0.016f);  
        }
    }

    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Z) {
            mIsMovingForward = false;
        }
    }
}


void Player::update(sf::Time deltaTime) {
    if (mIsMovingForward) {
        float angleRad = mShape.getRotation() * 3.14159f / 180.0f;  
        mVelocity.x = cos(angleRad) * mSpeed;
        mVelocity.y = sin(angleRad) * mSpeed;

        
        if (mSpeed < mMaxSpeed) {
            mSpeed += 10.0f;  
        }
    }
    else {
        
        mVelocity *= mFriction;
        mSpeed = std::max(0.0f, mSpeed - 1.0f);  
    }

   
    mShape.move(mVelocity * deltaTime.asSeconds());

    
    sf::Vector2f pos = mShape.getPosition();
    if (pos.x < 0) mShape.setPosition(0, pos.y);
    if (pos.x + mShape.getLocalBounds().width > 1500) mShape.setPosition(1500 - mShape.getLocalBounds().width, pos.y);
    if (pos.y < 0) mShape.setPosition(pos.x, 0);
    if (pos.y + mShape.getLocalBounds().height > 1000) mShape.setPosition(pos.x, 1000 - mShape.getLocalBounds().height);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(mShape);
}

void Player::increaseSpeed() {
    mSpeed = std::min(mMaxSpeed, mSpeed + 50.0f);
}

void Player::decreaseSpeed() {
    mSpeed = std::max(0.0f, mSpeed - 50.0f);
}

void Player::activateShield() {
    mHasShield = true;
}

void Player::addHealth() {
    mHealth = std::min(5, mHealth + 1);
}

void Player::activateScoreMultiplier() {
    mScoreMultiplierActive = true;
}

sf::FloatRect Player::getBounds() const {
    return mShape.getGlobalBounds();
}

int Player::getHealth() const {
    return mHealth;
}

bool Player::canShoot() const {
    return mShootClock.getElapsedTime().asSeconds() >= mShootInterval;
}

void Player::resetShootClock() {
    mShootClock.restart();
}

void Player::setShootInterval(float interval) {
    mShootInterval = interval;
}

sf::Vector2f Player::getPosition() const {
    return mShape.getPosition();
}

float Player::getRotation() const {
    return mShape.getRotation();
}
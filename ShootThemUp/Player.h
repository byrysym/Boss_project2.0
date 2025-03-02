#pragma once

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void handleInput(const sf::Event& event);
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    void increaseSpeed();
    void decreaseSpeed();
    void activateShield();
    void addHealth();
    void activateScoreMultiplier();
    sf::FloatRect getBounds() const;
    int getHealth() const;
    bool canShoot() const;
    void resetShootClock();
    void setShootInterval(float interval);
    sf::Vector2f getPosition() const;
    float getRotation() const;

private:
    sf::ConvexShape mShape;
    sf::Vector2f mVelocity;
    float mSpeed;
    float mMaxSpeed;  
    float mFriction;  
    int mHealth;
    bool mHasShield;
    bool mScoreMultiplierActive;
    float mRotationSpeed;  
    bool mIsMovingForward; 
    sf::Clock mShootClock;    
    float mShootInterval;

};

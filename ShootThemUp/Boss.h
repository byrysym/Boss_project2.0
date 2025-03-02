#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Projectile.h"
#include "BehaviorTree.h"
#include "BossAttack.h"

class Boss {
public:
    Boss(const sf::Vector2f& position, const sf::Vector2f& velocity);
    void update(sf::Time deltaTime, const sf::Vector2f& playerPosition);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isOffscreen() const;
    std::vector<Projectile> attack(const sf::Vector2f& playerPosition);
    void takeDamage(int damage);
    bool isDestroyed() const;
    int getHealth() const;

private:
    sf::ConvexShape mShape;
    sf::Vector2f mVelocity;
    sf::Clock mAttackClock;
    int mHealth;
    enum class Phase {
        PHASE_1, 
        PHASE_2, 
        PHASE_3  
    };

    Phase mCurrentPhase;
    void updatePhase();
    std::shared_ptr<BehaviorNode> mBehaviorTree;
    void setupBehaviorTree();
    std::unique_ptr<BossAttack> mCurrentAttack;
    std::unique_ptr<BasicShot> mBasicShot;
    std::unique_ptr<TripleShot> mTripleShot;
    std::unique_ptr<CircleShot> mCircleShot;
    std::unique_ptr<RapidTrackingShot> mRapidTrackingShot;
    void updateVisuals();
};
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Projectile.h"

class BossAttack {
public:
    virtual ~BossAttack() = default;
    virtual std::vector<Projectile> execute(const sf::Vector2f& bossPosition, const sf::Vector2f& playerPosition) = 0;
    virtual bool isReady(sf::Clock& clock) = 0;
};

class BasicShot : public BossAttack {
private:
    float mCooldown;

public:
    BasicShot(float cooldown = 2.0f) : mCooldown(cooldown) {}

    std::vector<Projectile> execute(const sf::Vector2f& bossPosition, const sf::Vector2f& playerPosition) override {
        std::vector<Projectile> projectiles;

        sf::Vector2f direction = playerPosition - bossPosition;
        float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;

        projectiles.emplace_back(bossPosition, angle);

        return projectiles;
    }

    bool isReady(sf::Clock& clock) override {
        return clock.getElapsedTime().asSeconds() >= mCooldown;
    }
};

class TripleShot : public BossAttack {
private:
    float mCooldown;
    float mSpreadAngle;

public:
    TripleShot(float cooldown = 3.0f, float spreadAngle = 15.0f)
        : mCooldown(cooldown), mSpreadAngle(spreadAngle) {
    }

    std::vector<Projectile> execute(const sf::Vector2f& bossPosition, const sf::Vector2f& playerPosition) override {
        std::vector<Projectile> projectiles;

        sf::Vector2f direction = playerPosition - bossPosition;
        float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;

        projectiles.emplace_back(bossPosition, angle);
        projectiles.emplace_back(bossPosition, angle - mSpreadAngle);
        projectiles.emplace_back(bossPosition, angle + mSpreadAngle);

        return projectiles;
    }

    bool isReady(sf::Clock& clock) override {
        return clock.getElapsedTime().asSeconds() >= mCooldown;
    }
};

class CircleShot : public BossAttack {
private:
    float mCooldown;
    int mProjectileCount;

public:
    CircleShot(float cooldown = 5.0f, int projectileCount = 8)
        : mCooldown(cooldown), mProjectileCount(projectileCount) {
    }

    std::vector<Projectile> execute(const sf::Vector2f& bossPosition, const sf::Vector2f& playerPosition) override {
        std::vector<Projectile> projectiles;

        for (int i = 0; i < mProjectileCount; ++i) {
            float angle = 360.0f * i / mProjectileCount;
            projectiles.emplace_back(bossPosition, angle);
        }

        return projectiles;
    }

    bool isReady(sf::Clock& clock) override {
        return clock.getElapsedTime().asSeconds() >= mCooldown;
    }
};

class RapidTrackingShot : public BossAttack {
private:
    float mCooldown;
    int mBurstCount;
    float mBurstDelay;
    int mCurrentBurst;
    sf::Clock mBurstClock;

public:
    RapidTrackingShot(float cooldown = 4.0f, int burstCount = 3)
        : mCooldown(cooldown), mBurstCount(burstCount), mBurstDelay(0.15f), mCurrentBurst(0) {
    }

    std::vector<Projectile> execute(const sf::Vector2f& bossPosition, const sf::Vector2f& playerPosition) override {
        std::vector<Projectile> projectiles;

        sf::Vector2f direction = playerPosition - bossPosition;
        float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;

        Projectile p(bossPosition, angle);
        projectiles.push_back(p);

        mCurrentBurst++;
        mBurstClock.restart();

        return projectiles;
    }

    bool isReady(sf::Clock& clock) override {
        if (mCurrentBurst == 0) {
            return clock.getElapsedTime().asSeconds() >= mCooldown;
        }
        else if (mCurrentBurst < mBurstCount) {
            return mBurstClock.getElapsedTime().asSeconds() >= mBurstDelay;
        }
        else {
            mCurrentBurst = 0;
            return false;
        }
    }
};
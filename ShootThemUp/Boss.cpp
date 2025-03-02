#include "Boss.h"
#include <cmath>

Boss::Boss(const sf::Vector2f& position, const sf::Vector2f& velocity)
    : mVelocity(velocity), mHealth(10), mCurrentPhase(Phase::PHASE_1)
{
    mShape.setPointCount(3);
    mShape.setPoint(0, sf::Vector2f(0, -30));
    mShape.setPoint(1, sf::Vector2f(-20, 30));
    mShape.setPoint(2, sf::Vector2f(20, 30));
    mShape.setFillColor(sf::Color::Red);
    mShape.setPosition(position);
    mBasicShot = std::make_unique<BasicShot>(2.0f);
    mTripleShot = std::make_unique<TripleShot>(3.0f, 20.0f);
    mCircleShot = std::make_unique<CircleShot>(5.0f, 12);
    mRapidTrackingShot = std::make_unique<RapidTrackingShot>(4.0f, 5);
    mCurrentAttack = std::make_unique<BasicShot>(2.0f);
    setupBehaviorTree();
}

void Boss::setupBehaviorTree() {
    mBehaviorTree = std::make_shared<Selector>(
        std::vector<std::shared_ptr<BehaviorNode>>{
        std::make_shared<Sequence>(
            std::vector<std::shared_ptr<BehaviorNode>>{
            std::make_shared<Condition>([this]() {
                return mCurrentPhase == Phase::PHASE_3;
                }, "IsPhase3"),
                std::make_shared<Action>([this]() {
                static bool useRapid = true;
                if (useRapid) {
                    mCurrentAttack = std::make_unique<RapidTrackingShot>(2.5f, 5);
                }
                else {
                    mCurrentAttack = std::make_unique<CircleShot>(4.0f, 16);
                }
                useRapid = !useRapid;
                return NodeStatus::SUCCESS;
                    }, "SetPhase3Attack")
        }
        ),

            std::make_shared<Sequence>(
                std::vector<std::shared_ptr<BehaviorNode>>{
            std::make_shared<Condition>([this]() {
                return mCurrentPhase == Phase::PHASE_2;
                }, "IsPhase2"),
                std::make_shared<Action>([this]() {
                static bool useTriple = true;
                if (useTriple) {
                    mCurrentAttack = std::make_unique<TripleShot>(2.5f, 15.0f);
                }
                else {
                    mCurrentAttack = std::make_unique<CircleShot>(4.0f, 8);
                }
                useTriple = !useTriple;
                return NodeStatus::SUCCESS;
                    }, "SetPhase2Attack")
        }
            ),

            std::make_shared<Sequence>(
                std::vector<std::shared_ptr<BehaviorNode>>{
            std::make_shared<Condition>([this]() {
                return mCurrentPhase == Phase::PHASE_1;
                }, "IsPhase1"),
                std::make_shared<Action>([this]() {
                static bool useBasic = true;
                if (useBasic) {
                    mCurrentAttack = std::make_unique<BasicShot>(1.5f);
                }
                else {
                    mCurrentAttack = std::make_unique<TripleShot>(3.0f, 10.0f);
                }
                useBasic = !useBasic;
                return NodeStatus::SUCCESS;
                    }, "SetPhase1Attack")
        }
            )
    }
    );
}

void Boss::update(sf::Time deltaTime, const sf::Vector2f& playerPosition) {
    mShape.move(mVelocity * deltaTime.asSeconds());
    updatePhase();
    float angle = std::atan2(playerPosition.y - mShape.getPosition().y,
        playerPosition.x - mShape.getPosition().x) * 180.0f / 3.14159f + 90.0f;
    mShape.setRotation(angle);
    mBehaviorTree->execute();
}

void Boss::updatePhase() {
    Phase newPhase;

    if (mHealth <= 3) {
        newPhase = Phase::PHASE_3;
    }
    else if (mHealth <= 6) {
        newPhase = Phase::PHASE_2;
    }
    else {
        newPhase = Phase::PHASE_1;
    }

    if (newPhase != mCurrentPhase) {
        mCurrentPhase = newPhase;
        updateVisuals();
    }
}

void Boss::updateVisuals() {
    switch (mCurrentPhase) {
    case Phase::PHASE_1:
        mShape.setFillColor(sf::Color::Red);
        break;
    case Phase::PHASE_2:
        mShape.setFillColor(sf::Color(255, 128, 0));
        for (size_t i = 0; i < mShape.getPointCount(); ++i) {
            mShape.setPoint(i, mShape.getPoint(i) * 1.2f);
        }
        break;
    case Phase::PHASE_3:
        mShape.setFillColor(sf::Color(255, 255, 0));
        mShape.setOutlineColor(sf::Color(255, 0, 0, 128));
        mShape.setOutlineThickness(5.0f);
        break;
    }
}

void Boss::draw(sf::RenderWindow& window) const {
    window.draw(mShape);
}

bool Boss::isOffscreen() const {
    sf::Vector2f pos = mShape.getPosition();
    return (pos.x < -50 || pos.x > 1550 || pos.y < -50 || pos.y > 1050);
}

sf::FloatRect Boss::getBounds() const {
    return mShape.getGlobalBounds();
}

std::vector<Projectile> Boss::attack(const sf::Vector2f& playerPosition) {
    if (mCurrentAttack && mCurrentAttack->isReady(mAttackClock)) {
        mAttackClock.restart();
        return mCurrentAttack->execute(mShape.getPosition(), playerPosition);
    }

    return std::vector<Projectile>();
}

void Boss::takeDamage(int damage) {
    mHealth -= damage;

    float healthPercentage = static_cast<float>(mHealth) / 10.0f;
    if (mCurrentPhase == Phase::PHASE_1) {
        sf::Uint8 redValue = static_cast<sf::Uint8>(255 * healthPercentage);
        mShape.setFillColor(sf::Color(255, redValue, 0));
    }
}

bool Boss::isDestroyed() const {
    return mHealth <= 0;
}

int Boss::getHealth() const {
    return mHealth;
}
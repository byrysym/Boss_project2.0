#include "Game.h"
#include <algorithm>
#include <SFML/Audio.hpp>



Game::Game()
    : mWindow(sf::VideoMode(1500, 1000), "Asteroid Survival"),
    mIsPlaying(false),
    mIsGameOver(false),
    mIsEnteringName(false),
    mScore(0),
    mScoreThresholdForBoss(1000),
    mBossSpawnedForCurrentThreshold(false) {
    mRNG.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    if (!mFont.loadFromFile("D:/STUp-Thomas-Noa-main/ShootThemUp/assets/Venus Rising Rg.otf"))//path à modifier {
    }
}

void Game::run() {
    sf::Clock clock;

    sf::Music music;
    if (!music.openFromFile("C:/Users/assets/pvzgw2 ops special wave (version b).mp3"))//path à modifier
    { }
    music.play();
    while (mWindow.isOpen()) {
        music.setLoop(true);
        processEvents();
        sf::Time deltaTime = clock.restart();
        if (mIsPlaying) {
            update(deltaTime);
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }

        if (mIsGameOver) {
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter) {
                mIsGameOver = false;
                mIsPlaying = false;
                mScore = 0;
                mProjectiles.clear();
                mAsteroids.clear();
                mEnemyShips.clear();
                mEnemyProjectiles.clear();
                mBosses.clear();
                mScoreThresholdForBoss = 1000;
                mBossSpawnedForCurrentThreshold = false;
                mPlayer = Player();
            }
        }
        else if (!mIsPlaying && !mIsEnteringName) {
            processMenuEvents(event);
        }
        else if (mIsEnteringName) {
            processNameEntryEvents(event);
        }
        else {
            processGameplayEvents(event);
        }
    }

        if (!mIsPlaying && !mIsEnteringName) {
            processMenuEvents(event);
        }
        else if (mIsEnteringName) {
            processNameEntryEvents(event);
        }
        else {
            processGameplayEvents(event);
        }
    if (mIsPlaying && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mPlayer.canShoot()) {
        sf::Vector2f playerPos = mPlayer.getPosition();
        float playerRotation = mPlayer.getRotation();
        mProjectiles.emplace_back(playerPos, playerRotation);
        mPlayer.resetShootClock();
    }
}

void Game::processMenuEvents(const sf::Event& event) {
    mMenu.handleInput(event, mIsPlaying);
}

void Game::processGameplayEvents(const sf::Event& event) {
    mPlayer.handleInput(event);
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Space &&
        mPlayer.canShoot()) {
        
        sf::Vector2f playerPos = mPlayer.getPosition();
        float playerRotation = mPlayer.getRotation();
        mProjectiles.emplace_back(playerPos, playerRotation);
        mPlayer.resetShootClock();
    }
}

void Game::spawnAsteroid() {
    
    int side = randomInt(0, 3);
    sf::Vector2f position;
    sf::Vector2f velocity;

    switch (side) {
    case 0: 
        position = sf::Vector2f(randomFloat(0.f, 1500.f), -50.f);
        velocity = sf::Vector2f(randomFloat(-200.f, 200.f), randomFloat(100.f, 300.f));
        break;
    case 1: 
        position = sf::Vector2f(1550.f, randomFloat(0.f, 1000.f));
        velocity = sf::Vector2f(randomFloat(-300.f, -100.f), randomFloat(-200.f, 200.f));
        break;
    case 2: 
        position = sf::Vector2f(randomFloat(0.f, 1500.f), 1050.f);
        velocity = sf::Vector2f(randomFloat(-200.f, 200.f), randomFloat(-300.f, -100.f));
        break;
    case 3: 
        position = sf::Vector2f(-50.f, randomFloat(0.f, 1000.f));
        velocity = sf::Vector2f(randomFloat(100.f, 300.f), randomFloat(-200.f, 200.f));
        break;
    }

    mAsteroids.emplace_back(position, velocity);
}

void Game::spawnEnemyShip() {
    int side = randomInt(0, 3);
    sf::Vector2f position;
    sf::Vector2f velocity;

    switch (side) {
    case 0: 
        position = sf::Vector2f(randomFloat(0.f, 1500.f), -50.f);
        velocity = sf::Vector2f(randomFloat(-100.f, 100.f), randomFloat(50.f, 150.f));
        break;
    case 1:
        position = sf::Vector2f(1550.f, randomFloat(0.f, 1000.f));
        velocity = sf::Vector2f(randomFloat(-150.f, -50.f), randomFloat(-100.f, 100.f));
        break;
    case 2:
        position = sf::Vector2f(randomFloat(0.f, 1500.f), 1050.f);
        velocity = sf::Vector2f(randomFloat(-100.f, 100.f), randomFloat(-150.f, -50.f));
        break;
    case 3:
        position = sf::Vector2f(-50.f, randomFloat(0.f, 1000.f));
        velocity = sf::Vector2f(randomFloat(50.f, 150.f), randomFloat(-100.f, 100.f));
        break;
    }

    mEnemyShips.emplace_back(position, velocity);
}


void Game::spawnBoss() {
    sf::Vector2f position(randomFloat(200.f, 1300.f), -50.f);
    sf::Vector2f velocity(randomFloat(-50.f, 50.f), randomFloat(30.f, 70.f));

    mBosses.emplace_back(position, velocity);
}

void Game::checkCollisions() {
    for (auto projectileIt = mProjectiles.begin(); projectileIt != mProjectiles.end();) {
        bool projectileHit = false;

        for (auto asteroidIt = mAsteroids.begin(); asteroidIt != mAsteroids.end();) {
            if (projectileIt->getBounds().intersects(asteroidIt->getBounds())) {
                asteroidIt->takeDamage(1);

                if (asteroidIt->isDestroyed()) {
                    asteroidIt = mAsteroids.erase(asteroidIt);
                    mScore += 100;
                }
                else {
                    ++asteroidIt;
                }

                projectileHit = true;
                break;
            }
            else {
                ++asteroidIt;
            }
        }

        for (auto enemyIt = mEnemyShips.begin(); enemyIt != mEnemyShips.end();) {
            if (!projectileHit && projectileIt->getBounds().intersects(enemyIt->getBounds())) {
                enemyIt->takeDamage(1);

                if (enemyIt->isDestroyed()) {
                    enemyIt = mEnemyShips.erase(enemyIt);
                    mScore += 200;
                }
                else {
                    ++enemyIt;
                }

                projectileHit = true;
                break;
            }
            else {
                ++enemyIt;
            }
        }

        if (projectileHit) {
            projectileIt = mProjectiles.erase(projectileIt);
        }
        else {
            ++projectileIt;
        }
    }

    for (const auto& asteroid : mAsteroids) {
        if (mPlayer.getBounds().intersects(asteroid.getBounds())) {
            mIsPlaying = false;
            mIsGameOver = true;
            return;
        }
    }

    for (const auto& enemy : mEnemyShips) {
        if (mPlayer.getBounds().intersects(enemy.getBounds())) {
            mIsPlaying = false;
            mIsGameOver = true;
            return;
        }
    }

    for (const auto& projectile : mEnemyProjectiles) {
        if (projectile.getBounds().intersects(mPlayer.getBounds())) {
            mIsPlaying = false;
            mIsGameOver = true;
            return;
        }
    }
    for (auto bossIt = mBosses.begin(); bossIt != mBosses.end();) {
        bool bossHit = false;

        for (auto projectileIt = mProjectiles.begin(); projectileIt != mProjectiles.end();) {
            if (projectileIt->getBounds().intersects(bossIt->getBounds())) {
                bossIt->takeDamage(1);
                projectileIt = mProjectiles.erase(projectileIt);

                if (bossIt->isDestroyed()) {
                    bossIt = mBosses.erase(bossIt);
                    mScore += 1500;

                    if (mScore >= mScoreThresholdForBoss + mScoreIncrementForBoss) {
                        mScoreThresholdForBoss += mScoreIncrementForBoss;
                        mBossSpawnedForCurrentThreshold = false;
                    }

                    bossHit = true;
                }
                break;
            }
            else {
                ++projectileIt;
            }
        }

        if (!bossHit) {
            ++bossIt;
        }
    }

    for (const auto& boss : mBosses) {
        if (mPlayer.getBounds().intersects(boss.getBounds())) {
            mIsPlaying = false;
            mIsGameOver = true;
            return;
        }
    }
}

void Game::processNameEntryEvents(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b' && !mPlayerName.empty()) {
            mPlayerName.pop_back();
        }
        else if (event.text.unicode >= 32 && event.text.unicode <= 126) {
            mPlayerName += static_cast<char>(event.text.unicode);
        }
    }
}

void Game::update(sf::Time deltaTime) {
    mPlayer.update(deltaTime);
    for (auto& projectile : mProjectiles) {
        projectile.update(deltaTime);
    }

    for (auto& asteroid : mAsteroids) {
        asteroid.update(deltaTime);
    }

    for (auto& enemy : mEnemyShips) {
        enemy.update(deltaTime, mPlayer.getPosition());
        auto newProjectiles = enemy.shoot(mPlayer.getPosition());
        mEnemyProjectiles.insert(mEnemyProjectiles.end(),
            newProjectiles.begin(),
            newProjectiles.end());
    }

    for (auto& boss : mBosses) {
        boss.update(deltaTime, mPlayer.getPosition());
        auto newProjectiles = boss.attack(mPlayer.getPosition());
        mEnemyProjectiles.insert(mEnemyProjectiles.end(),
            newProjectiles.begin(),
            newProjectiles.end());
    }

    for (auto& projectile : mEnemyProjectiles) {
        projectile.update(deltaTime);
    }



    if (mEnemySpawnClock.getElapsedTime().asSeconds() >= 5.0f) {
        spawnEnemyShip();
        mEnemySpawnClock.restart();
    }

    if (mAsteroidSpawnClock.getElapsedTime().asSeconds() >= 2.0f) {
        spawnAsteroid();
        mAsteroidSpawnClock.restart();
    }

    if (mScore >= mScoreThresholdForBoss && !mBossSpawnedForCurrentThreshold) {
        spawnBoss();
        mBossSpawnedForCurrentThreshold = true;
    }

    mProjectiles.erase(
        std::remove_if(mProjectiles.begin(), mProjectiles.end(),
            [](const Projectile& p) { return p.isOffscreen(); }),
        mProjectiles.end());

    mAsteroids.erase(
        std::remove_if(mAsteroids.begin(), mAsteroids.end(),
            [](const Asteroid& a) { return a.isOffscreen(); }),
        mAsteroids.end());

    mEnemyShips.erase(
        std::remove_if(mEnemyShips.begin(), mEnemyShips.end(),
            [](const EnemyShip& e) { return e.isOffscreen(); }),
        mEnemyShips.end());
    mEnemyProjectiles.erase(
        std::remove_if(mEnemyProjectiles.begin(), mEnemyProjectiles.end(),
            [](const Projectile& p) { return p.isOffscreen(); }),
        mEnemyProjectiles.end());

    checkCollisions();

}



void Game::render() {
    mWindow.clear();

    if (!mIsPlaying) {
        if (mIsGameOver) {
            mGameOverText.setFont(mFont);
            mGameOverText.setString("GAME OVER");
            mGameOverText.setCharacterSize(60);
            mGameOverText.setFillColor(sf::Color::Red);
            mGameOverText.setPosition(
                (mWindow.getSize().x - mGameOverText.getGlobalBounds().width) / 2,
                mWindow.getSize().y / 2 - 100
            );

            mScoreText.setFont(mFont);
            mScoreText.setString("Score: " + std::to_string(mScore));
            mScoreText.setCharacterSize(30);
            mScoreText.setFillColor(sf::Color::White);
            mScoreText.setPosition(
                (mWindow.getSize().x - mScoreText.getGlobalBounds().width) / 2,
                mWindow.getSize().y / 2
            );

            mRestartText.setFont(mFont);
            mRestartText.setString("Press ENTER to return to menu");
            mRestartText.setCharacterSize(20);
            mRestartText.setFillColor(sf::Color::Yellow);
            mRestartText.setPosition(
                (mWindow.getSize().x - mRestartText.getGlobalBounds().width) / 2,
                mWindow.getSize().y / 2 + 100
            );
            mWindow.draw(mGameOverText);
            mWindow.draw(mScoreText);
            mWindow.draw(mRestartText);
        }
        else {
            mMenu.draw(mWindow);
        }
    }
    else {
        mPlayer.draw(mWindow);

        for (const auto& projectile : mProjectiles) {
            projectile.draw(mWindow);
        }

        for (const auto& asteroid : mAsteroids) {
            asteroid.draw(mWindow);

            for (const auto& enemy : mEnemyShips) {
                enemy.draw(mWindow);
            }
            for (const auto& projectile : mEnemyProjectiles) {
                projectile.draw(mWindow);
            }
        }
    }

    mWindow.display();
}

#include "Game.hpp"
#include <iostream>
#include "utils.hpp"

namespace tempest {

Game::Game() 
    : m_window(sf::VideoMode(800, 600), "Tempest")
    , m_state(GameState::PLAYING)
    , m_playfield(Playfield::Type::CIRCLE, 16)
    , m_player(m_playfield)
    , m_enemyManager(m_playfield)
    , m_score(0)
{
    m_window.setFramerateLimit(60);
    Utils::printMessage("Game initialized");
}

Game::~Game() {
    Utils::printMessage("Game shutdown");
}

void Game::run() {
    while (m_window.isOpen()) {
        processInput();
        
        float deltaTime = m_clock.restart().asSeconds();
        update(deltaTime);
        
        render();
    }
}

void Game::processInput() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                m_window.close();
            }
        }
    }
    
    // Continuous input handling
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_player.moveLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_player.moveRight();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        m_player.shoot();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        m_player.useSuperzapper();
    }
}

void Game::update(float deltaTime) {
    if (m_state == GameState::PLAYING) {
        m_player.update(deltaTime);
        m_enemyManager.update(deltaTime);
        m_levelManager.update(deltaTime);
        
        checkCollisions();
        
        // Check if level is complete
        if (m_enemyManager.areEnemiesCleared()) {
            m_state = GameState::LEVEL_COMPLETE;
            m_levelManager.startNextLevel();
            m_playfield = Playfield(m_levelManager.getCurrentPlayfieldType(), m_levelManager.getNumSegments());
            m_enemyManager.setSpawnRate(m_levelManager.getEnemySpawnRate());
            m_enemyManager.setEnemySpeed(m_levelManager.getEnemySpeed());
            m_state = GameState::PLAYING;
        }
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);
    
    m_playfield.draw(m_window);
    m_player.draw(m_window);
    m_enemyManager.draw(m_window);
    
    m_window.display();
}

void Game::checkCollisions() {
    // Check collisions between player shots and enemies
    const auto& enemies = m_enemyManager.getEnemies();
    const auto& shots = m_player.getShots();
    
    for (const auto& shot : shots) {
        for (auto& enemy : const_cast<std::vector<Enemy>&>(enemies)) {
            if (!enemy.isDestroyed() && !shot.isOutOfBounds()) {
                float distance = std::sqrt(
                    std::pow(shot.getPosition().x - enemy.getPosition().x, 2) +
                    std::pow(shot.getPosition().y - enemy.getPosition().y, 2)
                );
                
                if (distance < (shot.getRadius() + enemy.getRadius())) {
                    enemy.destroy();
                    const_cast<Shot&>(shot).destroy();
                    m_score += 100; // Basic scoring
                }
            }
        }
    }
    
    // Check collisions between player and enemies
    for (const auto& enemy : enemies) {
        if (!enemy.isDestroyed() && enemy.isAtEdge() && enemy.getLane() == m_player.getPosition()) {
            // Player hit by enemy
            // Handle player death or game over
        }
    }
}

} // namespace tempest

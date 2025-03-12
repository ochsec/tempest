#include "Player.hpp"
#include <cmath>

namespace tempest {

Player::Player()
    : m_playfield(nullptr)
    , m_position(0)
    , m_lives(3)
    , m_score(0)
    , m_superzapperCharges(1)
    , m_shootCooldown(0.0f)
{
    m_shape.setPointCount(3);
    m_shape.setFillColor(sf::Color::Green);
}

Player::Player(Playfield& playfield)
    : m_playfield(&playfield)
    , m_position(0)
    , m_lives(3)
    , m_score(0)
    , m_superzapperCharges(1)
    , m_shootCooldown(0.0f)
{
    m_shape.setPointCount(3);
    m_shape.setFillColor(sf::Color::Green);
}

void Player::moveLeft() {
    if (m_playfield) {
        m_position = (m_position - 1 + m_playfield->getNumSegments()) % m_playfield->getNumSegments();
    }
}

void Player::moveRight() {
    if (m_playfield) {
        m_position = (m_position + 1) % m_playfield->getNumSegments();
    }
}

void Player::shoot() {
    if (m_playfield && m_shootCooldown <= 0.0f) {
        sf::Vector2f pos = m_playfield->getPointPosition(m_position, 0.0f);
        sf::Vector2f dir = m_playfield->getLaneDirection(m_position);
        
        m_shots.emplace_back(pos, dir);
        m_shootCooldown = 0.2f; // 0.2 seconds between shots
    }
}

void Player::useSuperzapper() {
    if (m_superzapperCharges > 0) {
        // Signal to game to clear all enemies
        // This will be handled in Game::checkCollisions
        m_superzapperCharges--;
    }
}

void Player::update(float deltaTime) {
    // Update shoot cooldown
    if (m_shootCooldown > 0.0f) {
        m_shootCooldown -= deltaTime;
    }
    
    // Update shots
    for (auto it = m_shots.begin(); it != m_shots.end();) {
        it->update(deltaTime);
        if (it->isOutOfBounds() || !it->isActive()) {
            it = m_shots.erase(it);
        } else {
            ++it;
        }
    }
    
    // Update player shape
    if (m_playfield) {
        sf::Vector2f pos = m_playfield->getPointPosition(m_position, 0.0f);
        sf::Vector2f dir = m_playfield->getLaneDirection(m_position);
        
        // Calculate perpendicular direction
        sf::Vector2f perpDir(-dir.y, dir.x);
        
        // Set player shape points
        float size = 15.0f;
        m_shape.setPoint(0, pos);
        m_shape.setPoint(1, pos - dir * size + perpDir * size * 0.5f);
        m_shape.setPoint(2, pos - dir * size - perpDir * size * 0.5f);
    }
}

void Player::draw(sf::RenderWindow& window) {
    // Draw player
    window.draw(m_shape);
    
    // Draw shots
    for (auto& shot : m_shots) {
        shot.draw(window);
    }
}

int Player::getPosition() const {
    return m_position;
}

int Player::getLives() const {
    return m_lives;
}

int Player::getScore() const {
    return m_score;
}

const std::vector<Shot>& Player::getShots() const {
    return m_shots;
}

} // namespace tempest

#include "Enemy.hpp"
#include <cmath>
#include <random>

namespace tempest {

Enemy::Enemy(Type type, int lane, Playfield& playfield)
    : m_type(type)
    , m_lane(lane)
    , m_depth(1.0f) // Start at the inner edge
    , m_speed(0.0f)
    , m_playfield(&playfield)
    , m_radius(0.0f)
    , m_destroyed(false)
{
    // Set properties based on enemy type
    switch (m_type) {
        case Type::FLIPPER:
            m_speed = 0.15f;
            m_radius = 8.0f;
            m_shape.setFillColor(sf::Color::Red);
            break;
            
        case Type::TANKER:
            m_speed = 0.1f;
            m_radius = 12.0f;
            m_shape.setFillColor(sf::Color::Magenta);
            break;
            
        case Type::SPIKER:
            m_speed = 0.12f;
            m_radius = 10.0f;
            m_shape.setFillColor(sf::Color::Cyan);
            break;
            
        case Type::FUSEBALL:
            m_speed = 0.2f;
            m_radius = 6.0f;
            m_shape.setFillColor(sf::Color(255, 165, 0)); // Orange
            break;
            
        case Type::PULSAR:
            m_speed = 0.08f;
            m_radius = 10.0f;
            m_shape.setFillColor(sf::Color::Green);
            break;
            
        default:
            m_speed = 0.1f;
            m_radius = 8.0f;
            m_shape.setFillColor(sf::Color::Red);
            break;
    }
    
    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);
    
    updatePosition();
}

void Enemy::update(float deltaTime) {
    if (!m_destroyed) {
        // Move toward the outer edge
        m_depth -= m_speed * deltaTime;
        
        // Clamp depth between 0 and 1
        m_depth = std::max(0.0f, std::min(1.0f, m_depth));
        
        // Update position
        updatePosition();
        
        // Special behavior based on enemy type
        switch (m_type) {
            case Type::FLIPPER:
                // Occasionally change lanes
                if (rand() % 100 < 1) {
                    m_lane = (m_lane + (rand() % 2 == 0 ? 1 : -1)) % m_playfield->getNumSegments();
                    if (m_lane < 0) m_lane += m_playfield->getNumSegments();
                }
                break;
                
            case Type::FUSEBALL:
                // Move faster and bounce between lanes
                if (rand() % 100 < 5) {
                    m_lane = (m_lane + (rand() % 3 - 1)) % m_playfield->getNumSegments();
                    if (m_lane < 0) m_lane += m_playfield->getNumSegments();
                }
                break;
                
            default:
                break;
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!m_destroyed) {
        window.draw(m_shape);
    }
}

bool Enemy::isAtEdge() const {
    return m_depth <= 0.05f;
}

bool Enemy::isDestroyed() const {
    return m_destroyed;
}

const sf::Vector2f& Enemy::getPosition() const {
    return m_position;
}

float Enemy::getRadius() const {
    return m_radius;
}

int Enemy::getLane() const {
    return m_lane;
}

Enemy::Type Enemy::getType() const {
    return m_type;
}

void Enemy::destroy() {
    m_destroyed = true;
}

void Enemy::updatePosition() {
    if (m_playfield) {
        m_position = m_playfield->getPointPosition(m_lane, m_depth);
        m_shape.setPosition(m_position);
    }
}

} // namespace tempest

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
    , m_rotationAngle(0.0f)
    , m_pulseTimer(0.0f)
    , m_pulseState(false)
{
    // Set properties based on enemy type
    switch (m_type) {
        case Type::FLIPPER:
            m_speed = 0.15f;
            m_radius = 10.0f;
            createFlipperShape();
            break;
            
        case Type::TANKER:
            m_speed = 0.1f;
            m_radius = 12.0f;
            createTankerShape();
            break;
            
        case Type::SPIKER:
            m_speed = 0.12f;
            m_radius = 10.0f;
            createSpikerShape();
            break;
            
        case Type::FUSEBALL:
            m_speed = 0.2f;
            m_radius = 8.0f;
            createFuseballShape();
            break;
            
        case Type::PULSAR:
            m_speed = 0.08f;
            m_radius = 10.0f;
            createPulsarShape();
            break;
            
        default:
            m_speed = 0.1f;
            m_radius = 8.0f;
            createFlipperShape();
            break;
    }
    
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
                // Rotate the flipper
                m_rotationAngle += 180.0f * deltaTime; // 180 degrees per second
                if (m_rotationAngle >= 360.0f) {
                    m_rotationAngle -= 360.0f;
                }
                
                // Occasionally change lanes
                if (rand() % 100 < 1) {
                    m_lane = (m_lane + (rand() % 2 == 0 ? 1 : -1)) % m_playfield->getNumSegments();
                    if (m_lane < 0) m_lane += m_playfield->getNumSegments();
                }
                break;
                
            case Type::FUSEBALL:
                // Rotate the fuseball
                m_rotationAngle += 360.0f * deltaTime; // 360 degrees per second
                if (m_rotationAngle >= 360.0f) {
                    m_rotationAngle -= 360.0f;
                }
                
                // Move faster and bounce between lanes
                if (rand() % 100 < 5) {
                    m_lane = (m_lane + (rand() % 3 - 1)) % m_playfield->getNumSegments();
                    if (m_lane < 0) m_lane += m_playfield->getNumSegments();
                }
                break;
                
            case Type::PULSAR:
                // Update pulse effect
                m_pulseTimer += deltaTime;
                if (m_pulseTimer >= 0.5f) { // Pulse every 0.5 seconds
                    m_pulseTimer = 0.0f;
                    m_pulseState = !m_pulseState;
                    
                    // Update color based on pulse state
                    if (m_pulseState) {
                        for (auto& shape : m_shapes) {
                            shape->setFillColor(sf::Color::Yellow);
                        }
                    } else {
                        for (auto& shape : m_shapes) {
                            shape->setFillColor(sf::Color::Green);
                        }
                    }
                }
                break;
                
            default:
                break;
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!m_destroyed) {
        for (auto& shape : m_shapes) {
            window.draw(*shape);
        }
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
        
        // Update all shapes' positions
        for (auto& shape : m_shapes) {
            shape->setPosition(m_position);
            
            // Apply rotation for certain enemy types
            if (m_type == Type::FLIPPER || m_type == Type::FUSEBALL) {
                shape->setRotation(m_rotationAngle);
            }
        }
    }
}

void Enemy::createFlipperShape() {
    // Clear any existing shapes
    m_shapes.clear();
    
    // Create bow-tie/chevron shape
    auto flipper = std::make_unique<sf::ConvexShape>();
    flipper->setPointCount(4);
    flipper->setPoint(0, sf::Vector2f(-10, -5));
    flipper->setPoint(1, sf::Vector2f(0, 0));
    flipper->setPoint(2, sf::Vector2f(-10, 5));
    flipper->setPoint(3, sf::Vector2f(-5, 0));
    flipper->setFillColor(sf::Color::Red);
    flipper->setOrigin(0, 0);
    
    m_shapes.push_back(std::move(flipper));
}

void Enemy::createTankerShape() {
    // Clear any existing shapes
    m_shapes.clear();
    
    // Create diamond/rhomboid shape
    auto tanker = std::make_unique<sf::ConvexShape>();
    tanker->setPointCount(4);
    tanker->setPoint(0, sf::Vector2f(0, -12));
    tanker->setPoint(1, sf::Vector2f(12, 0));
    tanker->setPoint(2, sf::Vector2f(0, 12));
    tanker->setPoint(3, sf::Vector2f(-12, 0));
    tanker->setFillColor(sf::Color::Magenta);
    tanker->setOrigin(0, 0);
    
    m_shapes.push_back(std::move(tanker));
}

void Enemy::createSpikerShape() {
    // Clear any existing shapes
    m_shapes.clear();
    
    // Create spiral shape with spikes
    auto spiker = std::make_unique<sf::ConvexShape>();
    spiker->setPointCount(8);
    
    // Create a star-like shape for the spiker
    for (int i = 0; i < 8; i++) {
        float angle = i * M_PI / 4.0f;
        float radius = (i % 2 == 0) ? 10.0f : 5.0f;
        spiker->setPoint(i, sf::Vector2f(
            radius * std::cos(angle),
            radius * std::sin(angle)
        ));
    }
    
    spiker->setFillColor(sf::Color::Cyan);
    spiker->setOrigin(0, 0);
    
    m_shapes.push_back(std::move(spiker));
    
    // Add a trail of spikes
    for (int i = 1; i <= 3; i++) {
        auto spike = std::make_unique<sf::CircleShape>(2.0f);
        spike->setFillColor(sf::Color::Cyan);
        spike->setOrigin(2.0f, 2.0f);
        m_shapes.push_back(std::move(spike));
    }
}

void Enemy::createFuseballShape() {
    // Clear any existing shapes
    m_shapes.clear();
    
    // Create central sphere
    auto center = std::make_unique<sf::CircleShape>(8.0f);
    center->setFillColor(sf::Color(255, 165, 0)); // Orange
    center->setOrigin(8.0f, 8.0f);
    
    m_shapes.push_back(std::move(center));
    
    // Add tendrils
    for (int i = 0; i < 8; i++) {
        auto tendril = std::make_unique<sf::RectangleShape>(sf::Vector2f(8.0f, 1.0f));
        tendril->setFillColor(sf::Color(255, 165, 0)); // Orange
        tendril->setOrigin(0, 0.5f);
        tendril->setRotation(i * 45.0f);
        
        m_shapes.push_back(std::move(tendril));
    }
}

void Enemy::createPulsarShape() {
    // Clear any existing shapes
    m_shapes.clear();
    
    // Create wavy line shape
    auto pulsar = std::make_unique<sf::ConvexShape>();
    pulsar->setPointCount(8);
    
    // Create a wavy circle
    for (int i = 0; i < 8; i++) {
        float angle = i * M_PI / 4.0f;
        float radius = 10.0f + ((i % 2 == 0) ? 2.0f : -2.0f);
        pulsar->setPoint(i, sf::Vector2f(
            radius * std::cos(angle),
            radius * std::sin(angle)
        ));
    }
    
    pulsar->setFillColor(sf::Color::Green);
    pulsar->setOrigin(0, 0);
    
    m_shapes.push_back(std::move(pulsar));
}

} // namespace tempest

#include "Shot.hpp"

namespace tempest {

Shot::Shot(const sf::Vector2f& startPos, const sf::Vector2f& direction)
    : m_position(startPos)
    , m_velocity(direction * 500.0f) // Speed of 500 pixels per second
    , m_radius(3.0f)
    , m_active(true)
{
    m_shape.setRadius(m_radius);
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setPosition(m_position - sf::Vector2f(m_radius, m_radius));
}

void Shot::update(float deltaTime) {
    if (m_active) {
        m_position += m_velocity * deltaTime;
        m_shape.setPosition(m_position - sf::Vector2f(m_radius, m_radius));
    }
}

void Shot::draw(sf::RenderWindow& window) {
    if (m_active) {
        window.draw(m_shape);
    }
}

bool Shot::isOutOfBounds() const {
    // Check if shot is outside the play area
    // For simplicity, we'll use a fixed boundary
    return m_position.x < 0 || m_position.x > 800 || 
           m_position.y < 0 || m_position.y > 600;
}

const sf::Vector2f& Shot::getPosition() const {
    return m_position;
}

float Shot::getRadius() const {
    return m_radius;
}

void Shot::destroy() {
    m_active = false;
}

bool Shot::isActive() const {
    return m_active;
}

} // namespace tempest

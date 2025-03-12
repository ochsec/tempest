#ifndef TEMPEST_SHOT_HPP
#define TEMPEST_SHOT_HPP

#include <SFML/Graphics.hpp>

namespace tempest {

class Shot {
public:
    Shot(const sf::Vector2f& startPos, const sf::Vector2f& direction);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    bool isOutOfBounds() const;
    const sf::Vector2f& getPosition() const;
    float getRadius() const;
    void destroy();
    bool isActive() const;
    
private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_radius;
    bool m_active;
    sf::CircleShape m_shape;
};

} // namespace tempest

#endif // TEMPEST_SHOT_HPP

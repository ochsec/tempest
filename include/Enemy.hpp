#ifndef TEMPEST_ENEMY_HPP
#define TEMPEST_ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Playfield.hpp"

namespace tempest {

class Enemy {
public:
    enum class Type {
        FLIPPER,
        TANKER,
        SPIKER,
        FUSEBALL,
        PULSAR
    };
    
    Enemy(Type type, int lane, Playfield& playfield);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    bool isAtEdge() const;
    bool isDestroyed() const;
    const sf::Vector2f& getPosition() const;
    float getRadius() const;
    int getLane() const;
    Type getType() const;
    void destroy();
    
private:
    // Enemy properties
    Type m_type;
    int m_lane;
    float m_depth;
    float m_speed;
    Playfield* m_playfield;
    sf::Vector2f m_position;
    float m_radius;
    bool m_destroyed;
    
    // Vector graphics shapes
    std::vector<std::unique_ptr<sf::Shape>> m_shapes;
    
    // Animation properties
    float m_rotationAngle;
    float m_pulseTimer;
    bool m_pulseState;
    
    // Helper methods
    void updatePosition();
    void createFlipperShape();
    void createTankerShape();
    void createSpikerShape();
    void createFuseballShape();
    void createPulsarShape();
};

} // namespace tempest

#endif // TEMPEST_ENEMY_HPP

#ifndef TEMPEST_PLAYFIELD_HPP
#define TEMPEST_PLAYFIELD_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace tempest {

class Playfield {
public:
    enum class Type {
        CIRCLE,
        SQUARE,
        HEXAGON,
        OCTAGON,
        PLUS,
        STAR,
        TRIANGLE
    };
    
    Playfield();
    Playfield(Type type, int numSegments);
    
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPointPosition(int segment, float depth) const;
    sf::Vector2f getLaneDirection(int segment) const;
    int getNumSegments() const;
    
private:
    void generateShape();
    
    Type m_type;
    int m_numSegments;
    sf::Vector2f m_center;
    std::vector<sf::VertexArray> m_lanes;
    float m_outerRadius;
    float m_innerRadius;
};

} // namespace tempest

#endif // TEMPEST_PLAYFIELD_HPP

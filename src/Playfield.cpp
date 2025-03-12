#include "Playfield.hpp"
#include <cmath>

namespace tempest {

Playfield::Playfield() 
    : m_type(Type::CIRCLE)
    , m_numSegments(16)
    , m_center(400.0f, 300.0f)
    , m_outerRadius(250.0f)
    , m_innerRadius(50.0f)
{
    generateShape();
}

Playfield::Playfield(Type type, int numSegments)
    : m_type(type)
    , m_numSegments(numSegments)
    , m_center(400.0f, 300.0f)
    , m_outerRadius(250.0f)
    , m_innerRadius(50.0f)
{
    generateShape();
}

void Playfield::draw(sf::RenderWindow& window) {
    for (const auto& lane : m_lanes) {
        window.draw(lane);
    }
}

sf::Vector2f Playfield::getPointPosition(int segment, float depth) const {
    // Ensure segment is within bounds
    segment = segment % m_numSegments;
    if (segment < 0) segment += m_numSegments;
    
    // Clamp depth between 0 (outer edge) and 1 (inner edge)
    depth = std::max(0.0f, std::min(1.0f, depth));
    
    float radius = m_outerRadius - (m_outerRadius - m_innerRadius) * depth;
    float angle = 0.0f;
    
    switch (m_type) {
        case Type::CIRCLE:
            angle = 2.0f * M_PI * segment / m_numSegments;
            return sf::Vector2f(
                m_center.x + radius * std::cos(angle),
                m_center.y + radius * std::sin(angle)
            );
            
        case Type::SQUARE:
            {
                int side = segment / (m_numSegments / 4);
                float sidePos = (segment % (m_numSegments / 4)) / static_cast<float>(m_numSegments / 4);
                
                switch (side) {
                    case 0: // Top
                        return sf::Vector2f(
                            m_center.x - radius + 2 * radius * sidePos,
                            m_center.y - radius
                        );
                    case 1: // Right
                        return sf::Vector2f(
                            m_center.x + radius,
                            m_center.y - radius + 2 * radius * sidePos
                        );
                    case 2: // Bottom
                        return sf::Vector2f(
                            m_center.x + radius - 2 * radius * sidePos,
                            m_center.y + radius
                        );
                    case 3: // Left
                        return sf::Vector2f(
                            m_center.x - radius,
                            m_center.y + radius - 2 * radius * sidePos
                        );
                    default:
                        return m_center;
                }
            }
            
        case Type::HEXAGON:
            angle = 2.0f * M_PI * segment / m_numSegments;
            return sf::Vector2f(
                m_center.x + radius * std::cos(angle),
                m_center.y + radius * std::sin(angle)
            );
            
        case Type::OCTAGON:
            angle = 2.0f * M_PI * segment / m_numSegments;
            return sf::Vector2f(
                m_center.x + radius * std::cos(angle),
                m_center.y + radius * std::sin(angle)
            );
            
        case Type::PLUS:
            // Implement plus shape
            angle = 2.0f * M_PI * segment / m_numSegments;
            return sf::Vector2f(
                m_center.x + radius * std::cos(angle),
                m_center.y + radius * std::sin(angle)
            );
            
        case Type::STAR:
            // Implement star shape
            angle = 2.0f * M_PI * segment / m_numSegments;
            return sf::Vector2f(
                m_center.x + radius * std::cos(angle),
                m_center.y + radius * std::sin(angle)
            );
            
        case Type::TRIANGLE:
            angle = 2.0f * M_PI * segment / m_numSegments;
            return sf::Vector2f(
                m_center.x + radius * std::cos(angle),
                m_center.y + radius * std::sin(angle)
            );
            
        default:
            angle = 2.0f * M_PI * segment / m_numSegments;
            return sf::Vector2f(
                m_center.x + radius * std::cos(angle),
                m_center.y + radius * std::sin(angle)
            );
    }
}

sf::Vector2f Playfield::getLaneDirection(int segment) const {
    sf::Vector2f outer = getPointPosition(segment, 0.0f);
    sf::Vector2f inner = getPointPosition(segment, 1.0f);
    
    sf::Vector2f direction = inner - outer;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0) {
        direction /= length;
    }
    
    return direction;
}

int Playfield::getNumSegments() const {
    return m_numSegments;
}

void Playfield::generateShape() {
    m_lanes.clear();
    
    // Create lanes connecting outer and inner points
    for (int i = 0; i < m_numSegments; ++i) {
        sf::VertexArray lane(sf::Lines, 2);
        
        sf::Vector2f outerPoint = getPointPosition(i, 0.0f);
        sf::Vector2f innerPoint = getPointPosition(i, 1.0f);
        
        lane[0].position = outerPoint;
        lane[0].color = sf::Color::Blue;
        
        lane[1].position = innerPoint;
        lane[1].color = sf::Color::Blue;
        
        m_lanes.push_back(lane);
    }
    
    // Create outer edge
    sf::VertexArray outerEdge(sf::LineStrip, m_numSegments + 1);
    for (int i = 0; i <= m_numSegments; ++i) {
        outerEdge[i].position = getPointPosition(i % m_numSegments, 0.0f);
        outerEdge[i].color = sf::Color::Blue;
    }
    m_lanes.push_back(outerEdge);
    
    // Create inner edge
    sf::VertexArray innerEdge(sf::LineStrip, m_numSegments + 1);
    for (int i = 0; i <= m_numSegments; ++i) {
        innerEdge[i].position = getPointPosition(i % m_numSegments, 1.0f);
        innerEdge[i].color = sf::Color::Blue;
    }
    m_lanes.push_back(innerEdge);
}

} // namespace tempest

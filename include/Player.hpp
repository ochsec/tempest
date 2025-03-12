#ifndef TEMPEST_PLAYER_HPP
#define TEMPEST_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Playfield.hpp"
#include "Shot.hpp"

namespace tempest {

class Player {
public:
    Player();
    Player(Playfield& playfield);
    
    void moveLeft();
    void moveRight();
    void shoot();
    void useSuperzapper();
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    int getPosition() const;
    int getLives() const;
    int getScore() const;
    const std::vector<Shot>& getShots() const;
    
private:
    Playfield* m_playfield;
    int m_position;
    int m_lives;
    int m_score;
    int m_superzapperCharges;
    float m_shootCooldown;
    std::vector<Shot> m_shots;
    sf::ConvexShape m_shape;
};

} // namespace tempest

#endif // TEMPEST_PLAYER_HPP

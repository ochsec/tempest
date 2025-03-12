#ifndef TEMPEST_LEVEL_HPP
#define TEMPEST_LEVEL_HPP

#include "Playfield.hpp"

namespace tempest {

class Level {
public:
    Level(int levelNumber, Playfield::Type playfieldType, int numSegments);
    
    int getLevelNumber() const;
    Playfield::Type getPlayfieldType() const;
    int getNumSegments() const;
    float getEnemySpawnRate() const;
    float getEnemySpeed() const;
    
private:
    int m_levelNumber;
    Playfield::Type m_playfieldType;
    int m_numSegments;
    float m_enemySpawnRate;
    float m_enemySpeed;
};

} // namespace tempest

#endif // TEMPEST_LEVEL_HPP

#include "Level.hpp"

namespace tempest {

Level::Level(int levelNumber, Playfield::Type playfieldType, int numSegments)
    : m_levelNumber(levelNumber)
    , m_playfieldType(playfieldType)
    , m_numSegments(numSegments)
{
    // Calculate enemy spawn rate and speed based on level number
    m_enemySpawnRate = 0.5f + (levelNumber * 0.1f);
    m_enemySpeed = 1.0f + (levelNumber * 0.05f);
}

int Level::getLevelNumber() const {
    return m_levelNumber;
}

Playfield::Type Level::getPlayfieldType() const {
    return m_playfieldType;
}

int Level::getNumSegments() const {
    return m_numSegments;
}

float Level::getEnemySpawnRate() const {
    return m_enemySpawnRate;
}

float Level::getEnemySpeed() const {
    return m_enemySpeed;
}

} // namespace tempest

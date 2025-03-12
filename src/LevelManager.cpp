#include "LevelManager.hpp"

namespace tempest {

LevelManager::LevelManager()
    : m_currentLevelIndex(0)
{
    initializeLevels();
}

void LevelManager::startNextLevel() {
    m_currentLevelIndex = (m_currentLevelIndex + 1) % m_levels.size();
}

void LevelManager::update(float deltaTime) {
    // Nothing to update in the level manager for now
}

Playfield::Type LevelManager::getCurrentPlayfieldType() const {
    return m_levels[m_currentLevelIndex].getPlayfieldType();
}

int LevelManager::getCurrentLevelNumber() const {
    return m_levels[m_currentLevelIndex].getLevelNumber();
}

float LevelManager::getEnemySpawnRate() const {
    return m_levels[m_currentLevelIndex].getEnemySpawnRate();
}

float LevelManager::getEnemySpeed() const {
    return m_levels[m_currentLevelIndex].getEnemySpeed();
}

int LevelManager::getNumSegments() const {
    return m_levels[m_currentLevelIndex].getNumSegments();
}

void LevelManager::initializeLevels() {
    // Create a variety of levels with different playfield shapes
    m_levels.emplace_back(1, Playfield::Type::CIRCLE, 16);
    m_levels.emplace_back(2, Playfield::Type::SQUARE, 16);
    m_levels.emplace_back(3, Playfield::Type::HEXAGON, 12);
    m_levels.emplace_back(4, Playfield::Type::OCTAGON, 16);
    m_levels.emplace_back(5, Playfield::Type::PLUS, 20);
    m_levels.emplace_back(6, Playfield::Type::STAR, 16);
    m_levels.emplace_back(7, Playfield::Type::TRIANGLE, 12);
    m_levels.emplace_back(8, Playfield::Type::CIRCLE, 24);
    m_levels.emplace_back(9, Playfield::Type::SQUARE, 24);
    m_levels.emplace_back(10, Playfield::Type::HEXAGON, 18);
    m_levels.emplace_back(11, Playfield::Type::OCTAGON, 24);
    m_levels.emplace_back(12, Playfield::Type::PLUS, 28);
    m_levels.emplace_back(13, Playfield::Type::STAR, 20);
    m_levels.emplace_back(14, Playfield::Type::TRIANGLE, 18);
    m_levels.emplace_back(15, Playfield::Type::CIRCLE, 32);
    m_levels.emplace_back(16, Playfield::Type::SQUARE, 32);
}

} // namespace tempest

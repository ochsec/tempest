#ifndef TEMPEST_LEVEL_MANAGER_HPP
#define TEMPEST_LEVEL_MANAGER_HPP

#include <vector>
#include "Level.hpp"
#include "Playfield.hpp"

namespace tempest {

class LevelManager {
public:
    LevelManager();
    
    void startNextLevel();
    void update(float deltaTime);
    
    Playfield::Type getCurrentPlayfieldType() const;
    int getCurrentLevelNumber() const;
    float getEnemySpawnRate() const;
    float getEnemySpeed() const;
    int getNumSegments() const;
    
private:
    void initializeLevels();
    
    std::vector<Level> m_levels;
    int m_currentLevelIndex;
};

} // namespace tempest

#endif // TEMPEST_LEVEL_MANAGER_HPP

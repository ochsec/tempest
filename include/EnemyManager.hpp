#ifndef TEMPEST_ENEMY_MANAGER_HPP
#define TEMPEST_ENEMY_MANAGER_HPP

#include <vector>
#include "Enemy.hpp"
#include "Playfield.hpp"

namespace tempest {

class EnemyManager {
public:
    EnemyManager();
    EnemyManager(Playfield& playfield);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    void spawnEnemy(Enemy::Type type, int lane);
    void clearAllEnemies();
    bool areEnemiesCleared() const;
    
    const std::vector<Enemy>& getEnemies() const;
    
    void setSpawnRate(float spawnRate);
    void setEnemySpeed(float speed);
    
private:
    std::vector<Enemy> m_enemies;
    Playfield* m_playfield;
    float m_spawnTimer;
    float m_spawnRate;
    float m_enemySpeed;
};

} // namespace tempest

#endif // TEMPEST_ENEMY_MANAGER_HPP

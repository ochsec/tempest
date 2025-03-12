#include "EnemyManager.hpp"
#include <cstdlib>
#include <ctime>

namespace tempest {

EnemyManager::EnemyManager()
    : m_playfield(nullptr)
    , m_spawnTimer(0.0f)
    , m_spawnRate(0.5f)  // Enemies per second
    , m_enemySpeed(1.0f)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

EnemyManager::EnemyManager(Playfield& playfield)
    : m_playfield(&playfield)
    , m_spawnTimer(0.0f)
    , m_spawnRate(0.5f)  // Enemies per second
    , m_enemySpeed(1.0f)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void EnemyManager::update(float deltaTime) {
    // Update existing enemies
    for (auto it = m_enemies.begin(); it != m_enemies.end();) {
        it->update(deltaTime);
        
        // Remove destroyed enemies
        if (it->isDestroyed()) {
            it = m_enemies.erase(it);
        } else {
            ++it;
        }
    }
    
    // Spawn new enemies
    if (m_playfield) {
        m_spawnTimer += deltaTime;
        
        if (m_spawnTimer >= 1.0f / m_spawnRate) {
            // Randomly select enemy type
            Enemy::Type type = static_cast<Enemy::Type>(std::rand() % 5);
            
            // Randomly select lane
            int lane = std::rand() % m_playfield->getNumSegments();
            
            spawnEnemy(type, lane);
            m_spawnTimer = 0.0f;
        }
    }
}

void EnemyManager::draw(sf::RenderWindow& window) {
    for (auto& enemy : m_enemies) {
        enemy.draw(window);
    }
}

void EnemyManager::spawnEnemy(Enemy::Type type, int lane) {
    if (m_playfield) {
        m_enemies.emplace_back(type, lane, *m_playfield);
    }
}

void EnemyManager::clearAllEnemies() {
    m_enemies.clear();
}

bool EnemyManager::areEnemiesCleared() const {
    return m_enemies.empty();
}

const std::vector<Enemy>& EnemyManager::getEnemies() const {
    return m_enemies;
}

void EnemyManager::setSpawnRate(float spawnRate) {
    m_spawnRate = spawnRate;
}

void EnemyManager::setEnemySpeed(float speed) {
    m_enemySpeed = speed;
}

} // namespace tempest

#ifndef TEMPEST_GAME_HPP
#define TEMPEST_GAME_HPP

#include <SFML/Graphics.hpp>
#include "Playfield.hpp"
#include "Player.hpp"
#include "EnemyManager.hpp"
#include "LevelManager.hpp"

namespace tempest {

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETE
};

class Game {
public:
    Game();
    ~Game();
    
    void run();
    
private:
    void processInput();
    void update(float deltaTime);
    void render();
    void checkCollisions();
    
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    GameState m_state;
    
    Playfield m_playfield;
    Player m_player;
    EnemyManager m_enemyManager;
    LevelManager m_levelManager;
    
    int m_score;
};

} // namespace tempest

#endif // TEMPEST_GAME_HPP

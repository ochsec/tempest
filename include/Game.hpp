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
    
    // Menu and UI methods
    void renderMenu();
    void renderGame();
    void renderGameOver();
    void renderLevelComplete();
    void updateScoreText();
    void drawTempestLogo();
    
    // Game state management
    void startGame();
    void startNextLevel();
    void playerHit();
    
    // High score management
    void loadHighScore();
    void saveHighScore();
    
    // Window and rendering
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    sf::Font m_font;
    
    // Game state
    GameState m_state;
    int m_score;
    int m_highScore;
    int m_level;
    int m_lives;
    
    // Game objects
    Playfield m_playfield;
    Player m_player;
    EnemyManager m_enemyManager;
    LevelManager m_levelManager;
    
    // UI elements
    sf::Text m_titleText;
    sf::Text m_instructionText;
    sf::Text m_controlsText;
    sf::Text m_scoreText;
    sf::Text m_highScoreText;
    sf::Text m_levelText;
    sf::Text m_livesText;
    sf::Text m_gameOverText;
};

} // namespace tempest

#endif // TEMPEST_GAME_HPP

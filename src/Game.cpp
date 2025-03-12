#include "Game.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include "utils.hpp"

namespace tempest {

Game::Game() 
    : m_window(sf::VideoMode(800, 600), "Tempest")
    , m_state(GameState::MENU)
    , m_playfield(Playfield::Type::CIRCLE, 16)
    , m_player(m_playfield)
    , m_enemyManager(m_playfield)
    , m_score(0)
    , m_highScore(0)
    , m_level(1)
    , m_lives(3)
{
    m_window.setFramerateLimit(60);
    
    // Load font
    if (!m_font.loadFromFile("assets/fonts/arcade.ttf")) {
        // If font loading fails, use a system font as fallback
        Utils::printMessage("Failed to load arcade font, using system font");
        
        // Try to load a system font as fallback
        #ifdef _WIN32
        if (!m_font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            Utils::printMessage("Failed to load system font");
        }
        #elif defined(__APPLE__)
        if (!m_font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            Utils::printMessage("Failed to load system font");
        }
        #else
        if (!m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            Utils::printMessage("Failed to load system font");
        }
        #endif
    }
    
    // Initialize text elements
    m_titleText.setFont(m_font);
    m_titleText.setString("TEMPEST");
    m_titleText.setCharacterSize(72);
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setStyle(sf::Text::Bold);
    m_titleText.setPosition(
        (m_window.getSize().x - m_titleText.getLocalBounds().width) / 2.0f,
        100.0f
    );
    
    m_instructionText.setFont(m_font);
    m_instructionText.setString("PRESS ENTER TO START");
    m_instructionText.setCharacterSize(24);
    m_instructionText.setFillColor(sf::Color::White);
    m_instructionText.setPosition(
        (m_window.getSize().x - m_instructionText.getLocalBounds().width) / 2.0f,
        300.0f
    );
    
    m_controlsText.setFont(m_font);
    m_controlsText.setString(
        "CONTROLS:\n"
        "LEFT/RIGHT: MOVE\n"
        "SPACE: SHOOT\n"
        "Z: SUPERZAPPER\n"
        "ESC: QUIT"
    );
    m_controlsText.setCharacterSize(16);
    m_controlsText.setFillColor(sf::Color::Cyan);
    m_controlsText.setPosition(
        (m_window.getSize().x - m_controlsText.getLocalBounds().width) / 2.0f,
        400.0f
    );
    
    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(20);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition(20.0f, 20.0f);
    
    m_highScoreText.setFont(m_font);
    m_highScoreText.setCharacterSize(20);
    m_highScoreText.setFillColor(sf::Color::Yellow);
    m_highScoreText.setPosition(
        m_window.getSize().x - 200.0f,
        20.0f
    );
    
    m_levelText.setFont(m_font);
    m_levelText.setCharacterSize(20);
    m_levelText.setFillColor(sf::Color::Green);
    m_levelText.setPosition(
        (m_window.getSize().x - 100.0f) / 2.0f,
        20.0f
    );
    
    m_livesText.setFont(m_font);
    m_livesText.setCharacterSize(20);
    m_livesText.setFillColor(sf::Color::Red);
    m_livesText.setPosition(20.0f, m_window.getSize().y - 40.0f);
    
    m_gameOverText.setFont(m_font);
    m_gameOverText.setString("GAME OVER");
    m_gameOverText.setCharacterSize(72);
    m_gameOverText.setFillColor(sf::Color::Red);
    m_gameOverText.setPosition(
        (m_window.getSize().x - m_gameOverText.getLocalBounds().width) / 2.0f,
        200.0f
    );
    
    // Load high score if available
    loadHighScore();
    
    Utils::printMessage("Game initialized");
}

Game::~Game() {
    // Save high score before shutting down
    saveHighScore();
    Utils::printMessage("Game shutdown");
}

void Game::run() {
    while (m_window.isOpen()) {
        processInput();
        
        float deltaTime = m_clock.restart().asSeconds();
        update(deltaTime);
        
        render();
    }
}

void Game::processInput() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                m_window.close();
            }
            
            // Handle state-specific key presses
            switch (m_state) {
                case GameState::MENU:
                    if (event.key.code == sf::Keyboard::Return) {
                        startGame();
                    }
                    break;
                    
                case GameState::GAME_OVER:
                    if (event.key.code == sf::Keyboard::Return) {
                        m_state = GameState::MENU;
                    }
                    break;
                    
                case GameState::LEVEL_COMPLETE:
                    if (event.key.code == sf::Keyboard::Return) {
                        startNextLevel();
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // Continuous input handling (only during gameplay)
    if (m_state == GameState::PLAYING) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            m_player.moveLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            m_player.moveRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            m_player.shoot();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            m_player.useSuperzapper();
        }
    }
}

void Game::update(float deltaTime) {
    // Update text elements
    updateScoreText();
    
    // Blink instruction text in menu
    static float blinkTimer = 0.0f;
    blinkTimer += deltaTime;
    if (blinkTimer >= 0.5f) {
        blinkTimer = 0.0f;
        if (m_state == GameState::MENU || m_state == GameState::GAME_OVER) {
            m_instructionText.setFillColor(
                m_instructionText.getFillColor() == sf::Color::White ? 
                sf::Color::Transparent : sf::Color::White
            );
        }
    }
    
    // State-specific updates
    switch (m_state) {
        case GameState::PLAYING:
            m_player.update(deltaTime);
            m_enemyManager.update(deltaTime);
            m_levelManager.update(deltaTime);
            
            checkCollisions();
            
            // Check if level is complete
            // Only check if there are enemies to clear
            if (m_enemyManager.getEnemies().size() > 0 && m_enemyManager.areEnemiesCleared()) {
                m_state = GameState::LEVEL_COMPLETE;
                // Add level completion bonus
                m_score += 1000 * m_level;
                updateScoreText();
            }
            break;
            
        default:
            break;
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);
    
    // State-specific rendering
    switch (m_state) {
        case GameState::MENU:
            renderMenu();
            break;
            
        case GameState::PLAYING:
            renderGame();
            break;
            
        case GameState::GAME_OVER:
            renderGameOver();
            break;
            
        case GameState::LEVEL_COMPLETE:
            renderLevelComplete();
            break;
    }
    
    m_window.display();
}

void Game::renderMenu() {
    // Draw text elements if font loaded
    m_window.draw(m_titleText);
    m_window.draw(m_instructionText);
    m_window.draw(m_controlsText);
    m_window.draw(m_highScoreText);
    
    // Draw a vector-style Tempest logo
    drawTempestLogo();
    
    Utils::printMessage("Rendering menu screen");
}

void Game::drawTempestLogo() {
    // Draw a vector-style Tempest logo using lines
    const float centerX = m_window.getSize().x / 2.0f;
    const float centerY = 200.0f;
    const float size = 150.0f;
    
    // Create a hexagon shape for the logo
    sf::VertexArray hexagon(sf::LineStrip, 7);
    for (int i = 0; i < 7; ++i) {
        float angle = i * 2.0f * M_PI / 6.0f;
        hexagon[i].position = sf::Vector2f(
            centerX + size * std::cos(angle),
            centerY + size * std::sin(angle)
        );
        hexagon[i].color = sf::Color::Yellow;
    }
    
    // Create spokes radiating from the center
    sf::VertexArray spokes(sf::Lines, 12);
    for (int i = 0; i < 6; ++i) {
        float angle = i * 2.0f * M_PI / 6.0f;
        spokes[i*2].position = sf::Vector2f(centerX, centerY);
        spokes[i*2].color = sf::Color::Yellow;
        
        spokes[i*2+1].position = sf::Vector2f(
            centerX + size * std::cos(angle),
            centerY + size * std::sin(angle)
        );
        spokes[i*2+1].color = sf::Color::Yellow;
    }
    
    m_window.draw(hexagon);
    m_window.draw(spokes);
}

void Game::renderGame() {
    m_playfield.draw(m_window);
    m_player.draw(m_window);
    m_enemyManager.draw(m_window);
    
    // Draw HUD elements
    m_window.draw(m_scoreText);
    m_window.draw(m_highScoreText);
    m_window.draw(m_levelText);
    m_window.draw(m_livesText);
    
    Utils::printMessage("Rendering game screen");
}

void Game::renderGameOver() {
    m_window.draw(m_gameOverText);
    m_window.draw(m_scoreText);
    m_window.draw(m_highScoreText);
    m_window.draw(m_instructionText);
}

void Game::renderLevelComplete() {
    sf::Text levelCompleteText;
    levelCompleteText.setFont(m_font);
    levelCompleteText.setString("LEVEL " + std::to_string(m_level) + " COMPLETE!");
    levelCompleteText.setCharacterSize(48);
    levelCompleteText.setFillColor(sf::Color::Green);
    levelCompleteText.setPosition(
        (m_window.getSize().x - levelCompleteText.getLocalBounds().width) / 2.0f,
        200.0f
    );
    
    sf::Text continueText;
    continueText.setFont(m_font);
    continueText.setString("PRESS ENTER TO CONTINUE");
    continueText.setCharacterSize(24);
    continueText.setFillColor(sf::Color::White);
    continueText.setPosition(
        (m_window.getSize().x - continueText.getLocalBounds().width) / 2.0f,
        300.0f
    );
    
    m_window.draw(levelCompleteText);
    m_window.draw(continueText);
    m_window.draw(m_scoreText);
}

void Game::checkCollisions() {
    // Check collisions between player shots and enemies
    const auto& enemies = m_enemyManager.getEnemies();
    const auto& shots = m_player.getShots();
    
    for (const auto& shot : shots) {
        for (auto& enemy : const_cast<std::vector<Enemy>&>(enemies)) {
            if (!enemy.isDestroyed() && !shot.isOutOfBounds()) {
                float distance = std::sqrt(
                    std::pow(shot.getPosition().x - enemy.getPosition().x, 2) +
                    std::pow(shot.getPosition().y - enemy.getPosition().y, 2)
                );
                
                if (distance < (shot.getRadius() + enemy.getRadius())) {
                    enemy.destroy();
                    const_cast<Shot&>(shot).destroy();
                    
                    // Award points based on enemy type
                    switch (enemy.getType()) {
                        case Enemy::Type::FLIPPER:
                            m_score += 150;
                            break;
                        case Enemy::Type::TANKER:
                            m_score += 200;
                            break;
                        case Enemy::Type::SPIKER:
                            m_score += 250;
                            break;
                        case Enemy::Type::FUSEBALL:
                            m_score += 300;
                            break;
                        case Enemy::Type::PULSAR:
                            m_score += 350;
                            break;
                        default:
                            m_score += 100;
                            break;
                    }
                    
                    // Update high score if needed
                    if (m_score > m_highScore) {
                        m_highScore = m_score;
                    }
                }
            }
        }
    }
    
    // Check collisions between player and enemies
    for (const auto& enemy : enemies) {
        if (!enemy.isDestroyed() && enemy.isAtEdge() && enemy.getLane() == m_player.getPosition()) {
            // Player hit by enemy
            playerHit();
        }
    }
}

void Game::startGame() {
    m_state = GameState::PLAYING;
    m_score = 0;
    m_level = 1;
    m_lives = 3;
    
    // Reset game elements
    m_playfield = Playfield(Playfield::Type::CIRCLE, 16);
    m_player = Player(m_playfield);
    m_enemyManager = EnemyManager(m_playfield);
    m_levelManager = LevelManager();
    
    // Set initial enemy spawn rate and speed
    m_enemyManager.setSpawnRate(m_levelManager.getEnemySpawnRate());
    m_enemyManager.setEnemySpeed(m_levelManager.getEnemySpeed());
    
    updateScoreText();
}

void Game::startNextLevel() {
    m_level++;
    m_levelManager.startNextLevel();
    m_playfield = Playfield(m_levelManager.getCurrentPlayfieldType(), m_levelManager.getNumSegments());
    m_player = Player(m_playfield);
    m_enemyManager = EnemyManager(m_playfield);
    m_enemyManager.setSpawnRate(m_levelManager.getEnemySpawnRate());
    m_enemyManager.setEnemySpeed(m_levelManager.getEnemySpeed());
    m_state = GameState::PLAYING;
}

void Game::playerHit() {
    m_lives--;
    
    if (m_lives <= 0) {
        m_state = GameState::GAME_OVER;
        
        // Update instruction text for game over screen
        m_instructionText.setString("PRESS ENTER TO CONTINUE");
        m_instructionText.setPosition(
            (m_window.getSize().x - m_instructionText.getLocalBounds().width) / 2.0f,
            300.0f
        );
    }
}

void Game::updateScoreText() {
    std::stringstream ss;
    ss << "SCORE: " << m_score;
    m_scoreText.setString(ss.str());
    
    ss.str("");
    ss << "HIGH SCORE: " << m_highScore;
    m_highScoreText.setString(ss.str());
    
    ss.str("");
    ss << "LEVEL: " << m_level;
    m_levelText.setString(ss.str());
    
    ss.str("");
    ss << "LIVES: " << m_lives;
    m_livesText.setString(ss.str());
}

void Game::loadHighScore() {
    std::ifstream file("highscore.dat");
    if (file.is_open()) {
        file >> m_highScore;
        file.close();
    }
}

void Game::saveHighScore() {
    std::ofstream file("highscore.dat");
    if (file.is_open()) {
        file << m_highScore;
        file.close();
    }
}

} // namespace tempest

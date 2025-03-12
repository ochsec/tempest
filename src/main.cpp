#include <iostream>
#include "Game.hpp"
#include "utils.hpp"

int main() {
    try {
        tempest::Utils::printMessage("Starting Tempest Game");
        
        tempest::Game game;
        game.run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}

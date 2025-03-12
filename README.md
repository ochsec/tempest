# Tempest

A C++ implementation of the classic Atari arcade game Tempest, using SFML for graphics rendering.

## Game Description

Tempest is a tube-shooting game that uses vector graphics to create a 3D perspective playfield. The player controls a claw-shaped "blaster" that moves around the outer edge of a geometric playfield. Enemies emerge from the center and move outward toward the player's position. The player must shoot the enemies before they reach the outer edge.

## Project Structure

```
tempest/
├── CMakeLists.txt       # CMake build configuration
├── include/             # Header files
│   ├── Game.hpp         # Main game class
│   ├── Playfield.hpp    # Playfield geometry
│   ├── Player.hpp       # Player controls and rendering
│   ├── Shot.hpp         # Player projectiles
│   ├── Enemy.hpp        # Enemy entities
│   ├── EnemyManager.hpp # Enemy spawning and management
│   ├── Level.hpp        # Level configuration
│   ├── LevelManager.hpp # Level progression
│   └── utils.hpp        # Utility functions
├── src/                 # Source files
│   ├── main.cpp         # Main application entry point
│   ├── Game.cpp         # Game implementation
│   ├── Playfield.cpp    # Playfield implementation
│   ├── Player.cpp       # Player implementation
│   ├── Shot.cpp         # Shot implementation
│   ├── Enemy.cpp        # Enemy implementation
│   ├── EnemyManager.cpp # Enemy manager implementation
│   ├── Level.cpp        # Level implementation
│   └── LevelManager.cpp # Level manager implementation
├── .vscode/             # VSCode configuration
│   └── c_cpp_properties.json
└── .gitignore           # Git ignore file
```

## Requirements

- CMake 3.14 or higher
- C++11 compatible compiler (GCC, Clang, MSVC)
- SFML 2.5 or higher (automatically downloaded and built by CMake)

## Building the Project

### Create a build directory

```bash
mkdir -p build
cd build
```

### Generate build files with CMake

```bash
cmake ..
```

### Build the project

```bash
cmake --build .
```

## Running the Application

After building, you can run the application:

```bash
# From the build directory
./tempest
```

## Game Controls

- **Left/Right Arrow Keys**: Move the player around the edge of the playfield
- **Space**: Shoot
- **Z**: Use Superzapper (clears all enemies, limited use)
- **Escape**: Quit the game

## Features

- Vector-style graphics with a 3D perspective playfield
- Multiple playfield shapes (circle, square, hexagon, etc.)
- Different enemy types with unique behaviors
- Level progression with increasing difficulty
- Collision detection
- Scoring system
- Superzapper special weapon

## Implementation Details

- Uses SFML for graphics rendering
- C++11 standard compliant
- CMake build system with FetchContent for SFML dependency
- Object-oriented design with separate classes for game components

cmake_minimum_required(VERSION 3.14)  # Upgraded for better FetchContent support
project(tempest VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include FetchContent module
include(FetchContent)

# Declare SFML dependency
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x  # Using the latest stable branch
)

# Make SFML available
FetchContent_MakeAvailable(SFML)

# Add executable target
add_executable(tempest 
    src/main.cpp
    src/Game.cpp
    src/Playfield.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Shot.cpp
    src/Level.cpp
    src/EnemyManager.cpp
    src/LevelManager.cpp
)

# Include directories
include_directories(include)

# Link SFML libraries
target_link_libraries(tempest PRIVATE sfml-graphics sfml-window sfml-system)

cmake_minimum_required(VERSION 3.28)
project(HappyBalls LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

set(
    SOURCES
    src/AssetsManager.cpp
    src/Ball.cpp
    src/Button.cpp
    src/Field.cpp
    src/Game.cpp
    src/GameOverScene.cpp
    src/main.cpp
    src/Scaler.cpp
    src/Scene.cpp
    src/ScoreBar.cpp
)

add_executable(happyballs ${SOURCES})
target_compile_features(happyballs PRIVATE cxx_std_17)
target_link_libraries(happyballs PRIVATE SFML::Graphics SFML::Window SFML::System)
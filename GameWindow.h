//
// Created by Xpc on 10/5/2025.
//

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "EnemyUnit.h"


class GameWindow {
private:
    sf::RenderWindow window;
    Map& map;
    const int tileSize = 50;
    const int rows = 15;
    const int cols = 25;

    std::vector<EnemyUnit*> enemies;
    sf::Clock spawnClock; // para controlar cada cuánto aparece un enemigo

public:
    GameWindow(Map& mapRef);
    void run();
    void drawMap();
    sf::Vector2i getCellFromMouse(const sf::Vector2i& mousePos);
    void spawnEnemy(); // ← nuevo método
};


#endif

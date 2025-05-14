#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "EnemyUnit.h"
#include <vector>

struct Shot {
    sf::Vector2f from;
    sf::Vector2f to;
};

class GameWindow {
private:
    sf::RenderWindow window;
    Map& map;
    const int tileSize = 50;
    const int rows = 15;
    const int cols = 25;

    std::vector<Shot> activeShots;
    std::vector<EnemyUnit*> enemies;
    sf::Clock spawnClock;
    int enemyCounter=0;

    // 🧱 Texturas del mapa
    sf::Texture texGrass, texGrass2;
    sf::Texture texCornerTL, texCornerTR, texCornerBL, texCornerBR;
    sf::Texture texRoad[8];
    sf::Texture texTowerLvl1;
    sf::Texture texTree;


public:
    GameWindow(Map& mapRef);

    void run();
    void drawMap();
    void drawTowers();
    sf::Vector2i getCellFromMouse(const sf::Vector2i& mousePos);
    void spawnEnemy();
    void updateCombat();
};

#endif

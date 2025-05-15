#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "EnemyUnit.h"
#include <vector>
#include "Projectile.h"


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
    int enemyCounter = 0;
    int playerGold = 50;

    // 🧱 Texturas del mapa
    sf::Texture texGrass, texGrass2;
    sf::Texture texCornerTL, texCornerTR, texCornerBL, texCornerBR;
    sf::Texture texRoad[8];
    sf::Texture texTowerA;
    sf::Texture texTowerM;
    sf::Texture texTowerS;
    sf::Texture texTree;
    sf::Texture texCastle;
    sf::Texture texPortal;
    sf::Texture texBulletA, texRocketM, texMissileS;
    std::vector<Projectile> projectiles;


    sf::Font font;
    sf::Text textOro;
    sf::Text textOleada;
    sf::Text textEnemigos;
    int oleada = 1;
    char torreSeleccionada = 'A'; // por defecto


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

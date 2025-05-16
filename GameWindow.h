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

struct Genome {
    float health;
    float speed;
    float arrowRes;
    float magicRes;
    float artRes;
    float fitness = 0.f;
    bool reachedEnd = false;
    char  type;
    int category;
};


class GameWindow {
private:
    sf::RenderWindow window;
    Map& map;
    const int tileSize = 50;
    const int rows = 15;
    const int cols = 25;

    sf::Clock gameOverClock;
    float countdownTime = 5.0f;
    sf::Text countdownText;

    std::vector<Shot> activeShots;
    std::vector<EnemyUnit*> enemies;
    sf::Clock spawnClock;
    int enemyCounter = 0;
    int playerGold = 50;

    int playerHP = 1000;
    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarFront;
    sf::Text gameOverText;
    bool gameOver = false;

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
    sf::RectangleShape botonMejorar;
    sf::Text textoMejorar;
    sf::Text textoInfoTorre;

    sf::Font font;
    sf::Text textOro;
    sf::Text textOleada;
    sf::Text textEnemigos;
    int oleada = 1;
    char tipoTorreSeleccionada = 'A';
    int torreSeleccionadaIndex = -1;

    std::vector<Genome> population;
    int populationSize = 20;
    float mutationChance = 0.1f;
    // estadísticas
    int muertosEstaOleada = 0;



public:
    GameWindow(Map& mapRef);

    void run();
    void drawMap();
    void drawTowers();
    sf::Vector2i getCellFromMouse(const sf::Vector2i& mousePos);
    void spawnEnemy();
    void updateCombat();
    void resetGame();

    void inicializarPoblacion();
    void spawnWave();                    // crea EnemyUnit a partir de poblacion[]
    void calcularFitness();              // al terminar la oleada
    void evolucionarPoblacion();         // selección + crossover + mutación
    void nextGeneration();               // invoca calcularFitness() + evolucionarPoblacion() + spawnWave()


};

#endif

//
// Created by Xpc on 10/5/2025.
//

#include "GameWindow.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

// Constructor
GameWindow::GameWindow(Map& mapRef)
    : map(mapRef), window(sf::VideoMode(25 * 50, 15 * 50), "Genetic Kingdom") {
    window.setFramerateLimit(60);

    texGrass.loadFromFile("Sprites/grass.png");
    texGrass2.loadFromFile("Sprites/grass2.png");

    texCornerTL.loadFromFile("Sprites/top_left_corner.png");
    texCornerTR.loadFromFile("Sprites/top_right_corner.png");
    texCornerBL.loadFromFile("Sprites/bottom_left_corner.png");
    texCornerBR.loadFromFile("Sprites/bottom_right_corner.png");

    for (int i = 0; i < 8; ++i) {
        texRoad[i].loadFromFile("Sprites/road" + std::to_string(i+1) + ".png");
    }

    texTowerLvl1.loadFromFile("Sprites/tower_lvl1.png");
    texTree.loadFromFile("Sprites/tree.png");




    std::cout << "Creando ventana de juego\n";
}

// Convierte la posición del mouse a coordenadas de celda
sf::Vector2i GameWindow::getCellFromMouse(const sf::Vector2i& mousePos) {
    return sf::Vector2i(mousePos.x / tileSize, mousePos.y / tileSize);
}



// Método principal de loop gráfico
void GameWindow::run() {
    std::cout << "Entrando al bucle principal\n";


    while (window.isOpen()) {
        // --- Manejo de eventos ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Clic izquierdo para colocar torres
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                sf::Vector2i cell = getCellFromMouse(mousePixel);
                Tower t(cell.x, cell.y, 'A', 5, 2, 3, 10, 5); // Torre básica

                bool ocupado = false;
                for (auto& enemy : enemies) {
                    sf::Vector2f pos = enemy->getPosition(); // Necesitamos agregar este método
                    int eCol = static_cast<int>(pos.x) / tileSize;
                    int eRow = static_cast<int>(pos.y) / tileSize;
                    if (eCol == cell.x && eRow == cell.y) {
                        ocupado = true;
                        break;
                    }
                }

                if (ocupado) {
                    std::cout << "No se puede colocar torre: un enemigo está sobre esa celda.\n";
                } else if (map.placeTower(cell.y, cell.x, t)) {
                    std::cout << "Torre colocada en (" << cell.y << ", " << cell.x << ")\n";
                }
                else {
                    std::cout << "No se pudo colocar torre\n";
                }
            }
        }

        // --- Lógica de juego ---

        // 1. Crear nuevo enemigo cada 3 segundos
        if (spawnClock.getElapsedTime().asSeconds() > 3.0f) {
            spawnEnemy();
            spawnClock.restart();
        }

        // 2. Actualizar todos los enemigos
        for (auto& enemy : enemies)
            enemy->update();

        updateCombat();

        // --- Dibujo ---
        window.clear(sf::Color(80, 200, 120)); // fondo menta claro

        drawMap();      // fondo visual
        drawTowers();   // torres giratorias

        for (auto& enemy : enemies)
            enemy->draw(window); // enemigos activos

        // Dibujar disparos (efecto rayo/bomba)
        for (const auto& shot : activeShots) {
            sf::Vertex line[] = {
                sf::Vertex(shot.from, sf::Color::Cyan),
                sf::Vertex(shot.to, sf::Color::Cyan)
            };
            window.draw(line, 2, sf::Lines);
        }

        activeShots.clear();
        window.display();
    }
}

// Genera un nuevo enemigo y lo agrega al vector de enemigos
void GameWindow::spawnEnemy() {
    Enemy e(0, 0, 0, 0, 'O'); // enemigo básico
    std::vector<std::pair<int, int>> rawPath = map.findPath(e);

    if (rawPath.empty()) {
        std::cout << "No se puede generar enemigo: no hay camino\n";
        return;
    }

    std::vector<sf::Vector2i> path;
    for (const auto& p : rawPath)
        path.push_back(sf::Vector2i(p.second, p.first)); // columna=x, fila=y

    if (enemyCounter < 5) {
        enemies.push_back(new EnemyUnit(path, tileSize, 'O'));
        enemyCounter++;
    } else if (enemyCounter >= 5 && enemyCounter < 10) {
        enemies.push_back(new EnemyUnit(path, tileSize, 'E'));
        enemyCounter++;
    } else if (enemyCounter >= 10 && enemyCounter < 15) {
        enemies.push_back(new EnemyUnit(path, tileSize, 'H'));
        enemyCounter++;
    } else if (enemyCounter >= 15) {
        enemies.push_back(new EnemyUnit(path, tileSize, 'M'));
        enemyCounter++;
    }
    std::cout << "Enemigo creado. Total enemigos: " << enemies.size() << "\n";
}

// Dibuja el mapa y las torres
void GameWindow::drawMap() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            char cell = map.getCell(row, col);
            sf::Sprite tile;

            // Entrada
            if (cell == 'E') {
                tile.setTexture(texCornerTL);
            }
            // Salida
            else if (cell == 'B') {
                tile.setTexture(texCornerTR);
            }
            // Camino
            else if (cell == '#') {
                tile.setTexture(texRoad[rand() % 8]);  // usa 8 variaciones de tiles
            }
            // Obstáculo natural (opcional)
            else if (cell == 'T') {
                tile.setTexture(texTree);
            }
            // Césped variado
            else if ((row + col) % 2 == 0) {
                tile.setTexture(texGrass);
            } else {
                tile.setTexture(texGrass2);
            }

            tile.setPosition(col * tileSize, row * tileSize);
            window.draw(tile);
        }
    }
}


// Actualiza el combate: verifica si los enemigos están dentro del rango de ataque de las torres
void GameWindow::updateCombat() {
    std::vector<EnemyUnit*> vivos;

    for (auto& enemy : enemies) {
        bool fueAtacado = false;

        for (const auto& tower : map.getTowers()) {
            sf::Vector2i enemyGrid = enemy->getGridPosition();
            int dx = enemyGrid.x - tower.col;
            int dy = enemyGrid.y - tower.row;
            int distancia2 = dx * dx + dy * dy;

            if (distancia2 <= tower.range * tower.range) {
                if (distancia2 <= tower.range * tower.range) {
                    enemy->takeDamage(tower.damage);

                    Shot s;
                    s.from = sf::Vector2f(tower.col * tileSize + tileSize / 2,
                                          tower.row * tileSize + tileSize / 2);
                    s.to = enemy->getPosition();
                    activeShots.push_back(s);

                    fueAtacado = true;
                    break;
                }

            }
        }

        if (enemy->isAlive())
            vivos.push_back(enemy);
        else
            delete enemy; // libera memoria
    }

    enemies = vivos;
}

void GameWindow::drawTowers() {
    for (const auto& tower : map.getTowers()) {
        sf::Sprite sprite;
        sprite.setTexture(texTowerLvl1);
        sprite.setOrigin(tileSize / 2, tileSize / 2);
        sprite.setPosition(tower.col * tileSize + tileSize / 2,
                           tower.row * tileSize + tileSize / 2);

        // Buscar el enemigo más cercano dentro del rango
        float closestDist = 999999.f;
        sf::Vector2f target;

        for (auto& enemy : enemies) {
            sf::Vector2f enemyPos = enemy->getPosition();
            float dx = enemyPos.x - sprite.getPosition().x;
            float dy = enemyPos.y - sprite.getPosition().y;
            float distSquared = dx * dx + dy * dy;

            if (distSquared <= tower.range * tileSize * tower.range * tileSize && distSquared < closestDist) {
                closestDist = distSquared;
                target = enemyPos;
            }
        }

        // Si encontró un objetivo, rota el sprite y actualiza la torre
        if (closestDist < 999999.f) {
            float angle = atan2(target.y - sprite.getPosition().y,
                                target.x - sprite.getPosition().x) * 180.f / 3.14159265f;
            float rotated = angle + 90.f;
            sprite.setRotation(rotated);
            const_cast<Tower&>(tower).rotation = rotated;  // Guardar la última rotación
        } else {
            sprite.setRotation(tower.rotation);  // Usar la rotación previa
        }

        window.draw(sprite);
    }
}





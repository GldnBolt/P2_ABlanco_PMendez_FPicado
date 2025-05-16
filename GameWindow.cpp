//
// Created by Xpc on 10/5/2025.
//

#include "GameWindow.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Projectile.h"
#include <sstream>

// Constructor
GameWindow::GameWindow(Map& mapRef)
    : map(mapRef),
      window(sf::VideoMode(25 * 50 + 300, 15 * 50), "Genetic Kingdom")  // 1250 + 300 = 1550 ancho
{
    window.setFramerateLimit(60);

    // Carga de texturas
    texCastle.loadFromFile("Sprites/tree.png");
    texGrass.loadFromFile("Sprites/grass.png");
    texGrass2.loadFromFile("Sprites/grass2.png");

    texCornerTL.loadFromFile("Sprites/top_left_corner.png");
    texCornerTR.loadFromFile("Sprites/top_right_corner.png");
    texCornerBL.loadFromFile("Sprites/bottom_left_corner.png");
    texCornerBR.loadFromFile("Sprites/bottom_right_corner.png");

    for (int i = 0; i < 8; ++i) {
        texRoad[i].loadFromFile("Sprites/road" + std::to_string(i + 1) + ".png");
    }

    texTowerA.loadFromFile("Sprites/tower_lvl1.png");
    texTowerM.loadFromFile("Sprites/tower_lvl2.png");
    texTowerS.loadFromFile("Sprites/tower_lvl3.png");

    texBulletA.loadFromFile("Sprites/bullet_A.png");
    texRocketM.loadFromFile("Sprites/rocket_M.png");
    texMissileS.loadFromFile("Sprites/missile_S.png");


    texTree.loadFromFile("Sprites/tree.png");

    // Fuente para el HUD
    if (!font.loadFromFile("D:/GitHub/P2_ABlanco_PMendez_FPicado/P2_ABlanco_PMendez_FPicado/assets/ARIAL.TTF")) {
        std::cout << "Error al cargar la fuente\n";
    }
    else {
        std::cout << "Fuente cargada correctamente\n";
    }

    int hudX = 25 * 50 + 20;  // Posición X del panel lateral

    // HUD: oro
    textOro.setFont(font);
    textOro.setCharacterSize(24);
    textOro.setFillColor(sf::Color::Yellow);
    textOro.setPosition(hudX, 30);

    // HUD: oleada
    textOleada.setFont(font);
    textOleada.setCharacterSize(24);
    textOleada.setFillColor(sf::Color::Cyan);
    textOleada.setPosition(hudX, 70);

    // HUD: enemigos
    textEnemigos.setFont(font);
    textEnemigos.setCharacterSize(24);
    textEnemigos.setFillColor(sf::Color::White);
    textEnemigos.setPosition(hudX, 110);

    // Barra de vida (fondo y frente)
    hpBarBack.setSize(sf::Vector2f(200, 20));
    hpBarBack.setFillColor(sf::Color(100, 100, 100));
    hpBarBack.setPosition(cols * tileSize + 20, 150);

    hpBarFront.setSize(sf::Vector2f(200, 20));
    hpBarFront.setFillColor(sf::Color::Red);
    hpBarFront.setPosition(cols * tileSize + 20, 150);

    // Texto "GAME OVER"
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(cols * tileSize + 20, 200);

    countdownText.setFont(font);
    countdownText.setCharacterSize(24);
    countdownText.setFillColor(sf::Color::White);
    countdownText.setPosition(cols * tileSize + 20, 240);
    countdownText.setString(""); // inicial vacío

    botonMejorar.setSize(sf::Vector2f(200, 40));
    botonMejorar.setFillColor(sf::Color(30, 130, 230)); // Azul fuerte
    botonMejorar.setOutlineColor(sf::Color::White);
    botonMejorar.setOutlineThickness(2);
    botonMejorar.setPosition(cols * tileSize + 20, 300);

    textoMejorar.setFont(font);
    textoMejorar.setCharacterSize(20);
    textoMejorar.setFillColor(sf::Color::White);
    textoMejorar.setStyle(sf::Text::Bold);
    textoMejorar.setString("Mejorar torre");
    textoMejorar.setPosition(cols * tileSize + 30, 305);

    textoInfoTorre.setFont(font);
    textoInfoTorre.setCharacterSize(20);
    textoInfoTorre.setFillColor(sf::Color::White);
    textoInfoTorre.setPosition(cols * tileSize + 20, 220);

    std::cout << "Creando ventana de juego\n";
}

// Convierte la posición del mouse a coordenadas de celda
sf::Vector2i GameWindow::getCellFromMouse(const sf::Vector2i& mousePos) {
    return sf::Vector2i(mousePos.x / tileSize, mousePos.y / tileSize);
}

// Método principal de loop gráfico
void GameWindow::run() {
    std::cout << "Entrando al bucle principal\n";
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        for (auto& p : projectiles)
            p.update(dt);

        // --- Manejo de eventos ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameOver) {
                // Aún se dibujan cosas, pero no se actualiza el mundo
                window.clear(sf::Color(80, 200, 120));
                drawMap();
                drawTowers();
                for (auto& enemy : enemies) enemy->draw(window);
                for (auto& p : projectiles) p.draw(window);
                window.draw(hpBarBack);
                window.draw(hpBarFront);
                window.draw(gameOverText);
                window.draw(countdownText);
                window.display();
                continue;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) tipoTorreSeleccionada = 'A';
                if (event.key.code == sf::Keyboard::Num2) tipoTorreSeleccionada = 'M';
                if (event.key.code == sf::Keyboard::Num3) tipoTorreSeleccionada = 'S';

            }

            // Clic izquierdo para colocar torres o seleccionar
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                sf::Vector2f mouseF(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));
                sf::Vector2i cell = getCellFromMouse(mousePixel);

                // Clic en botón de mejora
                if (botonMejorar.getGlobalBounds().contains(mouseF)) {
                    if (torreSeleccionadaIndex >= 0 && torreSeleccionadaIndex < static_cast<int>(map.getTowers().size())) {
                        Tower& torre = map.getTowers()[torreSeleccionadaIndex];
                        if (torre.level < 3 && playerGold >= torre.upgradeCost) {
                            if (torre.upgrade(playerGold)) {
                                std::cout << "Torre mejorada a nivel " << torre.level << "\n";
                            }
                        }
                    }
                    break;
                }

                // Si clic fue en el panel derecho (HUD), no hacer nada
                if (mouseF.x >= cols * tileSize)
                    break;

                // Selección de torre
                torreSeleccionadaIndex = -1;
                const auto& torres = map.getTowers();
                for (size_t i = 0; i < torres.size(); ++i) {
                    if (torres[i].row == cell.y && torres[i].col == cell.x) {
                        torreSeleccionadaIndex = static_cast<int>(i);
                        std::cout << "Torre seleccionada en (" << torres[i].row << "," << torres[i].col << "), nivel " << torres[i].level << "\n";
                        break;
                    }
                }

                // 🔹 Colocar nueva torre
                Tower nueva(cell.x, cell.y, tipoTorreSeleccionada, 10);
                int costo = nueva.getCost();

                if (playerGold < costo) {
                    std::cout << "No tienes suficiente oro para torre tipo " << tipoTorreSeleccionada
                              << " (costo: " << costo << ")\n";
                    break;
                }

                bool ocupado = false;
                for (auto& enemy : enemies) {
                    sf::Vector2f pos = enemy->getPosition();
                    int eCol = static_cast<int>(pos.x) / tileSize;
                    int eRow = static_cast<int>(pos.y) / tileSize;
                    if (eCol == cell.x && eRow == cell.y) {
                        ocupado = true;
                        break;
                    }
                }

                if (ocupado) {
                    std::cout << "❌ No se puede colocar torre: un enemigo está sobre esa celda.\n";
                } else if (map.placeTower(cell.y, cell.x, nueva)) {
                    playerGold -= costo;
                    std::cout << "Torre tipo " << tipoTorreSeleccionada << " colocada en ("
                              << cell.y << ", " << cell.x << "). Oro restante: " << playerGold << "\n";

                    // Reseleccionar la torre recién colocada
                    const auto& nuevasTorres = map.getTowers();
                    for (size_t i = 0; i < nuevasTorres.size(); ++i) {
                        if (nuevasTorres[i].row == cell.y && nuevasTorres[i].col == cell.x) {
                            torreSeleccionadaIndex = static_cast<int>(i);
                            break;
                        }
                    }
                } else {
                    std::cout << "No se pudo colocar torre\n";
                }
            }

        }

        // --- Lógica de juego ---
        if (spawnClock.getElapsedTime().asSeconds() > 2.0f) {
            spawnEnemy();
            spawnClock.restart();
        }

        for (auto& enemy : enemies)
            enemy->update();

        for (auto& tower : map.getTowers())
            const_cast<Tower&>(tower).lastAttackTime += dt;

        updateCombat();

        // Actualizar proyectiles animados
        for (auto& p : projectiles)
            p.update(dt);

        // Eliminar proyectiles que llegaron a su destino
        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](Projectile& p) { return p.hasHit(); }),
            projectiles.end()
        );

        // --- HUD dinámico ---
        textOro.setString("Oro: " + std::to_string(playerGold));
        textOleada.setString("Oleada: " + std::to_string(oleada));
        textEnemigos.setString("Enemigos: " + std::to_string(enemies.size()));

        float hpRatio = static_cast<float>(playerHP) / 1000.f;
        hpBarFront.setSize(sf::Vector2f(200.f * hpRatio, 20.f));

        // --- Dibujo ---
        window.clear(sf::Color(80, 200, 120)); // fondo menta claro

        // Panel lateral
        sf::RectangleShape panelBG(sf::Vector2f(300, rows * tileSize));
        panelBG.setPosition(cols * tileSize, 0);
        panelBG.setFillColor(sf::Color(60, 60, 60));
        window.draw(panelBG);

        drawMap();
        drawTowers();

        for (auto& enemy : enemies)
            enemy->draw(window);

        for (auto& p : projectiles)
            p.draw(window);

        window.draw(hpBarBack);
        window.draw(hpBarFront);

        if (gameOver) {
            window.draw(gameOverText);
        }

        if (gameOver) {
            float elapsed = gameOverClock.getElapsedTime().asSeconds();
            float remaining = countdownTime - elapsed;

            if (remaining > 0) {
                countdownText.setString("Reiniciando en: " + std::to_string(static_cast<int>(std::ceil(remaining))) + "s");
            } else {
                resetGame();
            }

            window.draw(countdownText);
        }

        // Mostrar info de la torre seleccionada
        // Mostrar info de la torre seleccionada
        if (torreSeleccionadaIndex >= 0 && torreSeleccionadaIndex < (int)map.getTowers().size()) {
            Tower& torre = map.getTowers()[torreSeleccionadaIndex];

            // 1) Generar la cadena
            std::ostringstream info;
            info << "Torre seleccionada\n"
                 << "Nivel: " << torre.level << "\n"
                 << "Dano: "  << torre.damage << "\n"
                 << "Alcance: " << torre.range << "\n"
                 << "Cooldown: " << torre.attackCooldown << "s\n"
                 << "Costo Mejora: " << torre.upgradeCost;

            textoInfoTorre.setString(info.str());

            // 2) Posicionar texto e imprimirlo
            float panelX = cols * tileSize + 20.f;
            float infoY  = 220.f;
            textoInfoTorre.setPosition(panelX, infoY);
            window.draw(textoInfoTorre);

            // 3) Calcular el alto ocupado por el texto
            sf::FloatRect infoBounds = textoInfoTorre.getGlobalBounds();

            // 4) Re-posicionar el botón justo debajo
            float btnX = panelX;
            float btnY = infoBounds.top + infoBounds.height + 10.f;  // +10px de margen
            botonMejorar.setPosition(btnX, btnY);
            textoMejorar.setPosition(btnX + 10.f, btnY + 8.f);

            // 5) Dibujar botón + texto (si procede)
            if (torre.level < 3 && playerGold >= torre.upgradeCost) {
                window.draw(botonMejorar);
                window.draw(textoMejorar);
            }
        }

        // Dibujar HUD
        window.draw(textOro);
        window.draw(textOleada);
        window.draw(textEnemigos);

        window.display();
    }
}

// Genera un nuevo enemigo y lo agrega al vector de enemigos
void GameWindow::spawnEnemy() {
    Enemy e(0, 0, 1000, 1, 'O'); // estructura temporal solo para path
    std::vector<std::pair<int, int>> rawPath = map.findPath(e);

    if (rawPath.empty()) {
        std::cout << "No se puede generar enemigo: no hay camino\n";
        return;
    }

    std::vector<sf::Vector2i> path;
    for (const auto& p : rawPath)
        path.push_back(sf::Vector2i(p.second, p.first)); // columna=x, fila=y

    // Rotar tipo de enemigo para variedad
    static int count = 0;
    char tipo;
    int categoria;

    switch (count % 4) {
        case 0: tipo = 'O'; categoria = 1; break;
        case 1: tipo = 'E'; categoria = 2; break;
        case 2: tipo = 'H'; categoria = 2; break;
        case 3: tipo = 'M'; categoria = 3; break;
    }

    enemies.push_back(new EnemyUnit(path, tileSize, tipo, categoria));
    count++;

    std::cout << "👾 Enemigo tipo " << tipo << ", categoría " << categoria << " generado.\n";
}

// Dibuja el mapa y las torres
void GameWindow::drawMap() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            char cell = map.getCell(row, col);
            sf::Sprite tile;

            // Entrada
            if (cell == 'E') {
                tile.setTexture(texPortal);
            }
            // Salida
            else if (cell == 'B') {
                tile.setTexture(texCastle);
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

        for (auto& tower : map.getTowers()) {
            sf::Vector2i enemyGrid = enemy->getGridPosition();
            int dx = enemyGrid.x - tower.col;
            int dy = enemyGrid.y - tower.row;
            int distancia2 = dx * dx + dy * dy;

            if (distancia2 <= tower.range * tower.range) {
                // Verifica cooldown
                if (tower.lastAttackTime >= tower.attackCooldown) {
                    // Ataque con posibilidad de especial
                    float chance = static_cast<float>(rand()) / RAND_MAX;
                    int damage = (chance < tower.specialChance)
                                 ? tower.damage * 2
                                 : tower.damage;

                    enemy->takeDamage(damage, tower.type);
                    tower.lastAttackTime = 0.f;  // reinicia cooldown

                    // Seleccionar textura según tipo de torre
                    sf::Texture* tex = nullptr;
                    switch (tower.type) {
                        case 'A': tex = &texBulletA; break;
                        case 'M': tex = &texRocketM; break;
                        case 'S': tex = &texMissileS; break;
                    }

                    // Crear y agregar proyectil
                    sf::Vector2f from = sf::Vector2f(tower.col * tileSize + tileSize / 2,
                                                     tower.row * tileSize + tileSize / 2);
                    sf::Vector2f to = enemy->getPosition();
                    projectiles.emplace_back(*tex, from, to, tower.type, tower.rotation);
                }

                fueAtacado = true;
                break;
            }
        }

        if (enemy->hasReachedEnd()) {
            playerHP -= 50;
            std::cout << "💥 Enemigo alcanzó el castillo. Vida restante: " << playerHP << "\n";

            if (playerHP <= 0 && !gameOver) {
                gameOver = true;
                playerHP = 0;
                std::cout << "❌ GAME OVER\n";
                gameOverClock.restart(); // inicia el conteo de 5s
            }

            delete enemy;
            continue;
        }

        if (enemy->isAlive()) {
            vivos.push_back(enemy);
        } else {
            int categoria = enemy->getCategoria();
            char tipo = enemy->getTipo();
            int oroGanado = map.getTowers()[0].calculateGoldReward(tipo, categoria);

            playerGold += oroGanado;
            enemyCounter++;
            std::cout << "💰 Enemigo eliminado. +" << oroGanado << " oro. Total: " << playerGold << "\n";
            delete enemy;
        }
    }

    enemies = vivos;
}

void GameWindow::drawTowers() {
    for (const auto& tower : map.getTowers()) {
        sf::Sprite sprite;
        switch (tower.type) {
            case 'A': sprite.setTexture(texTowerA); break;
            case 'M': sprite.setTexture(texTowerM); break;
            case 'S': sprite.setTexture(texTowerS); break;
            default:  sprite.setTexture(texTowerA); break;
        }
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

void GameWindow::resetGame() {
    enemies.clear();
    projectiles.clear();
    playerGold = 50;
    playerHP = 1000;
    oleada = 1;
    enemyCounter = 0;
    gameOver = false;
    map.clearTowers();  // Necesitas implementar este método
    gameOverClock.restart();
    std::cout << "🔄 Juego reiniciado\n";
}






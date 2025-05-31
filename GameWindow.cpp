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
    texCastle.loadFromFile("Sprites/castle.png");
    texGrass.loadFromFile("Sprites/grass3.png");
    texGrass2.loadFromFile("Sprites/grass2.png");
    texPortal.loadFromFile("Sprites/portal.png");

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
    if (!font.loadFromFile("assets/ARIAL.TTF")) {
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

    textoStats.setFont(font);
    textoStats.setCharacterSize(20);
    textoStats.setFillColor(sf::Color::White);
    textoStats.setPosition(cols * tileSize + 20, 450);  // posición en panel lateral

    // Texto "GAME OVER"
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(cols * tileSize + 20, 600);

    countdownText.setFont(font);
    countdownText.setCharacterSize(24);
    countdownText.setFillColor(sf::Color::White);
    countdownText.setPosition(cols * tileSize + 20, 640);
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

    // Inicializa la primera generación y lanza la oleada
    inicializarPoblacion();
    spawnWave();

    while (window.isOpen()) {
        // Calcula delta-time
        float dt = clock.restart().asSeconds();

        // Actualiza proyectiles antes de procesar eventos
        for (auto& p : projectiles)
            p.update(dt);

        // --- Manejo de eventos ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Si estamos en Game Over, sólo dibujar HUD y esperar reinicio
            if (gameOver) {
                window.clear(sf::Color(80, 200, 120));
                drawMap();
                drawTowers();
                for (auto& enemy : enemies)   enemy->draw(window);
                for (auto& p     : projectiles) p.draw(window);
                window.draw(hpBarBack);
                window.draw(hpBarFront);
                window.draw(gameOverText);
                window.draw(countdownText);
                window.display();
                continue;
            }

            // Selección de tipo de torre con teclas 1,2,3
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) tipoTorreSeleccionada = 'A';
                if (event.key.code == sf::Keyboard::Num2) tipoTorreSeleccionada = 'M';
                if (event.key.code == sf::Keyboard::Num3) tipoTorreSeleccionada = 'S';
            }

            // Clic izquierdo: mejora, selección o colocación
            if (event.type == sf::Event::MouseButtonPressed
             && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                sf::Vector2f mouseF(static_cast<float>(mousePixel.x),
                                    static_cast<float>(mousePixel.y));
                sf::Vector2i cell = getCellFromMouse(mousePixel);

                // 1) Clic en el botón Mejorar
                if (botonMejorar.getGlobalBounds().contains(mouseF)) {
                    if (torreSeleccionadaIndex >= 0
                     && torreSeleccionadaIndex < static_cast<int>(map.getTowers().size())) {
                        Tower& torre = map.getTowers()[torreSeleccionadaIndex];
                        if (torre.level < 3
                         && playerGold >= torre.upgradeCost
                         && torre.upgrade(playerGold)) {
                            std::cout << "Torre mejorada a nivel "
                                      << torre.level << "\n";
                        }
                    }
                    break;  // sale sólo del while(pollEvent)
                }

                // 2) Si clic fue en el panel HUD (derecha), ignorar
                if (mouseF.x >= cols * tileSize)
                    break;

                // 3) Intentar seleccionar torre existente
                torreSeleccionadaIndex = -1;
                const auto& torres = map.getTowers();
                for (size_t i = 0; i < torres.size(); ++i) {
                    if (torres[i].row == cell.y
                     && torres[i].col == cell.x) {
                        torreSeleccionadaIndex = static_cast<int>(i);
                        std::cout << "Torre seleccionada en ("
                                  << torres[i].row << ","
                                  << torres[i].col << "), nivel "
                                  << torres[i].level << "\n";
                        break;
                    }
                }
                if (torreSeleccionadaIndex >= 0)
                    break;  // sólo selección, no colocar

                // 4) Colocar nueva torre
                Tower nueva(cell.x, cell.y, tipoTorreSeleccionada, 10);
                int costo = nueva.getCost();
                if (playerGold < costo) {
                    std::cout << "No tienes suficiente oro para torre tipo "
                              << tipoTorreSeleccionada << " (costo: "
                              << costo << ")\n";
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
                    std::cout << "No se puede colocar torre: un enemigo está encima.\n";
                } else if (map.placeTower(cell.y, cell.x, nueva)) {
                    playerGold -= costo;
                    std::cout << "Torre tipo " << tipoTorreSeleccionada
                              << " colocada en (" << cell.y << ", "
                              << cell.x << "). Oro restante: "
                              << playerGold << "\n";
                    // Reselecciona la recién colocada
                    const auto& nt = map.getTowers();
                    for (size_t i = 0; i < nt.size(); ++i) {
                        if (nt[i].row == cell.y
                         && nt[i].col == cell.x) {
                            torreSeleccionadaIndex = static_cast<int>(i);
                            break;
                        }
                    }
                } else {
                    std::cout << "No se pudo colocar torre\n";
                }
            }
        } // fin while pollEvent

        // --- Lógica de oleadas con GA ---
        bool oleadaTermino = std::all_of(
            enemies.begin(), enemies.end(),
            [](EnemyUnit* e){ return e->hasReachedEnd() || !e->isAlive(); }
        );
        if (oleadaTermino) {
            if (oleada >= 50) {
                gameWon = true;
                gameOver = true;  // para detener el juego y mostrar pantalla final
                std::cout << "¡Felicidades! Has ganado el juego.\n";
            } else {
                nextGeneration();
            }
        }


        // Actualiza enemigos, torres y combate
        for (auto& enemy : enemies) enemy->update();
        for (auto& tower : map.getTowers())
            const_cast<Tower&>(tower).lastAttackTime += dt;
        updateCombat();

        // Actualiza y limpia proyectiles
        for (auto& p : projectiles) p.update(dt);
        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](Projectile& p){ return p.hasHit(); }),
            projectiles.end()
        );
        // --- Actualiza estadísticas ---
        std::ostringstream stats;
        stats << "Mut. chance: " << static_cast<int>(mutationChance * 100) << "%\n"
              << "Mut. ocurridas: " << totalMutations;
        textoStats.setString(stats.str());

        // --- Actualiza HUD dinámico ---
        textOro.setString("Oro: " + std::to_string(playerGold));
        textOleada.setString("Oleada: " + std::to_string(oleada));
        textEnemigos.setString("Enemigos: " + std::to_string(enemies.size()));
        float hpRatio = static_cast<float>(playerHP) / 1000.f;
        hpBarFront.setSize(sf::Vector2f(200.f * hpRatio, 20.f));

        // --- Dibujado de todo ---
        window.clear(sf::Color(80, 200, 120));
        // Panel lateral
        sf::RectangleShape panelBG(
            sf::Vector2f(300, rows * tileSize)
        );
        panelBG.setPosition(cols * tileSize, 0);
        panelBG.setFillColor(sf::Color(60, 60, 60));
        window.draw(panelBG);

        drawMap();
        drawTowers();
        for (auto& enemy : enemies) enemy->draw(window);
        for (auto& p     : projectiles) p.draw(window);
        window.draw(hpBarBack);
        window.draw(hpBarFront);
        window.draw(textoStats);

        if (gameOver) {
            // Mostrar texto distinto según si ganó o perdió
            if (gameWon) {
                gameOverText.setString("¡Ganaste! Felicidades.");
            } else {
                gameOverText.setString("Game Over");
            }
            window.draw(gameOverText);
        }

        if (gameOver) {
            float elapsed   = gameOverClock.getElapsedTime().asSeconds();
            float remaining = countdownTime - elapsed;
            if (remaining > 0) {
                if (gameWon)
                    countdownText.setString(
                        "Reiniciando en: "
                        + std::to_string(static_cast<int>(std::ceil(remaining)))
                        + "s\nGracias por jugar!"
                    );
                else
                    countdownText.setString(
                        "Reiniciando en: "
                        + std::to_string(static_cast<int>(std::ceil(remaining)))
                        + "s"
                    );
            } else {
                resetGame();
            }
            window.draw(countdownText);
        }

        // Mostrar info de la torre seleccionada + botón bajo el texto
        if (torreSeleccionadaIndex >= 0
         && torreSeleccionadaIndex < static_cast<int>(map.getTowers().size())) {
            Tower& torre = map.getTowers()[torreSeleccionadaIndex];

            std::ostringstream info;
            info << "Torre seleccionada\n"
                 << "Nivel: "  << torre.level                     << "\n"
                 << "Dano: "   << torre.damage                    << "\n"
                 << "Alcance: "<< torre.range                     << "\n"
                 << "Cooldown: "<< torre.attackCooldown << "s\n"
                 << "Costo Mejora: " << torre.upgradeCost;

            textoInfoTorre.setString(info.str());
            float panelX = cols * tileSize + 20.f;
            float infoY  = 220.f;
            textoInfoTorre.setPosition(panelX, infoY);
            window.draw(textoInfoTorre);

            sf::FloatRect infoBounds = textoInfoTorre.getGlobalBounds();
            float btnX = panelX;
            float btnY = infoBounds.top + infoBounds.height + 10.f;
            botonMejorar.setPosition(btnX, btnY);
            textoMejorar.setPosition(btnX + 10.f, btnY + 8.f);

            if (torre.level < 3 && playerGold >= torre.upgradeCost) {
                window.draw(botonMejorar);
                window.draw(textoMejorar);
            }
        }

        // Dibujar HUD numérico
        window.draw(textOro);
        window.draw(textOleada);
        window.draw(textEnemigos);

        window.display();
    }
}

void GameWindow::inicializarPoblacion() {
    population.clear();
    for (int i = 0; i < populationSize; ++i) {
        Genome g;
        g.health     = 1000.f;
        g.speed      = 0.25f + static_cast<float>(rand())/RAND_MAX * 0.75f;
        g.arrowRes   = 1.f;
        g.magicRes   = 1.f;
        g.artRes     = 1.f;
        g.type       = 'O';       // por defecto ogro
        g.category   = 1;

        // Opcional: repartir tipos en la población
        int r = rand() % 4;
        switch (r) {
            case 0: g.type = 'O'; g.category = 1; break;
            case 1: g.type = 'E'; g.category = 2; break;
            case 2: g.type = 'H'; g.category = 2; break;
            case 3: g.type = 'M'; g.category = 3; break;
        }

        population.push_back(g);
    }
}

void GameWindow::spawnWave() {
    // 1) Limpia la oleada anterior
    for (auto* e : enemies) delete e;
    enemies.clear();
    muertosEstaOleada = 0;

    // 2) Para cada genoma en poblacion, crea un EnemyUnit
    for (const auto& g : population) {
        // genera el path con un Enemy temporal
        Enemy temp(0, 0, static_cast<int>(g.health), 1, g.type);
        auto rawPath = map.findPath(temp);
        std::vector<sf::Vector2i> path;
        path.reserve(rawPath.size());
        for (auto& p : rawPath)
            path.emplace_back(p.second, p.first);

        // crea la unidad usando el tipo y categoría del genoma
        EnemyUnit* e = new EnemyUnit(path, tileSize, g.type, g.category);

        // aplica los atributos genéticos
        e->setHealth(static_cast<int>(g.health));
        e->setSpeed(g.speed);
        e->setResistances(g.arrowRes, g.magicRes, g.artRes);

        enemies.push_back(e);
    }

    std::cout << "Oleada " << oleada << ": "
              << enemies.size() << " enemigos spawned.\n";
}

void GameWindow::calcularFitness() {
    // recorre enemies que murieron o llegaron
    for (int i = 0; i < populationSize; ++i) {
        // por ejemplo: fitness = cantidad viva al final (0 o 1) + frames sobrevividos
        population[i].fitness = enemies[i]->isAlive() ? 1.f : 0.f;
        population[i].reachedEnd = enemies[i]->hasReachedEnd();
    }
}

// Evoluciona la población: selecciona los mejores y cruza para crear nuevos genomas
void GameWindow::evolucionarPoblacion() { // Ordena por fitness y cruza los mejores
    std::sort(population.begin(), population.end(),
              [](auto& a, auto& b){ return a.fitness > b.fitness; });
    std::vector<Genome> nueva;
    nueva.push_back(population[0]); // mejor genoma
    nueva.push_back(population[1]); // segundo mejor

    while ((int)nueva.size() < populationSize) { // cruza los mejores
        const Genome& padreA = population[rand() % (populationSize/2)]; // selecciona al azar entre los mejores
        const Genome& padreB = population[rand() % (populationSize/2)]; // selecciona al azar entre los mejores
        Genome hijo;
        // simple promedio
        hijo.health = (padreA.health + padreB.health)/2;
        hijo.speed  = (padreA.speed  + padreB.speed )/2;
        hijo.arrowRes = (padreA.arrowRes + padreB.arrowRes)/2;
        hijo.magicRes = (padreA.magicRes + padreB.magicRes)/2;
        hijo.artRes   = (padreA.artRes   + padreB.artRes  )/2;
        // mutar
        if ((rand()/(float)RAND_MAX) < mutationChance) { // probabilidad de mutación
            hijo.speed += ((rand()/(float)RAND_MAX)-0.5f) * 0.2f; // variación de -0.1 a +0.1
            totalMutations++;
        }
        nueva.push_back(hijo);
    }
    population.swap(nueva);
}

void GameWindow::nextGeneration() {
    calcularFitness();
    evolucionarPoblacion();
    oleada++;
    spawnWave();
}

// Dibuja el mapa y las torres
void GameWindow::drawMap() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            char cell = map.getCell(row, col);
            sf::Sprite tile;

            // Zacate
            if (cell == ' ') {
                tile.setTexture(texGrass);
            }
            // Entrada
            else if (cell == 'E') {
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
                if (tower.lastAttackTime > tower.attackCooldown) {
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
            playerHP -= 25;
            std::cout << "Enemigo alcanzó el castillo. Vida restante: " << playerHP << "\n";

            if (playerHP <= 0 && !gameOver) {
                gameOver = true;
                playerHP = 0;
                std::cout << "GAME OVER\n";
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
            std::cout << "Enemigo eliminado. +" << oroGanado << " oro. Total: " << playerGold << "\n";
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
    playerGold = 150;
    playerHP = 1000;
    oleada = 1;
    enemyCounter = 0;
    gameOver = false;
    map.clearTowers();
    gameOverClock.restart();
    std::cout << "Juego reiniciado\n";
}






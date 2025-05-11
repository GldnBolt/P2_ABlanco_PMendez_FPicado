//
// Created by Xpc on 10/5/2025.
//

#include "GameWindow.h"
#include <iostream>
#include <SFML/Graphics.hpp>


// Constructor
GameWindow::GameWindow(Map& mapRef)
    : map(mapRef), window(sf::VideoMode(25 * 50, 15 * 50), "Genetic Kingdom") {
    window.setFramerateLimit(60);

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

                if (map.placeTower(cell.y, cell.x, t)) {
                    std::cout << "Torre colocada en (" << cell.y << ", " << cell.x << ")\n";
                } else {
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

        // --- Dibujo ---
        window.clear(sf::Color::Black);

        drawMap(); // Mapa y torres

        for (auto& enemy : enemies)
            enemy->draw(window); // Todos los enemigos

        window.display();
    }
}

void GameWindow::spawnEnemy() {
    Enemy e(0, 0, 100, 1, 'O'); // enemigo básico
    std::vector<std::pair<int, int>> rawPath = map.findPath(e);

    if (rawPath.empty()) {
        std::cout << "No se puede generar enemigo: no hay camino\n";
        return;
    }

    std::vector<sf::Vector2i> path;
    for (const auto& p : rawPath)
        path.push_back(sf::Vector2i(p.second, p.first)); // columna=x, fila=y

    enemies.push_back(new EnemyUnit(path, tileSize));
    std::cout << "Enemigo creado. Total enemigos: " << enemies.size() << "\n";
}

// Dibuja el mapa y las torres
void GameWindow::drawMap() {
    for (int row = 0; row < 15; ++row) {
        for (int col = 0; col < 25; ++col) {
            sf::RectangleShape tile(sf::Vector2f(tileSize - 1, tileSize - 1));
            tile.setPosition(col * tileSize, row * tileSize);

            char cell = map.getCell(row, col);
            if (cell == 'E') {
                tile.setFillColor(sf::Color::Green);
            } else if (cell == 'B') {
                tile.setFillColor(sf::Color::Blue);
            } else if (cell == 'A' || cell == 'M' || cell == 'T') {
                tile.setFillColor(sf::Color::Red);
            } else {
                tile.setFillColor(sf::Color(100, 100, 100));
            }

            window.draw(tile);
        }
    }
}




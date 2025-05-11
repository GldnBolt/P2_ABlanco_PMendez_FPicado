//
// Created by Xpc on 10/5/2025.
//

#include "GameWindow.h"
#include <iostream>

// Constructor
GameWindow::GameWindow(Map& mapRef) : map(mapRef), window(sf::VideoMode(500, 500), "Genetic Kingdom") {
    window.setFramerateLimit(60);
    std::cout << "Ventana SFML creada\n";
}

// Convierte la posición del mouse a coordenadas de celda
sf::Vector2i GameWindow::getCellFromMouse(const sf::Vector2i& mousePos) {
    return sf::Vector2i(mousePos.x / tileSize, mousePos.y / tileSize);
}

// Método principal de loop gráfico
void GameWindow::run() {
    std::cout << "Entrando al bucle principal\n";
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Manejo de clic izquierdo para colocar torre
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                sf::Vector2i cell = getCellFromMouse(mousePixel);
                Tower t(cell.x, cell.y, 'A', 5, 2, 3, 10, 5); // Torre ejemplo
                if (map.placeTower(cell.y, cell.x, t)) {
                    std::cout << "Torre colocada en (" << cell.y << ", " << cell.x << ")\n";
                }
            }
        }

        window.clear(sf::Color::Black);
        drawMap(); // Dibuja mapa y torres
        window.display();
    }
}

// Dibuja el mapa y las torres
void GameWindow::drawMap() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            sf::RectangleShape tile(sf::Vector2f(tileSize - 1, tileSize - 1));
            tile.setPosition(col * tileSize, row * tileSize);

            char cell = map.getCell(row, col);
            if (cell == 'E') {
                tile.setFillColor(sf::Color::Green); // Entrada
            } else if (cell == 'B') {
                tile.setFillColor(sf::Color::Blue); // Puente
            } else if (cell == 'A' || cell == 'M' || cell == 'T') {
                tile.setFillColor(sf::Color::Red); // Torre
            } else {
                tile.setFillColor(sf::Color(100, 100, 100)); // Celda vacía
            }

            window.draw(tile);
        }
    }
}


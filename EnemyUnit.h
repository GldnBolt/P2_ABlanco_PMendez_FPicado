//
// Created by Xpc on 10/5/2025.
//

#ifndef ENEMY_UNIT_H
#define ENEMY_UNIT_H

#include <SFML/Graphics.hpp>
#include <vector>

class EnemyUnit {
private:
    sf::CircleShape shape;
    std::vector<sf::Vector2i> path;
    int currentIndex;
    float speed; // en píxeles por segundo
    sf::Vector2f position;
    sf::Clock clock;
    int tileSize;

public:
    EnemyUnit(const std::vector<sf::Vector2i>& path, int tileSize);

    void update();                         // Mueve al enemigo paso a paso
    void draw(sf::RenderWindow& window);   // Lo dibuja en pantalla
    bool hasReachedEnd() const;            // Ya llegó al destino
    sf::Vector2f getPosition() const;      // ← Necesario para verificar posición del enemigo
};

#endif
//
// Created by Xpc on 10/5/2025.
//

#ifndef ENEMY_UNIT_H
#define ENEMY_UNIT_H

#include <SFML/Graphics.hpp>
#include <vector>

class EnemyUnit {
private:
    sf::CircleShape shape; // forma del enemigo
    std::vector<sf::Vector2i> path; // ruta a seguir
    int currentIndex; // índice de la celda actual
    sf::Vector2f position; // posición en píxeles
    sf::Clock clock; // reloj para controlar el tiempo
    int tileSize; // tamaño de la celda en píxeles

    int health = 100; // vida del enemigo
    float speed; // en píxeles por segundo
    int type; // tipo de enemigo
    int arroewResistance; // resistencia a flechas
    int magicResistance; // resistencia a magia
    int artilleryResistance;

    sf::Color color;


public:
    EnemyUnit(const std::vector<sf::Vector2i>& path, int tileSize, char type);

    void update();                         // Mueve al enemigo paso a paso
    void draw(sf::RenderWindow& window);   // Lo dibuja en pantalla
    bool hasReachedEnd() const;            // Ya llegó al destino
    sf::Vector2f getPosition() const;      // ← Necesario para verificar posición del enemigo
    int getHealth() const;               // Devuelve la vida actual
    void takeDamage(int dmg);            // Resta vida al enemigo
    bool isAlive() const;               // Devuelve true si el enemigo sigue vivo
    sf::Vector2i getGridPosition() const;   // Devuelve la posición en la cuadrícula

};

#endif
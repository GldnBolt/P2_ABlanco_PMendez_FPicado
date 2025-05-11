//
// Created by Xpc on 10/5/2025.
//

#include "EnemyUnit.h"
#include <cmath>
#include <iostream>

EnemyUnit::EnemyUnit(const std::vector<sf::Vector2i>& path, int tileSize)
    : path(path), currentIndex(0), speed(100.f), tileSize(tileSize), health(100) {

    shape.setRadius(tileSize / 4);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(shape.getRadius(), shape.getRadius());

    if (!path.empty()) {
        position = sf::Vector2f(path[0].x * tileSize + tileSize / 2,
                                path[0].y * tileSize + tileSize / 2);
        shape.setPosition(position);
        std::cout << "Inicializando enemigo en celda: (" << path[0].y << ", " << path[0].x << ")\n";
        std::cout << "Posicion en pixeles: (" << position.x << ", " << position.y << ")\n";
    }
}

void EnemyUnit::update() {
    if (currentIndex + 1 >= path.size()) return;

    float dt = clock.getElapsedTime().asSeconds();
    if (dt < 0.1f) return;

    clock.restart();

    currentIndex++;
    sf::Vector2i next = path[currentIndex];
    position = sf::Vector2f(next.x * tileSize + tileSize / 2,
                            next.y * tileSize + tileSize / 2);

    shape.setPosition(position);
}

void EnemyUnit::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool EnemyUnit::hasReachedEnd() const {
    return currentIndex + 1 >= path.size();
}

sf::Vector2f EnemyUnit::getPosition() const {
    return position;
}

int EnemyUnit::getHealth() const {
    return health;
}

void EnemyUnit::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

bool EnemyUnit::isAlive() const {
    return health > 0;
}

sf::Vector2i EnemyUnit::getGridPosition() const {
    return sf::Vector2i(static_cast<int>(position.x) / tileSize,
                        static_cast<int>(position.y) / tileSize);
}



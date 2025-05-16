#include "EnemyUnit.h"
#include <cmath>
#include <iostream>

EnemyUnit::EnemyUnit(const std::vector<sf::Vector2i>& path, int tileSize, char type, int category)
    : path(path), currentIndex(0), tileSize(tileSize), type(type), category(category), health(1000) {

    switch (type) {
        case 'O': // Ogro
            speed = 0.75f;
            arrowResistance = 0.5f;
            magicResistance = 2.f;
            artilleryResistance = 2.f;
            color = sf::Color::White;
            break;
        case 'E': // Esqueleto
            speed = 0.25f;
            arrowResistance = 2.f;
            magicResistance = 0.5f;
            artilleryResistance = 2.f;
            color = sf::Color::Green;
            break;
        case 'H': // Humano
            speed = 0.5f;
            arrowResistance = 1.f;
            magicResistance = 1.f;
            artilleryResistance = 0.f;
            color = sf::Color::Yellow;
            break;
        case 'M': // Mago oscuro
            speed = 0.5f;
            arrowResistance = 0.5f;
            magicResistance = 2.f;
            artilleryResistance = 0.5f;
            color = sf::Color::Blue;
            break;
        default:
            speed = 0.5f;
            arrowResistance = 1.f;
            magicResistance = 1.f;
            artilleryResistance = 1.f;
            color = sf::Color::Red;
            break;
    }

    shape.setRadius(tileSize / 4);
    shape.setFillColor(color);
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
    if (dt < speed) return;

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

void EnemyUnit::takeDamage(int dmg, char damageType) {
    switch (damageType) {
        case 'B':
            health -= dmg * arrowResistance;
            break;
        case 'A':
            health -= dmg * artilleryResistance;
            break;
        case 'M':
            health -= dmg * magicResistance;
            break;
        default:
            health -= dmg;
            break;
    }

    if (health < 0) health = 0;
}

bool EnemyUnit::isAlive() const {
    return health > 0;
}

sf::Vector2i EnemyUnit::getGridPosition() const {
    return sf::Vector2i(static_cast<int>(position.x) / tileSize,
                        static_cast<int>(position.y) / tileSize);
}

char EnemyUnit::getTipo() const {
    return type;
}

int EnemyUnit::getCategoria() const {
    return category;
}

void EnemyUnit::setHealth(int h) {
    health = h;
}

void EnemyUnit::setSpeed(float s) {
    speed = s;
}

void EnemyUnit::setResistances(float arrowRes, float magicRes, float artilleryRes) {
    arrowResistance     = arrowRes;
    magicResistance     = magicRes;
    artilleryResistance = artilleryRes;
}

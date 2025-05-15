#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Projectile {
public:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f target;
    float speed;
    char towerType;
    bool hit = false;


    Projectile(sf::Texture& texture, const sf::Vector2f& from, const sf::Vector2f& to, char type, float rotation)
        : position(from), target(to), speed(200.f), towerType(type) {
        sprite.setRotation(rotation);
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        sprite.setPosition(position);
        sprite.setScale(0.5f, 0.5f); // Reducir tamaño si es muy grande

        // Inicialmente apunta al objetivo
        sf::Vector2f direction = target - position;
        float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
        sprite.setRotation(angle);
    }

    void update(float dt) {
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < speed * dt) {
            hit = true;
            return;
        }

        position += direction / distance * speed * dt;
        sprite.setPosition(position);

        // 🔁 Rota visualmente en tiempo real hacia su objetivo
        float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f + 90.f;
        sprite.setRotation(angle);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    bool hasHit() const { return hit; }
    sf::Vector2f getTarget() const { return target; }
    char getType() const { return towerType; }
};

#endif

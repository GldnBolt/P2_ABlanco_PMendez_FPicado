//
// Created by Xpc on 10/5/2025.
//

#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    int x, y;
    int health;
    int speed;
    char type;

    Enemy(int x, int y, int health, int speed, char type);
};

#endif


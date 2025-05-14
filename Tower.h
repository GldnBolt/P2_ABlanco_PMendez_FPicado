//
// Created by Xpc on 10/5/2025.
//

#ifndef TOWER_H
#define TOWER_H

class Tower {
public:
    int row, col;
    char type;
    int cost;
    int level = 0;
    int damage;
    int speed;
    int range;
    int specialCooldown;
    int attackCooldown;
    float rotation = 0.f;

    Tower(int x, int y, char type, int specialCooldown);
    int calculateGoldReward(char enemyType, int enemyCategory) const;
};

#endif


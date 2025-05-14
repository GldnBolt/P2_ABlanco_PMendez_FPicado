//
// Created by Xpc on 10/5/2025.
//

#include "Tower.h"

Tower::Tower(int x, int y, char type, int specialCooldown)
    : row(x), col(y), type(type), specialCooldown(specialCooldown){

    switch (type) {
        case 'S':
            range = 7;
            damage = 5;
            attackCooldown = 1;
            break;
        case 'M':
            range = 5;
            damage = 10;
            attackCooldown = 2;
            break;
        case 'A':
            range = 3;
            damage = 20;
            attackCooldown = 3;
            break;
    }
}

int Tower::calculateGoldReward(char enemyType, int enemyCategory) const {
    int baseReward = 10;
    int categoryBonus = enemyCategory * 5;
    return baseReward + categoryBonus;
}


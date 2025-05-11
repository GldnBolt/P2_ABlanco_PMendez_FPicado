//
// Created by Xpc on 10/5/2025.
//

#include "Tower.h"

Tower::Tower(int x, int y, char type, int damage, int speed, int range, int specialCooldown, int attackCooldown)
    : row(x), col(y), type(type), damage(damage), speed(speed), range(range),
      specialCooldown(specialCooldown), attackCooldown(attackCooldown) {}

int Tower::calculateGoldReward(char enemyType, int enemyCategory) const {
    int baseReward = 10;
    int categoryBonus = enemyCategory * 5;
    return baseReward + categoryBonus;
}


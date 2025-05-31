#include "Tower.h"

Tower::Tower(int x, int y, char type, int specialCooldown)
    : row(x), col(y), type(type), specialCooldown(specialCooldown) {

    switch (type) {
        case 'S': // Sniper
            range = 2;
        damage = 25;
        attackCooldown = 1.5f;
        break;
        case 'M': // Mage
            range = 3;
        damage = 20;
        attackCooldown = 1.0f;
        break;
        case 'A': // Archer
            range = 4;
        damage = 15;
        attackCooldown = 0.5f;
        break;
        default:
            range = 5;
        damage = 10;
        attackCooldown = 1;
    }
}

int Tower::calculateGoldReward(char enemyType, int enemyCategory) const {
    int base = 5;
    int typeBonus = (type == 'A') ? 2 : (type == 'M') ? 3 : 1;
    int categoryBonus = enemyCategory * 4;
    return base + typeBonus + categoryBonus;
}

bool Tower::upgrade(int& playerGold) {
    if (level >= 3) return false;
    if (playerGold < upgradeCost) return false;

    playerGold -= upgradeCost;
    level++;
    damage += 5;
    range += 1;
    attackCooldown -= 0.2f; // Reduce cooldown
    specialChance += 0.05f;
    upgradeCost += 15;

    return true;
}

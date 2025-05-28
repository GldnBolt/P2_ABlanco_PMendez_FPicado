#include "Tower.h"

Tower::Tower(int x, int y, char type, int specialCooldown)
    : row(x), col(y), type(type), specialCooldown(specialCooldown) {

    switch (type) {
        case 'S': // Sniper
            range = 3;
        damage = 15;
        attackCooldown = 1.5f;
        break;
        case 'M': // Mage
            range = 4;
        damage = 10;
        attackCooldown = 1.0f;
        break;
        case 'A': // Archer
            range = 5;
        damage = 5;
        attackCooldown = 0.8f;
        break;
        default:
            range = 5;
        damage = 5;
        attackCooldown = 2;
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
    attackCooldown = static_cast<int>(attackCooldown * 0.9f); // Reducción del 10%
    range += 1;
    specialChance += 0.05f;
    upgradeCost += 15;

    return true;
}

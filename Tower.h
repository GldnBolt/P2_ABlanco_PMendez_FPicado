#ifndef TOWER_H
#define TOWER_H

class Tower {
public:
    int row, col;
    char type;
    int cost = 10;

    int level = 1;         // Nivel actual (1–3)
    int damage;
    int range;
    float attackCooldown;
    int specialCooldown;
    int upgradeCost = 15;  // Costo del siguiente upgrade
    float specialChance = 0.1f; // Probabilidad de ataque especial
    float rotation = 0.f;       // Ángulo actual para visualización
    float lastAttackTime = 0.f; // Último ataque realizado


    Tower(int x, int y, char type, int specialCooldown);

    int calculateGoldReward(char enemyType, int enemyCategory) const;
    bool upgrade(int& playerGold); // ← nuevo método miembro

    int getCost() const {
        switch (type) {
            case 'A': return 10;  // Arqueros
            case 'M': return 15;  // Magos
            case 'S': return 20;  // Sniper
            default: return 10;
        }
    }

};

#endif

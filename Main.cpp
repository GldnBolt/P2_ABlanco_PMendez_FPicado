//
// Created by Xpc on 10/5/2025.
//

#include "Map.h"
#include "Enemy.h"
#include <iostream>

int main() {
    Map map(10, 10, 0, 0, 9, 9);
    Enemy enemy(0, 0, 100, 1, 'O');

    std::vector<std::pair<int, int>> path = map.findPath(enemy);

    if (!path.empty()) {
        std::cout << "Camino encontrado: \n";
        for (const auto& p : path) {
            std::cout << "(" << p.first << ", " << p.second << ") -> ";
        }
        std::cout << "Destino alcanzado.\n";
    } else {
        std::cout << "No se encontró un camino.\n";
    }

    return 0;
}

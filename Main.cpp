//
// Created by Xpc on 10/5/2025.
//

#include "Map.h"
#include "GameWindow.h"

int main() {
    Map map(15, 25, 0, 0, 14, 24);  // filas = 15, columnas = 25
    GameWindow game(map);
    game.run();
    return 0;
}


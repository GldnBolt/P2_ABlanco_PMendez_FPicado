//
// Created by Xpc on 10/5/2025.
//

#include "Map.h"
#include <iostream>
#include <queue>
#include <cmath>
#include <algorithm>

Map::Map(int rows, int cols, int entranceRow, int entranceCol, int bridgeRow, int bridgeCol)
    : rows(rows), cols(cols), entranceRow(entranceRow), entranceCol(entranceCol),
      bridgeRow(bridgeRow), bridgeCol(bridgeCol) {
    grid.resize(rows, std::vector<char>(cols, '.'));
    grid[entranceRow][entranceCol] = 'E';
    grid[bridgeRow][bridgeCol] = 'B';
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i != entranceRow && j != entranceCol && i != bridgeRow && j != bridgeCol) {
                grid[i][j] = '.'; // camino
            }
        }
    }
    std::cout << "Mapa inicializado\n";
}

char Map::getCell(int row, int col) const {
    return grid[row][col];
}

bool Map::placeTower(int row, int col, Tower tower) {
    if (row < 0 || row >= rows || col < 0 || col >= cols || grid[row][col] != '.') {
        std::cout << "No se puede colocar la torre en esta posición.\n";
        return false;
    }

    // Simular la torre antes de agregarla realmente
    grid[row][col] = tower.type;

    // Si bloquea el camino, revertimos
    if (!isPathToBridge()) {
        grid[row][col] = '.'; // Revertir la marca
        std::cout << "No se puede colocar la torre aquí, bloquearía el camino al puente.\n";
        return false;
    }

    // Ahora sí es válida, guardar torre
    tower.row = row;
    tower.col = col;
    towers.push_back(tower);
    return true;
}

bool Map::isPathToBridge() {
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    return dfs(entranceRow, entranceCol, visited);
}

void Map::printMap() const {
    for (const auto& row : grid) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
}

void Map::printTowers() const {
    for (const auto& tower : towers) {
        std::cout << "Torre en (" << tower.row << ", " << tower.col << ") - Tipo: " << tower.type << '\n';
    }
}

std::vector<std::pair<int, int>> Map::findPath(Enemy& enemy) {
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::vector<std::vector<bool>> closedList(rows, std::vector<bool>(cols, false));

    Node* startNode = new Node(enemy.x, enemy.y, 0, manhattanDistance(enemy.x, enemy.y, bridgeRow, bridgeCol));
    openList.push(*startNode);

    std::vector<std::pair<int, int>> directions = {{-1,0},{1,0},{0,-1},{0,1}};

    while (!openList.empty()) {
        Node* currentNode = new Node(openList.top().x, openList.top().y, openList.top().gCost,
                                     openList.top().hCost, openList.top().parent);
        openList.pop();

        if (currentNode->x == bridgeRow && currentNode->y == bridgeCol) {
            return reconstructPath(currentNode);
        }

        closedList[currentNode->x][currentNode->y] = true;

        for (const auto& dir : directions) {
            int newX = currentNode->x + dir.first;
            int newY = currentNode->y + dir.second;

            // Validar límites
            if (newX < 0 || newX >= rows || newY < 0 || newY >= cols)
                continue;

            // Verificar si ya se cerró o si no es transitable
            if (closedList[newX][newY])
                continue;

            char cell = grid[newX][newY];
            if (cell != '.' && cell != 'E' && cell != 'B')  // ← solo se puede caminar sobre estos
                continue;

            int newGCost = currentNode->gCost + 1;
            int newHCost = manhattanDistance(newX, newY, bridgeRow, bridgeCol);
            Node* neighbor = new Node(newX, newY, newGCost, newHCost, currentNode);
            openList.push(*neighbor);
        }
    }

    return {}; // no hay camino posible
}


int Map::manhattanDistance(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<std::pair<int, int>> Map::reconstructPath(Node* node) {
    std::vector<std::pair<int, int>> path;
    while (node != nullptr) {
        path.push_back({node->x, node->y});
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

bool Map::dfs(int row, int col, std::vector<std::vector<bool>>& visited) {
    // 1. Verifica que esté dentro del rango
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return false;

    // 2. Verifica si ya fue visitado
    if (visited[row][col])
        return false;

    // 3. Verifica si la celda es transitable (solo '.', 'E', 'B')
    char cell = grid[row][col];
    if (cell != '.' && cell != 'E' && cell != 'B')
        return false;

    // 4. Si llegó al puente
    if (row == bridgeRow && col == bridgeCol)
        return true;

    // 5. Marca como visitado
    visited[row][col] = true;

    // 6. Intenta moverse en las 4 direcciones
    return dfs(row + 1, col, visited) || dfs(row - 1, col, visited) ||
           dfs(row, col + 1, visited) || dfs(row, col - 1, visited);
}

std::vector<Tower>& Map::getTowers() {
    return towers;
}

void Map::clearTowers() {
    // Quita todas las torres del grid
    for (const Tower& t : towers) {
        if (grid[t.row][t.col] == t.type) {
            grid[t.row][t.col] = '.'; // celda vacía
        }
    }

    // Limpia la lista de torres
    towers.clear();
}





//
// Created by Xpc on 10/5/2025.
//

#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Tower.h"
#include "Enemy.h"
#include "Node.h"


class Map {
private:
    int rows, cols;
    std::vector<std::vector<char>> grid;
    int entranceRow, entranceCol;
    int bridgeRow, bridgeCol;
    std::vector<Tower> towers;
    int manhattanDistance(int x1, int y1, int x2, int y2);
    std::vector<std::pair<int, int>> reconstructPath(Node* node);
    bool dfs(int row, int col, std::vector<std::vector<bool>>& visited);

public:
    Map(int rows, int cols, int entranceRow, int entranceCol, int bridgeRow, int bridgeCol);
    char getCell(int row, int col) const;
    bool placeTower(int row, int col, Tower tower);
    bool isPathToBridge();
    void printMap() const;
    void printTowers() const;
    std::vector<std::pair<int, int>> findPath(Enemy& enemy);
    std::vector<Tower>& getTowers();


};


#endif


#include <iostream>
#include <vector>
class Tower {
public:
    int row, col; // Coordenadas de la torre
    char type; // Tipo de torre (por ejemplo, 'A' para Arquero, 'M' para Mago)
    int damage; // Daño que inflige la torre
    int speed; // Velocidad de ataque de la torre
    int range; // Alcance de ataque de la torre
    int specialCooldown; // Tiempo para regenerar el poder especial
    int attackCooldown; // Tiempo para recargar el ataque

    Tower(int x, int y, char type, int damage, int speed, int range, int specialCooldown, int attackCooldown)
        : row(x), col(y), type(type), damage(damage), speed(speed), range(range), specialCooldown(specialCooldown), attackCooldown(attackCooldown) {}

    // Calcula la recompensa en oro al derrotar a un enemigo
    int calculateGoldReward(char enemyType, int enemyCategory) const {
        int baseReward = 10; // Recompensa base por derrotar a un enemigo
        int categoryBonus = enemyCategory * 5; // Bono basado en la categoría del enemigo
        return baseReward + categoryBonus;
    }

};
class Map {
private:
    int rows, cols; 
    std::vector<std::vector<char>> grid; 
    int entranceRow, entranceCol; // Coordenadas del punto de ingreso de los enemigos
    int bridgeRow, bridgeCol; // Coordenadas del puente del castillo
    std::vector<Tower> towers; // Lista de torres colocadas en el mapa

public:

    Map(int rows, int cols, int entranceRow, int entranceCol, int bridgeRow, int bridgeCol)
        : rows(rows), cols(cols), entranceRow(entranceRow), entranceCol(entranceCol), bridgeRow(bridgeRow), bridgeCol(bridgeCol) {
        grid.resize(rows, std::vector<char>(cols, '.')); 
        grid[entranceRow][entranceCol] = 'E'; // Marca el punto de ingreso
        grid[bridgeRow][bridgeCol] = 'B'; // Marca el puente del castillo
    }

    // Coloca una torre en el mapa
    bool placeTower(int row, int col, Tower tower) {
        if (row < 0 || row >= rows || col < 0 || col >= cols || grid[row][col] != '.') {
            std::cout << "No se puede colocar la torre en esta posición.\n";
            return false; // No se puede colocar la torre
        }
        grid[row][col] = tower.type; // Marca la posición de la torre
        tower.row = row;
        tower.col = col;

        // Verifica si aún hay un camino hacia el puente después de colocar la torre
        if (!isPathToBridge()) {
            grid[row][col] = '.'; // Revertir la colocación de la torre
            std::cout << "No se puede colocar la torre aquí, bloquearía el camino al puente.\n";
            return false;
        }

        towers.push_back(tower); // Agrega la torre a la lista de torres
        return true;
    }

    // Verifica si hay un camino hacia el puente
    bool isPathToBridge() {
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        return dfs(entranceRow, entranceCol, visited);
    }

    // Imprime el mapa actual
    void printMap() const {
        for (const auto& row : grid) {
            for (char cell : row) {
                std::cout << cell << ' ';
            }
            std::cout << '\n';
        }
    }

    // Imprime las torres colocadas en el mapa
    void printTowers() const {
        for (const auto& tower : towers) {
            std::cout << "Torre en (" << tower.row << ", " << tower.col << ") - Tipo: " << tower.type << '\n';
        }
    }

private:
    // Realiza una búsqueda en profundidad (DFS) para verificar si hay un camino al puente
    bool dfs(int row, int col, std::vector<std::vector<bool>>& visited) {
        if (row < 0 || row >= rows || col < 0 || col >= cols || visited[row][col] || grid[row][col] == 'T') {
            return false;
        }
        if (row == bridgeRow && col == bridgeCol) {
            return true; // Se encontró un camino al puente
        }
        visited[row][col] = true;
        // Explora las cuatro direcciones
        return dfs(row + 1, col, visited) || dfs(row - 1, col, visited) ||
               dfs(row, col + 1, visited) || dfs(row, col - 1, visited);
    }
};
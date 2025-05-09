#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

// Clase Torre
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

// Estructura para representar un nodo en el algoritmo A*
struct Node {
    int x, y; // Coordenadas del nodo
    int gCost, hCost; // gCost (costo desde el inicio) y hCost (heurística estimada al objetivo)
    Node* parent; // Nodo padre

    Node(int x, int y, int gCost, int hCost, Node* parent = nullptr)
        : x(x), y(y), gCost(gCost), hCost(hCost), parent(parent) {}

    // Suma de gCost y hCost para obtener el fCost
    int fCost() const { return gCost + hCost; }

    // Comparador para ordenar nodos en la cola de prioridad (basado en fCost)
    bool operator>(const Node& other) const {
        return fCost() > other.fCost();
    }
};

// Clase Enemigo
class Enemy {
public:
    int x, y; // Posición actual del enemigo
    int health; // Salud del enemigo
    int speed; // Velocidad de movimiento
    char type; // Tipo de enemigo (por ejemplo, 'O' para ogro, 'E' para elfo oscuro)

    Enemy(int x, int y, int health, int speed, char type)
        : x(x), y(y), health(health), speed(speed), type(type) {}
};

// Clase Mapa
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

    // Método que ejecuta el algoritmo A* para encontrar el camino
    std::vector<std::pair<int, int>> findPath(Enemy& enemy) {
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
        std::vector<std::vector<bool>> closedList(rows, std::vector<bool>(cols, false));

        Node* startNode = new Node(enemy.x, enemy.y, 0, manhattanDistance(enemy.x, enemy.y, bridgeRow, bridgeCol));
        openList.push(*startNode);

        // Direcciones posibles: arriba, abajo, izquierda, derecha
        std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

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

                if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && grid[newX][newY] != 'T' && !closedList[newX][newY]) {
                    int newGCost = currentNode->gCost + 1;
                    int newHCost = manhattanDistance(newX, newY, bridgeRow, bridgeCol);
                    Node* neighbor = new Node(newX, newY, newGCost, newHCost, currentNode);
                    openList.push(*neighbor);
                }
            }
        }

        return {}; // Si no se encontró camino
    }

private:
    // Función heurística: calcula la distancia Manhattan
    int manhattanDistance(int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }

    // Reconstruye el camino desde el nodo final
    std::vector<std::pair<int, int>> reconstructPath(Node* node) {
        std::vector<std::pair<int, int>> path;
        while (node != nullptr) {
            path.push_back({node->x, node->y});
            node = node->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Realiza una búsqueda en profundidad (DFS) para verificar si hay un camino al puente
    bool dfs(int row, int col, std::vector<std::vector<bool>>& visited) {
        if (row < 0 || row >= rows || col < 0 || col >= cols || visited[row][col] || grid[row][col] == 'T') {
            return false;
        }
        if (row == bridgeRow && col == bridgeCol) {
            return true; // Se encontró un camino al puente
        }
        visited[row][col] = true;
        return dfs(row + 1, col, visited) || dfs(row - 1, col, visited) ||
               dfs(row, col + 1, visited) || dfs(row, col - 1, visited);
    }
};

// Función principal
int main() {
    Map map(10, 10, 0, 0, 9, 9); // Crear mapa de 10x10, entrada en (0,0) y puente en (9,9)
    Enemy enemy(0, 0, 100, 1, 'O'); // Crear enemigo en la entrada

    std::vector<std::pair<int, int>> path = map.findPath(enemy); // Encontrar el camino

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

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <iomanip>
#include <climits>

using namespace std;

// Функция для генерации матрицы смежности графа
vector<vector<int>> generateAdjMatrix(int n, bool isDirected) {
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
    srand(time(0)); // Инициализация генератора случайных чисел

    // Заполняем матрицу случайными весами рёбер (от 1 до 10)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && rand() % 2) { // Ребро существует с вероятностью 50%, не самосоединение
                int weight = rand() % 10 + 1; // Вес рёбер от 1 до 10
                adjMatrix[i][j] = weight;
                if (!isDirected) {
                    adjMatrix[j][i] = weight; // Для неориентированного графа делаем симметрию
                }
            }
        }
    }
    return adjMatrix;
}

// Функция для вывода матрицы
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            if (val == -1) {
                cout << std::setw(3) << "∞"; // Используем "∞" для недостижимых вершин
            }
            else {
                cout << std::setw(3) << val;
            }
        }
        cout << endl;
    }
}

// Алгоритм ПОШ для нахождения расстояний
vector<int> bfsDistances(const vector<vector<int>>& G, int v) {
    int n = G.size();                   // Количество вершин в графе
    vector<int> DIST(n, -1);            // Вектор расстояний, изначально -1 ("не посещено")
    queue<int> Q;                       // Очередь для обхода графа

    // Инициализация для начальной вершины
    Q.push(v);
    DIST[v] = 0;

    // Алгоритм поиска в ширину
    while (!Q.empty()) {
        int current = Q.front();        // Текущая вершина
        Q.pop();

        // Проверяем всех соседей текущей вершины
        for (int i = 0; i < n; i++) {
            if (G[current][i] > 0 && DIST[i] == -1) {  // Если ребро существует и вершина не посещена
                Q.push(i);
                DIST[i] = DIST[current] + G[current][i]; // Обновляем расстояние
            }
        }
    }

    return DIST;
}

// Функция для расчета радиуса, диаметра, центральных и периферийных вершин
void calculateGraphProperties(const vector<vector<int>>& distanceMatrix) {
    int n = distanceMatrix.size();
    vector<int> eccentricities(n, 0); // Вектор эксцентриситетов

    // Находим эксцентриситеты
    for (int i = 0; i < n; i++) {
        int maxDistance = 0;
        for (int j = 0; j < n; j++) {
            if (distanceMatrix[i][j] != -1) { // Если вершина достижима
                maxDistance = max(maxDistance, distanceMatrix[i][j]);
            }
        }
        eccentricities[i] = maxDistance;
    }

    // Радиус и диаметр графа
    int radius = INT_MAX, diameter = 0;
    for (int ecc : eccentricities) {
        radius = min(radius, ecc);
        diameter = max(diameter, ecc);
    }

    // Поиск центральных и периферийных вершин
    vector<int> centralVertices, peripheralVertices;
    for (int i = 0; i < n; i++) {
        if (eccentricities[i] == radius) {
            centralVertices.push_back(i);
        }
        if (eccentricities[i] == diameter) {
            peripheralVertices.push_back(i);
        }
    }

    // Вывод результатов
    cout << "\nРадиус графа (r(G)): " << radius << endl;
    cout << "Диаметр графа (D(G)): " << diameter << endl;

    cout << "Центральные вершины: ";
    for (int v : centralVertices) cout << v + 1 << " ";
    cout << endl;

    cout << "Периферийные вершины: ";
    for (int v : peripheralVertices) cout << v + 1 << " ";
    cout << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n;
    char choice;

    // Ввод количества вершин графа
    cout << "Введите количество вершин в графе: ";
    cin >> n;

    // Выбор типа графа
    cout << "Граф ориентированный? (y/n): ";
    cin >> choice;
    bool isDirected = (choice == 'y' || choice == 'Y');

    // Генерация случайной матрицы смежности
    vector<vector<int>> adjMatrix = generateAdjMatrix(n, isDirected);

    // Вывод сгенерированной матрицы смежности
    cout << "Сгенерированная матрица смежности:\n";
    printMatrix(adjMatrix);

    // Матрица расстояний
    vector<vector<int>> distanceMatrix(n, vector<int>(n, -1));

    // Вычисление расстояний от каждой вершины до всех остальных
    for (int i = 0; i < n; i++) {
        distanceMatrix[i] = bfsDistances(adjMatrix, i);
    }

    // Вывод матрицы расстояний
    cout << "\nМатрица расстояний от каждой вершины до каждой:\n";
    printMatrix(distanceMatrix);

    // Расчет радиуса, диаметра и характеристик вершин
    calculateGraphProperties(distanceMatrix);

    return 0;
}
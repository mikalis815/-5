#include <iostream>
#include <vector>
#include <climits>   // Для INT_MAX
#include <cstring>   // Для memset (альтернатива)

using namespace std;

class TSP {
private:
    int n; // Количество городов
    vector<vector<int>> distances; // Матрица расстояний
    vector<vector<int>> memo; // Таблица мемоизации

    /**
     * Рекурсивная функция для решения TSP
     * @param current - текущий город
     * @param mask - битовая маска посещенных городов
     * @return минимальная стоимость маршрута из текущего состояния
     */
    int tsp(int current, int mask) {
        // БАЗОВЫЙ СЛУЧАЙ: все города посещены
        // (1 << n) - 1 создает битовую маску из n единиц
        if (mask == (1 << n) - 1) {
            // Возвращаемся в город 0 из текущего города
            return distances[current][0];
        }
        
        // ПРОВЕРКА МЕМОИЗАЦИИ: если результат уже вычислен, возвращаем его
        if (memo[current][mask] != -1) {
            return memo[current][mask];
        }
        
        // Инициализируем минимальную стоимость максимальным значением
        int minCost = INT_MAX;
        
        // ПЕРЕБОР всех городов как кандидатов на следующий визит
        for (int next = 0; next < n; next++) {
            // Проверяем, не посещен ли город 'next'
            // (mask & (1 << next)) == 0 означает, что бит 'next' не установлен
            if ((mask & (1 << next)) == 0) {
                // Создаем новую маску, устанавливая бит 'next'
                int newMask = mask | (1 << next);
                
                // Вычисляем общую стоимость:
                // стоимость до next + минимальная стоимость из next
                int cost = distances[current][next] + tsp(next, newMask);
                
                // Обновляем минимум, если нашли лучший вариант
                if (cost < minCost) {
                    minCost = cost;
                }
            }
        }
        
        // Сохраняем вычисленный результат перед возвратом
        memo[current][mask] = minCost;
        return minCost;
    }

public:
    /**
     * Публичный метод для поиска минимальной стоимости маршрута
     * @param dist - матрица расстояний между городами
     * @return минимальная длина маршрута
     */
    int findMinCost(vector<vector<int>>& dist) {
        n = dist.size();
        distances = dist;
        
        // Инициализация таблицы мемоизации:
        // n строк (городов) и 2^n столбцов (возможных масок)
        memo.resize(n, vector<int>(1 << n, -1));
        
        // Начинаем рекурсию:
        // - Из города 0
        // - С маской 1 (0001 в двоичной - посетили только город 0)
        return tsp(0, 1);
    }
};

// Демонстрация работы программы
int main() {
    TSP solver;
    
    // Создаем матрицу расстояний 4x4
    // distances[i][j] = расстояние от города i до города j
    vector<vector<int>> distances = {
        // Из города 0 в города: 0,  1,  2,  3
        {0, 10, 15, 20},   // Расстояния из города 0
        {10, 0, 35, 25},   // Расстояния из города 1
        {15, 35, 0, 30},   // Расстояния из города 2
        {20, 25, 30, 0}    // Расстояния из города 3
    };
    
    cout << "Матрица расстояний:" << endl;
    for (int i = 0; i < distances.size(); i++) {
        cout << "Город " << i << ": ";
        for (int j = 0; j < distances[i].size(); j++) {
            cout << distances[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    
    // Вычисляем минимальную стоимость маршрута
    int result = solver.findMinCost(distances);
    
    cout << "Результат вычислений:" << endl;
    cout << "Минимальная длина маршрута: " << result << endl;
    cout << "Ожидаемый результат: 80" << endl;
    cout << "Оптимальный маршрут: 0 → 1 → 3 → 2 → 0" << endl;
    cout << "Проверка: 10 (0→1) + 25 (1→3) + 30 (3→2) + 15 (2→0) = 80" << endl;
    
    return 0;
}
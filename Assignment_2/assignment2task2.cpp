// Assignment 2 task 2: Работа с массивами и OpenMP
// Реализуйте программу на C++, которая:
// 1. Создаёт массив из 10 000 случайных чисел.
// 2. Находит минимальное и максимальное значения массива:
//          - в последовательной реализации;
//          - с использованием OpenMP для параллельной обработки.
// 3. Сравнивает время выполнения обеих реализаций и формулирует выводы.

#include <iostream>      // Для работы с вводом/выводом (cout, cin, endl)
#include <vector>        // Для динамического массива vector
#include <random>        // Для генерации случайных чисел
#include <omp.h>         // Для работы с OpenMP
#include <chrono>        // Для измерения времени выполнения

using namespace std;     // Используем стандартное пространство имен

int main() {
    const int SIZE = 10000;             // Размер массива
    vector<int> arr(SIZE);              // Создание динамического массива из SIZE элементов

    
    // Настройка генератора случайных чисел
    random_device rd;                  // random_device используется как источник случайности
    mt19937 gen(rd());                 // mt19937 — генератор случайных чисел Mersenne Twister
                                       // Инициализируем его значением из rd(), чтобы последовательность была случайной
    
    uniform_int_distribution<int> dist(0, 9999);      // uniform_int_distribution задаёт равномерное распределение чисел от 0 до 9999 (можно изменить)
                                                      // dist(gen) будет возвращать случайное число из этого диапазона

    // Заполнение массива случайными числами
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = dist(gen);                           // Генерация случайного числа и запись в массив
    }

    // Последовательная реализация поиска min и max
    auto start_seq = chrono::high_resolution_clock::now(); // Начало замера времени

    int min_val = arr[0];               // Инициализация минимума первым элементом
    int max_val = arr[0];               // Инициализация максимума первым элементом
    for (int i = 1; i < SIZE; ++i) {   // Проход по всему массиву
        if (arr[i] < min_val) min_val = arr[i]; // Обновление минимума
        if (arr[i] > max_val) max_val = arr[i]; // Обновление максимума
    }

    auto end_seq = chrono::high_resolution_clock::now();         // Конец замера времени
    chrono::duration<double> duration_seq = end_seq - start_seq; // Вычисляем длительность

    cout << "Последовательная реализация:\n";                    // Вывод результата
    cout << "Минимум: " << min_val << ", Максимум: " << max_val << "\n"; // Минимум и максимум
    cout << "Время: " << duration_seq.count() << " секунд\n\n";  // Время выполнения


    // Параллельная реализация с OpenMP
    auto start_par = chrono::high_resolution_clock::now(); // Начала замера времени

    int min_val_par = arr[0];            // Инициализация минимума для параллельной версии
    int max_val_par = arr[0];            // Инициализация максимума для параллельной версии

    #pragma omp parallel for reduction(min:min_val_par) reduction(max:max_val_par) // Параллельный цикл с редукцией
    for (int i = 1; i < SIZE; ++i) {                                               // Каждый поток обрабатывает часть массива
        if (arr[i] < min_val_par) min_val_par = arr[i];                            // Локальный минимум обновляется
        if (arr[i] > max_val_par) max_val_par = arr[i];                            // Локальный максимум обновляется
    }

    auto end_par = chrono::high_resolution_clock::now();                           // Конец замера времени
    chrono::duration<double> duration_par = end_par - start_par;                   // Вычисляем длительность

    cout << "Параллельная реализация (OpenMP):\n";                                 // Вывод результата
    cout << "Минимум: " << min_val_par << ", Максимум: " << max_val_par << "\n";   // Минимум и максимум
    cout << "Время: " << duration_par.count() << " секунд\n\n";                    // Время выполнения


    // Сравнение времени выполнения
    cout << "Вывод:\n";                                     // Заголовок вывода
    if (duration_par.count() < duration_seq.count()) {      // Если параллельная быстрее
        cout << "Параллельная реализация быстрее последовательной.\n"; 
    } else {
        cout << "Последовательная реализация быстрее или накладные расходы OpenMP больше.\n"; 
    }

    return 0;                                               // Завершение программы
}


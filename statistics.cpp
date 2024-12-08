#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>   
#include <chrono> 
# include <fstream>

#include "statistics.h"
#include "alloc.h"
using namespace std;


//– void statistics1_malloc_function(void* (*malloc_func)(size_t), size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize)
//– сбор статистических данных первого режима со случайной генерацией размеров блоков памяти в пользовательских границах для разного количества итераций;
//– void statistics2_malloc_function(void* (*malloc_func)(size_t), string method, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize) 
//– сбор статистических данных второго режима с разными способами генерации размеров блоков памяти в пользовательских границах для разного количества итераций;


void statistics1MallocFunctionMethods(void* (*malloc_func)(size_t), string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize) {
    srand(static_cast<unsigned int>(time(0)));
    for (size_t current = minIterations; current <= maxIterations; current += stepIterations)
    {
        vector<void*> pointers; // Вектор для хранения указателей
        auto start = chrono::high_resolution_clock::now(); // Старт времени для числа элементов current
        pointers.push_back(malloc_func(rand() % (maxSize - minSize + 1) + minSize)); // Добавление одного элемента
        int total = 1;
        while (true) {
            // Добавление случайного количества элементов
            if (current != total) {
                int addCount = rand() % (current - total) + 1; // Случайное число от 1 до остаточного количества элементов
                total += addCount;
                for (int i = 0; i < addCount; ++i) {
                    pointers.push_back(malloc_func(rand() % (maxSize - minSize + 1) + minSize));
                }
            }
            // Удаление случайного количества элементов
            int removeCount = rand() % (pointers.size()+1); // Случайное число от 0 до размера вектора
            for (int i = 0; i < removeCount; ++i) {
                int indexToRemove = rand() % pointers.size(); // Случайный индекс для удаления
                freeAlloc(pointers[indexToRemove]);
                pointers.erase(pointers.begin() + indexToRemove);
            }
            if (total == current && pointers.size() == 0) break; // Выход из цикла, если нужное количество элементов добавлено и вектор пуст
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;

        // Запись результатов в файл
        ofstream resFile("DefaultAndOtherStat.txt", ios::app); // Открытие файла в режиме добавления
        cout << pointers.size();
        resFile << nameMalloc << " " << current << " " << diff.count() << " " << osMemoryRequests << endl; // Запись строки
        resFile.close();
    }
}

void statistics1MallocFunctionDefault(string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize) {
    for (size_t current = minIterations; current <= maxIterations; current += stepIterations)
    {
        srand(static_cast<unsigned int>(time(0)));
        vector<int> pointers; // Вектор для хранения указателей
        auto start = chrono::high_resolution_clock::now(); // Старт времени для числа элементов current
        pointers.push_back(rand() % (maxSize - minSize + 1) + minSize); // Добавление одного элемента
        int total = 1;

        while (true) {
            if (current != total) {
                // Добавление случайного количества элементов
                int addCount = rand() % (current - total) + 1;; // Случайное число от 0 до остаточного количества элементов
                total += addCount;
                for (int i = 0; i < addCount; ++i) {
                    pointers.push_back(rand() % (maxSize - minSize + 1) + minSize);
                }
            }

            // Удаление случайного количества элементов
            int removeCount = rand() % (pointers.size() + 1); // Случайное число от 0 до размера вектора
            for (int i = 0; i < removeCount; ++i) {
                int indexToRemove = rand() % pointers.size(); // Случайный индекс для удаления
                pointers.erase(pointers.begin() + indexToRemove);
            }
            if (total == current && pointers.size() == 0) break; // Выход из цикла, если нужное количество элементов добавлено и вектор пуст
        }
        pointers.clear();

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;

        // Запись результатов в файл
        ofstream resFile("DefaultAndOtherStat.txt", ios::app); // Открытие файла в режиме добавления
        resFile << nameMalloc << " " << current << " " << diff.count() << endl; // Запись строки
        resFile.close();
    }
}

//int main() {
//    srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел
//    std::vector<T> numbers;
//
//    // Добавляем первый элемент
//    addElement(numbers);
//
//    // Пропускаем случайное количество добавлений (от 1 до 5)
//    int skipCount = rand() % 5 + 1; // Случайное число от 1 до 5
//    std::cout << "Пропускаем " << skipCount << " добавлений." << std::endl;
//
//    for (int i = 0; i < skipCount; ++i) {
//        addElement(numbers); // Добавляем элементы, которые мы пропускаем
//        std::this_thread::sleep_for(std::chrono::seconds(1)); // Задержка 1 секунда между добавлениями
//    }
//
//    // Удаляем случайный элемент из уже добавленных
//    removeRandomElement(numbers);
//
//    // Удаляем все оставшиеся элементы
//    numbers.clear(); // Очищаем вектор
//    std::cout << "Все элементы удалены." << std::endl;
//
//    return 0;
//}
//
//void test_malloc_function(void* (*malloc_func)(size_t), const char* name, size_t iterations, size_t size) {
//    vector<void*> pointers;
//    auto start = chrono::high_resolution_clock::now();
//
//    for (size_t i = 0; i < iterations; ++i) {
//        pointers.push_back(malloc_func(size));
//    }
//
//    for (size_t i = 0; i < iterations; ++i) {
//        freeAlloc(pointers[i]);
//        /*free(pointers[i]);*/
//    }
//
//    auto end = chrono::high_resolution_clock::now();
//    chrono::duration<double> diff = end - start;
//
//    cout << name << "Время работы " << diff.count() << " секунд." << endl;
//}
//// удалять и освобождать рандомно 
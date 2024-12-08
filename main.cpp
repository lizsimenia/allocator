#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>

#include "alloc.h"
#include "statistics.h"
#include <clocale>
using namespace std;

void test_malloc_function(void* (*malloc_func)(size_t), const char* name, size_t iterations, size_t size) {
    vector<void*> pointers;
    auto start = chrono::high_resolution_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        pointers.push_back(malloc_func(size));
    }

    for (size_t i = 0; i < iterations; ++i) {
        freeAlloc(pointers[i]);
        /*free(pointers[i]);*/
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;

    cout << name << "Время работы " << diff.count() << " секунд." << endl;
}
// удалять и освобождать рандомно 


enum Mode {
    START,
    STATISTICS_MODE1,
    STATISTICS_MODE2,
    EXIT
};

// Пункты выбора генерации размеров блоков памяти
enum ElemsVector {
    START1,
    RANDOM,
    ORDERED,
    REVERSE,  
    SPECEQUAL
};

enum MallocMethod {
    START2,
    NEW,
    SPECMALLOC
};

enum specMallocMethod {
    START3,
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

// Меню выбора генерации значений памяти
int getMenuGenerateBytes() {
    int answer;
    do {
        cout << endl << "Выберите один из способ генерации размера блока памяти:" << endl
            << RANDOM << " – случайные значения" << endl
            << ORDERED << " – упорядоченные значения (от минимального к максимальному)" << endl
            << REVERSE << " – упорядоченные значения в обратном порядке (от максимального к минимальному)" << endl
            << SPECEQUAL << " – одинаковые блоки пользовательского размера" << endl;
        cout << "Введите номер одного из способов: ";
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Пожалуйста, введите число." << endl;
        }
        cout << endl;
    } while (answer <= START || answer > SPECEQUAL);
    return answer;
}
// Меню выбора метода по управлению памяти
int getMenuMallocMethod() {
    int answer;
    do {
        cout << endl << "Выберите один из способ выделения и удаления памяти:" << endl
            << NEW << " – стандартный оператор new" << endl
            << SPECMALLOC << " – распределитель памяти" << endl;
        cout << "Введите номер одного из способов: ";
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Пожалуйста, введите число." << endl;
        }
        cout << endl;
    } while (answer <= START || answer > SPECMALLOC);
    return answer;
}
// Меню выбора метода нахождения места
int getMenuFit() {
    int answer;
    do {
        cout << endl << "Выберите один из методов нахождения места для каждого нового блока:" << endl
            << FIRST_FIT << " – первый подходящий" << endl
            << BEST_FIT << " – наилучший подходящий" << endl
            << WORST_FIT << " – наихудший подходящий" << endl;
        cout << "Введите номер одного из методов: ";
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Пожалуйста, введите число." << endl;
        }
        cout << endl;
    } while (answer <= START || answer > WORST_FIT);
    return answer;
};
// Главное меню
int getMenuItem() {
    int answer;
    do {
        cout << endl << "Выберите один из пунктов меню:" << endl
            << STATISTICS_MODE1 << " – накопление данных для сравнения со стандартным оператором new" << endl
            << STATISTICS_MODE2 << " – накопление данных для сравнения методов нахождения места в памяти" << endl
            << EXIT << " - выйти из программы." << endl << endl;
        cout << "Введите номер пункта меню: ";
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Пожалуйста, введите число." << endl;
        }
        cout << endl;
    } while (answer <= START || answer > EXIT);
    return answer;
};

//int checkCinNum() {
//
//}

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "-- Исследование распределителя памяти --" << endl;
    setlocale(LC_ALL, "Russian");

    int answer;
    do {
        answer = getMenuItem();
        cout << endl;

        switch (answer)
        {
        case STATISTICS_MODE1: {
            answer = getMenuMallocMethod();
            int minMem, maxMem;
            while (true) {
                cout << "Введите минимальную границу случайного значения размера блока: ";
                cin >> minMem;
                cout << "Введите максимальную границу случайного значения размера блока: ";
                cin >> maxMem;
                if (maxMem > minMem) break;
                else if (minMem > maxMem) cerr << "Минимальная граница должна быть меньше максимальной." << endl;
                else cerr << "Границы не могут совпадать." << endl;
            }

            cout << endl;
            int minIt, maxIt, stepIt;
            while (true) {
                cout << "Введите минимальное количество итераций с памятью: ";
                cin >> minIt;
                cout << "Введите максимальное количество итераций с памятью: ";
                cin >> maxIt;
                cout << "Введите шаг изменения количества итераций: ";
                cin >> stepIt;

                if (maxIt > minIt) break;
                else if (minIt > maxIt) cerr << "Минимальная граница должна быть меньше максимальной." << endl;
                else cerr << "Границы не могут совпадать." << endl;
            }
            if (answer == 1) statistics1MallocFunctionDefault("new", minIt, maxIt, stepIt, minMem, maxMem);
            else {
                statistics1MallocFunctionMethods(firstFitMalloc, "firstFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
                statistics1MallocFunctionMethods(bestFitMalloc, "bestFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
                statistics1MallocFunctionMethods(worstFitMalloc, "worstFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
            }
            break;


        }
        case STATISTICS_MODE2:{
            int fit = getMenuFit();
            int generate = getMenuGenerateBytes();
            if (generate == 1) {
                int minMem, maxMem;
                while (true) {
                    cout << "Введите минимальную границу случайного значения: ";
                    cin >> minMem;
                    cout << "Введите максимальную границу случайного значения: ";
                    cin >> maxMem;
                    if (maxMem > minMem) break;
                    else if (minMem > maxMem) cerr << "Минимальная граница должна быть меньше максимальной." << endl;
                    else cerr << "Границы не могут совпадать." << endl;
                }
                cout << endl;
            }
            else if(generate == 4) {
                int size;
                cout << "Введите размер блока памяти: ";
                cin >> size;
            }

            int minIt, maxIt, stepIt;
            while (true) {
                cout << "Введите минимальное количество итераций с памятью: ";
                cin >> minIt;
                cout << "Введите максимальное количество итераций с памятью: ";
                cin >> maxIt;
                cout << "Введите шаг изменения количества итераций: ";
                cin >> stepIt;

                if (maxIt > minIt) break;
                else if (minIt > maxIt) cerr << "Минимальная граница должна быть меньше максимальной." << endl;
                else cerr << "Границы не могут совпадать." << endl;
            }


            break;
        }
        default:
        {
            cout << "" << endl;
        }
        }
    } while (answer != EXIT);
    return 0;
}
   // osMemoryRequests = 0;

   // size_t iterations = 10000;
   // size_t size = 10; // 1KB block size

   // cout << "Testing with block size: " << size << " bytes and " << iterations << " iterations." << std::endl;

   ///* test_malloc_function(malloc, "Standard malloc ", iterations, size);*/
   // test_malloc_function(firstFitMalloc, "First Fit malloc ", iterations, size);
   // cout << osMemoryRequests;




//    return 0;
//}

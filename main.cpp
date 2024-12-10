#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <clocale>
#include <cstdlib> 
#include <string>


#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable:4996)

#include "alloc.h"
#include "statistics.h"
using namespace std;

// Пунты выбора главного меню
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
// Пункты выбора распределителя памяти
enum MallocMethod {
    START2,
    MALLOC,
    SPECMALLOC
};
// Пункты выбора метода нахождения блока памяти
enum specMallocMethod {
    START3,
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

// Проверка на ввод положительного числа
int getNum() {
    int num;
    while (true) { // Бесконечный цикл для повторного запроса ввода
        cout << "\nВведите число: ";
        if (cin >> num) { // Если ввод успешен
            if (num > 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшихся символов
                return num;
            }
            else cerr << "Введите положительное число." << endl;
        }
        else {
            cin.clear(); // Очищаем состояние потока
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Пожалуйста, введите число." << endl;
        }
    }
}
// Ввод и проверка границ для случайной генрации
void getBordersRandom(int& minim, int& maxim) {
    while (true) {
        cout << "\nВведите минимальную границу случайного значения размера блока.";
        minim = getNum();
        cout << "Введите максимальную границу случайного значения размера блока.";
        maxim = getNum();
        if (maxim > minim) break;
        else if (minim > maxim) cerr << "Минимальная граница должна быть меньше максимальной.";
        else cerr << "Границы не могут совпадать.";
    }
    return;
}
// Ввод и проверка границ количества блоков памяти (количество выделения = удаления)
void getBordersStepIter(int& minim, int& maxim, int& step) {
    while (true) {
        cout << "\nВведите минимальное количество итераций с памятью.";
        minim = getNum();
        cout << "Введите максимальное количество итераций с памятью.";
        maxim = getNum();
        if (maxim > minim) break;
        else if (minim > maxim) cerr << "Минимальная граница должна быть меньше максимальной.";
        else cerr << "Границы не могут совпадать.";
    }
    while (true) {
        cout << "\nВведите шаг изменения количества итераций: ";
        step = getNum();
        if (maxim - minim < step) cerr << "Шаг не может быть больше количества итераций.";
        else break;
    }
}

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
        answer = getNum();
    } while (answer <= START || answer > SPECEQUAL);
    return answer;
}
// Меню выбора метода по управлению памяти
int getMenuMallocMethod() {
    int answer;
    do {
        cout << endl << "Выберите один из способ выделения и удаления памяти:" << endl
            << MALLOC << " – стандартная функция malloc" << endl
            << SPECMALLOC << " – распределитель памяти" << endl;
        cout << "Введите номер одного из способов: ";
        answer = getNum();
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
        answer = getNum();
    } while (answer <= START || answer > WORST_FIT);
    return answer;
};
// Главное меню
int getMenuItem() {
    int answer;
    do {
        cout << endl << "Выберите один из пунктов меню:" << endl
            << STATISTICS_MODE1 << " – накопление данных для сравнения со стандартной функцией malloc" << endl
            << STATISTICS_MODE2 << " – накопление данных для сравнения методов нахождения места в памяти" << endl
            << EXIT << " - выйти из программы." << endl << endl;
        cout << "Введите номер пункта меню: ";
        answer = getNum();
    } while (answer <= START || answer > EXIT);
    return answer;
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    cout << "-- Исследование распределителя памяти --" << endl;
    int answerMenu, answer;
    do {
        answerMenu = getMenuItem();
        cout << endl;
        switch (answerMenu)
        {
        // Первый режим сбора данных для сравнительного анализа стандартного malloc и разработанного аллокатора
        case STATISTICS_MODE1: {
            answer = getMenuMallocMethod();

            // Ввод и проверка границ случайного размера блока
            int minMem, maxMem;
            getBordersRandom(minMem, maxMem);
            cout << endl;

            // Ввод и проверка шага и границ количества выделения и удаления блоков из памяти
            int minIt, maxIt, stepIt;
            getBordersStepIter(minIt, maxIt, stepIt);
            cout << endl;

            // Выполнение сбора данных. Для malloc и разработанного распределителя памяти разные функции.
            if (answer == 1) statistics1MallocFunctionDefault("malloc", minIt, maxIt, stepIt, minMem, maxMem);
            else {
                answer = getMenuFit();
                if (answer == 1) statistics1MallocFunctionMethods(firstFitMalloc, "firstFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
                else if (answer == 2) statistics1MallocFunctionMethods(bestFitMalloc, "bestFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
                else statistics1MallocFunctionMethods(worstFitMalloc, "worstFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
            }
            std::string command1 = argv[0]; // Получаем имя текущей программы
            std::string command = "\"" + command1 + "\"";
            int result = system(command.c_str());
            exit(0); // Выход из текущего экземпляра
            break;
        }
        // Второй режим сбора данных для сравнительного анализа распределителей памяти в зависимости от выбора блока
        case STATISTICS_MODE2:{
            string generated; // Название способа генерации 
            int fit = getMenuFit(); // Номер способа выбора блока памяти
            int generate = getMenuGenerateBytes(); // Номер способа генерации
            int minMem = 0, maxMem = 0; // Памраметры для случайной генерации
            int size = 0; // Параметр для одинаковых блоков

            // В зависимости от генерации сбор дополнительных параметров
            if (generate == 1) {
                generated = "random";
                getBordersRandom(minMem, maxMem);
                cout << endl;
            }
            else if (generate == 2) generated = "ordered";
            else if (generate == 3) generated = "reverse";
            else if(generate == 4) {
                generated = "equal";
                cout << "Введите размер блока памяти.";
                size = getNum();
            }

            // Ввод и проверка шага и границ количества выделения и удаления блоков из памяти
            int minIt, maxIt, stepIt;
            getBordersStepIter(minIt, maxIt, stepIt);
            cout << endl;

            // Выполнение сбора данных
            if (fit == 1) statistics2MallocFunctionMethods(firstFitMalloc, "firstFitMalloc", generated, minIt, maxIt, stepIt, minMem, maxMem, size);
            else if (fit == 2) statistics2MallocFunctionMethods(bestFitMalloc, "bestFitMalloc", generated, minIt, maxIt, stepIt, minMem, maxMem, size);
            else if (fit == 3) statistics2MallocFunctionMethods(firstFitMalloc, "worstFitMalloc", generated, minIt, maxIt, stepIt, minMem, maxMem, size);

            std::string command1 = argv[0]; // Получаем имя текущей программы
            std::string command = "\"" + command1 + "\"";
            int result = system(command.c_str());
            exit(0); // Выход из текущего экземпляра

            break;
        }
        default:
        {
            cout << "" << endl;
        }
        }
    } while (answerMenu != EXIT);
    return 0;
}

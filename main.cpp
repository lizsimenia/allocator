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

    cout << name << "����� ������ " << diff.count() << " ������." << endl;
}
// ������� � ����������� �������� 


enum Mode {
    START,
    STATISTICS_MODE1,
    STATISTICS_MODE2,
    EXIT
};

// ������ ������ ��������� �������� ������ ������
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

// ���� ������ ��������� �������� ������
int getMenuGenerateBytes() {
    int answer;
    do {
        cout << endl << "�������� ���� �� ������ ��������� ������� ����� ������:" << endl
            << RANDOM << " � ��������� ��������" << endl
            << ORDERED << " � ������������� �������� (�� ������������ � �������������)" << endl
            << REVERSE << " � ������������� �������� � �������� ������� (�� ������������� � ������������)" << endl
            << SPECEQUAL << " � ���������� ����� ����������������� �������" << endl;
        cout << "������� ����� ������ �� ��������: ";
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "������������ ����. ����������, ������� �����." << endl;
        }
        cout << endl;
    } while (answer <= START || answer > SPECEQUAL);
    return answer;
}
// ���� ������ ������ �� ���������� ������
int getMenuMallocMethod() {
    int answer;
    do {
        cout << endl << "�������� ���� �� ������ ��������� � �������� ������:" << endl
            << NEW << " � ����������� �������� new" << endl
            << SPECMALLOC << " � �������������� ������" << endl;
        cout << "������� ����� ������ �� ��������: ";
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "������������ ����. ����������, ������� �����." << endl;
        }
        cout << endl;
    } while (answer <= START || answer > SPECMALLOC);
    return answer;
}
// ���� ������ ������ ���������� �����
int getMenuFit() {
    int answer;
    do {
        cout << endl << "�������� ���� �� ������� ���������� ����� ��� ������� ������ �����:" << endl
            << FIRST_FIT << " � ������ ����������" << endl
            << BEST_FIT << " � ��������� ����������" << endl
            << WORST_FIT << " � ��������� ����������" << endl;
        cout << "������� ����� ������ �� �������: ";
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "������������ ����. ����������, ������� �����." << endl;
        }
        cout << endl;
    } while (answer <= START || answer > WORST_FIT);
    return answer;
};
// ������� ����
int getMenuItem() {
    int answer;
    do {
        cout << endl << "�������� ���� �� ������� ����:" << endl
            << STATISTICS_MODE1 << " � ���������� ������ ��� ��������� �� ����������� ���������� new" << endl
            << STATISTICS_MODE2 << " � ���������� ������ ��� ��������� ������� ���������� ����� � ������" << endl
            << EXIT << " - ����� �� ���������." << endl << endl;
        cout << "������� ����� ������ ����: ";
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "������������ ����. ����������, ������� �����." << endl;
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
    cout << "-- ������������ �������������� ������ --" << endl;
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
                cout << "������� ����������� ������� ���������� �������� ������� �����: ";
                cin >> minMem;
                cout << "������� ������������ ������� ���������� �������� ������� �����: ";
                cin >> maxMem;
                if (maxMem > minMem) break;
                else if (minMem > maxMem) cerr << "����������� ������� ������ ���� ������ ������������." << endl;
                else cerr << "������� �� ����� ���������." << endl;
            }

            cout << endl;
            int minIt, maxIt, stepIt;
            while (true) {
                cout << "������� ����������� ���������� �������� � �������: ";
                cin >> minIt;
                cout << "������� ������������ ���������� �������� � �������: ";
                cin >> maxIt;
                cout << "������� ��� ��������� ���������� ��������: ";
                cin >> stepIt;

                if (maxIt > minIt) break;
                else if (minIt > maxIt) cerr << "����������� ������� ������ ���� ������ ������������." << endl;
                else cerr << "������� �� ����� ���������." << endl;
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
                    cout << "������� ����������� ������� ���������� ��������: ";
                    cin >> minMem;
                    cout << "������� ������������ ������� ���������� ��������: ";
                    cin >> maxMem;
                    if (maxMem > minMem) break;
                    else if (minMem > maxMem) cerr << "����������� ������� ������ ���� ������ ������������." << endl;
                    else cerr << "������� �� ����� ���������." << endl;
                }
                cout << endl;
            }
            else if(generate == 4) {
                int size;
                cout << "������� ������ ����� ������: ";
                cin >> size;
            }

            int minIt, maxIt, stepIt;
            while (true) {
                cout << "������� ����������� ���������� �������� � �������: ";
                cin >> minIt;
                cout << "������� ������������ ���������� �������� � �������: ";
                cin >> maxIt;
                cout << "������� ��� ��������� ���������� ��������: ";
                cin >> stepIt;

                if (maxIt > minIt) break;
                else if (minIt > maxIt) cerr << "����������� ������� ������ ���� ������ ������������." << endl;
                else cerr << "������� �� ����� ���������." << endl;
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

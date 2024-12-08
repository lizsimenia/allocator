#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>   
#include <chrono> 
# include <fstream>

#include "statistics.h"
#include "alloc.h"
using namespace std;


//� void statistics1_malloc_function(void* (*malloc_func)(size_t), size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize)
//� ���� �������������� ������ ������� ������ �� ��������� ���������� �������� ������ ������ � ���������������� �������� ��� ������� ���������� ��������;
//� void statistics2_malloc_function(void* (*malloc_func)(size_t), string method, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize) 
//� ���� �������������� ������ ������� ������ � ������� ��������� ��������� �������� ������ ������ � ���������������� �������� ��� ������� ���������� ��������;


void statistics1MallocFunctionMethods(void* (*malloc_func)(size_t), string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize) {
    srand(static_cast<unsigned int>(time(0)));
    for (size_t current = minIterations; current <= maxIterations; current += stepIterations)
    {
        vector<void*> pointers; // ������ ��� �������� ����������
        auto start = chrono::high_resolution_clock::now(); // ����� ������� ��� ����� ��������� current
        pointers.push_back(malloc_func(rand() % (maxSize - minSize + 1) + minSize)); // ���������� ������ ��������
        int total = 1;
        while (true) {
            // ���������� ���������� ���������� ���������
            if (current != total) {
                int addCount = rand() % (current - total) + 1; // ��������� ����� �� 1 �� ����������� ���������� ���������
                total += addCount;
                for (int i = 0; i < addCount; ++i) {
                    pointers.push_back(malloc_func(rand() % (maxSize - minSize + 1) + minSize));
                }
            }
            // �������� ���������� ���������� ���������
            int removeCount = rand() % (pointers.size()+1); // ��������� ����� �� 0 �� ������� �������
            for (int i = 0; i < removeCount; ++i) {
                int indexToRemove = rand() % pointers.size(); // ��������� ������ ��� ��������
                freeAlloc(pointers[indexToRemove]);
                pointers.erase(pointers.begin() + indexToRemove);
            }
            if (total == current && pointers.size() == 0) break; // ����� �� �����, ���� ������ ���������� ��������� ��������� � ������ ����
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;

        // ������ ����������� � ����
        ofstream resFile("DefaultAndOtherStat.txt", ios::app); // �������� ����� � ������ ����������
        cout << pointers.size();
        resFile << nameMalloc << " " << current << " " << diff.count() << " " << osMemoryRequests << endl; // ������ ������
        resFile.close();
    }
}

void statistics1MallocFunctionDefault(string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize) {
    for (size_t current = minIterations; current <= maxIterations; current += stepIterations)
    {
        srand(static_cast<unsigned int>(time(0)));
        vector<int> pointers; // ������ ��� �������� ����������
        auto start = chrono::high_resolution_clock::now(); // ����� ������� ��� ����� ��������� current
        pointers.push_back(rand() % (maxSize - minSize + 1) + minSize); // ���������� ������ ��������
        int total = 1;

        while (true) {
            if (current != total) {
                // ���������� ���������� ���������� ���������
                int addCount = rand() % (current - total) + 1;; // ��������� ����� �� 0 �� ����������� ���������� ���������
                total += addCount;
                for (int i = 0; i < addCount; ++i) {
                    pointers.push_back(rand() % (maxSize - minSize + 1) + minSize);
                }
            }

            // �������� ���������� ���������� ���������
            int removeCount = rand() % (pointers.size() + 1); // ��������� ����� �� 0 �� ������� �������
            for (int i = 0; i < removeCount; ++i) {
                int indexToRemove = rand() % pointers.size(); // ��������� ������ ��� ��������
                pointers.erase(pointers.begin() + indexToRemove);
            }
            if (total == current && pointers.size() == 0) break; // ����� �� �����, ���� ������ ���������� ��������� ��������� � ������ ����
        }
        pointers.clear();

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;

        // ������ ����������� � ����
        ofstream resFile("DefaultAndOtherStat.txt", ios::app); // �������� ����� � ������ ����������
        resFile << nameMalloc << " " << current << " " << diff.count() << endl; // ������ ������
        resFile.close();
    }
}

//int main() {
//    srand(static_cast<unsigned int>(time(0))); // ������������� ���������� ��������� �����
//    std::vector<T> numbers;
//
//    // ��������� ������ �������
//    addElement(numbers);
//
//    // ���������� ��������� ���������� ���������� (�� 1 �� 5)
//    int skipCount = rand() % 5 + 1; // ��������� ����� �� 1 �� 5
//    std::cout << "���������� " << skipCount << " ����������." << std::endl;
//
//    for (int i = 0; i < skipCount; ++i) {
//        addElement(numbers); // ��������� ��������, ������� �� ����������
//        std::this_thread::sleep_for(std::chrono::seconds(1)); // �������� 1 ������� ����� ������������
//    }
//
//    // ������� ��������� ������� �� ��� �����������
//    removeRandomElement(numbers);
//
//    // ������� ��� ���������� ��������
//    numbers.clear(); // ������� ������
//    std::cout << "��� �������� �������." << std::endl;
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
//    cout << name << "����� ������ " << diff.count() << " ������." << endl;
//}
//// ������� � ����������� �������� 
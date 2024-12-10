#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>   
#include <chrono> 
#include <fstream>
#include <string>
#include <windows.h>

#include "statistics.h"
#include "alloc.h"
using namespace std;

chrono::duration<double> randomGenerate(void* (*malloc_func)(size_t), size_t current, size_t minSize, size_t maxSize) {
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
        int removeCount = rand() % (pointers.size() + 1); // ��������� ����� �� 0 �� ������� �������
        for (int i = 0; i < removeCount; ++i) {
            int indexToRemove = rand() % pointers.size(); // ��������� ������ ��� ��������
            freeAlloc(pointers[indexToRemove]);
            pointers.erase(pointers.begin() + indexToRemove);
        }
        if (total == current && pointers.size() == 0) break; // ����� �� �����, ���� ������ ���������� ��������� ��������� � ������ ����
    }

    pointers.clear();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    return diff;
}
chrono::duration<double> orderedGenerate(void* (*malloc_func)(size_t), size_t current){
    vector<void*> pointers; // ������ ��� �������� ����������
    auto start = chrono::high_resolution_clock::now(); // ����� ������� ��� ����� ��������� current
    pointers.push_back(malloc_func(1)); // ���������� ������ �������� ����� 1 ����
    int total = 1;
    while (true) {
        // ���������� ���������� ���������� ���������
        if (current != total) {
            int addCount = rand() % (current - total) + 1; // ��������� ����� �� 1 �� ����������� ���������� ���������
            for (int i = total; i < total+addCount; ++i) {
                pointers.push_back(malloc_func(i)); // ���������� ������������� ������������������ �� 1 �� current
            }
            total += addCount;
        }
        // �������� ���������� ���������� ���������
        int removeCount = rand() % (pointers.size() + 1); // ��������� ����� �� 0 �� ������� �������
        for (int i = 0; i < removeCount; ++i) {
            int indexToRemove = rand() % pointers.size(); // ��������� ������ ��� ��������
            freeAlloc(pointers[indexToRemove]);
            pointers.erase(pointers.begin() + indexToRemove);
        }
        if (total == current && pointers.size() == 0) break; // ����� �� �����, ���� ������ ���������� ��������� ��������� � ������ ����
    }
    pointers.clear();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    return diff;
}
chrono::duration<double> reverseGenerate(void* (*malloc_func)(size_t), size_t current) {
    vector<void*> pointers; // ������ ��� �������� ����������
    auto start = chrono::high_resolution_clock::now(); // ����� ������� ��� ����� ��������� current
    pointers.push_back(malloc_func(current)); // ���������� ������ �������� ����� 1 ����
    int total = 1;
    while (true) {
        // ���������� ���������� ���������� ���������
        if (current != total) {
            int addCount = rand() % (current - total) + 1; // ��������� ����� �� 1 �� ����������� ���������� ���������
            for (int i = total; i < total + addCount; ++i) {
                pointers.push_back(malloc_func(current - i));
            }
            total += addCount;
        }
        // �������� ���������� ���������� ���������
        int removeCount = rand() % (pointers.size() + 1); // ��������� ����� �� 0 �� ������� �������
        for (int i = 0; i < removeCount; ++i) {
            int indexToRemove = rand() % pointers.size(); // ��������� ������ ��� ��������
            freeAlloc(pointers[indexToRemove]);
            pointers.erase(pointers.begin() + indexToRemove);
        }
        if (total == current && pointers.size() == 0) break; // ����� �� �����, ���� ������ ���������� ��������� ��������� � ������ ����
    }
    pointers.clear();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    return diff;
}
chrono::duration<double> equalGenerate(void* (*malloc_func)(size_t), size_t current, size_t size) {
    vector<void*> pointers; // ������ ��� �������� ����������
    auto start = chrono::high_resolution_clock::now(); // ����� ������� ��� ����� ��������� current
    pointers.push_back(malloc_func(size)); // ���������� ������ �������� ����� 1 ����
    int total = 1;
    while (true) {
        // ���������� ���������� ���������� ���������
        if (current != total) {
            int addCount = rand() % (current - total) + 1; // ��������� ����� �� 1 �� ����������� ���������� ���������
            total += addCount;
            for (int i = 0; i < total; ++i) {
                pointers.push_back(malloc_func(size));
            }
        }
        // �������� ���������� ���������� ���������
        int removeCount = rand() % (pointers.size() + 1); // ��������� ����� �� 0 �� ������� �������
        for (int i = 0; i < removeCount; ++i) {
            int indexToRemove = rand() % pointers.size(); // ��������� ������ ��� ��������
            freeAlloc(pointers[indexToRemove]);
            pointers.erase(pointers.begin() + indexToRemove);
        }
        if (total == current && pointers.size() == 0) break; // ����� �� �����, ���� ������ ���������� ��������� ��������� � ������ ����
    }
    pointers.clear();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    return diff;
}

void writeToFile(string nameFile, string result) {
    ofstream resFile(nameFile, ios::app); // �������� ����� � ������ ����������
    resFile << result << endl; // ������ ������
    resFile.close();
}

void statistics1MallocFunctionMethods(void* (*malloc_func)(size_t), string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize) {
    srand(static_cast<unsigned int>(time(0)));
    for (size_t current = minIterations; current <= maxIterations; current += stepIterations)
    {
        chrono::duration<double> resultTime = randomGenerate(malloc_func, current, minSize, maxSize);
        // ������ ����������� � ����
        writeToFile("DefaultAndOtherStat.txt", nameMalloc + " " + to_string(current) + " " + to_string(resultTime.count()));
    }
}
void statistics1MallocFunctionDefault(string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize) {
    for (size_t current = minIterations; current <= maxIterations; current += stepIterations)
    {
        srand(static_cast<unsigned int>(time(0)));
        vector<void*> pointers; // ������ ��� �������� ����������
        auto start = chrono::high_resolution_clock::now(); // ����� ������� ��� ����� ��������� current
        pointers.push_back(malloc(rand() % (maxSize - minSize + 1) + minSize)); // ���������� ������ ��������
        int total = 1;

        while (true) {
            if (current != total) {
                // ���������� ���������� ���������� ���������
                int addCount = rand() % (current - total) + 1;; // ��������� ����� �� 0 �� ����������� ���������� ���������
                total += addCount;
                for (int i = 0; i < addCount; ++i) {
                    pointers.push_back(malloc(rand() % (maxSize - minSize + 1) + minSize));
                }
            }

            // �������� ���������� ���������� ���������
            int removeCount = rand() % (pointers.size() + 1); // ��������� ����� �� 0 �� ������� �������
            for (int i = 0; i < removeCount; ++i) {
                int indexToRemove = rand() % pointers.size(); // ��������� ������ ��� ��������
                free(pointers[indexToRemove]);
                pointers.erase(pointers.begin() + indexToRemove);
            }
            if (total == current && pointers.size() == 0) break; // ����� �� �����, ���� ������ ���������� ��������� ��������� � ������ ����
        }
        pointers.clear();

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;

        // ������ ����������� � ����
        writeToFile("DefaultAndOtherStat.txt", nameMalloc + " " + to_string(current) + " " + to_string(diff.count()));
    }
}

size_t osMemoryRequests = 0;
void statistics2MallocFunctionMethods(void* (*malloc_func)(size_t), string nameMalloc, string generate, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize, size_t size) {
    srand(static_cast<unsigned int>(time(0)));
    for (size_t current = minIterations; current <= maxIterations; current += stepIterations)
    {
        osMemoryRequests = 0;
        if (generate == "random") {
            chrono::duration<double> resultTime = randomGenerate(malloc_func, current, minSize, maxSize);
            writeToFile("RandomStatisticsSpecMalloc.txt", nameMalloc + " " + to_string(current) + " " + to_string(resultTime.count()) + " " + to_string(osMemoryRequests)); // ������ ����������� � ����
        }
        else if (generate == "ordered") {
            chrono::duration<double> resultTime = orderedGenerate(malloc_func, current);
            writeToFile("OrderedStatisticsSpecMalloc.txt", nameMalloc + " " + to_string(current) + " " + to_string(resultTime.count()) + " " + to_string(osMemoryRequests)); // ������ ����������� � ����
        }
        else if (generate == "reverse") {
            chrono::duration<double> resultTime = reverseGenerate(malloc_func, current);
            writeToFile("ReverseStatisticsSpecMalloc.txt", nameMalloc + " " + to_string(current) + " " + to_string(resultTime.count()) + " " + to_string(osMemoryRequests)); // ������ ����������� � ����
        }
        else {
            chrono::duration<double> resultTime = equalGenerate(malloc_func, current, size);
            writeToFile("EqualStatisticsSpecMalloc.txt", nameMalloc + " " + to_string(current) + " " + to_string(resultTime.count()) + " " + to_string(osMemoryRequests));  // ������ ����������� � ����
        }
        cleanUpMemory();
    }

}
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

// ����� ������ �������� ����
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
// ������ ������ �������������� ������
enum MallocMethod {
    START2,
    MALLOC,
    SPECMALLOC
};
// ������ ������ ������ ���������� ����� ������
enum specMallocMethod {
    START3,
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

// �������� �� ���� �������������� �����
int getNum() {
    int num;
    while (true) { // ����������� ���� ��� ���������� ������� �����
        cout << "\n������� �����: ";
        if (cin >> num) { // ���� ���� �������
            if (num > 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������������� ���������� ��������
                return num;
            }
            else cerr << "������� ������������� �����." << endl;
        }
        else {
            cin.clear(); // ������� ��������� ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "������������ ����. ����������, ������� �����." << endl;
        }
    }
}
// ���� � �������� ������ ��� ��������� ��������
void getBordersRandom(int& minim, int& maxim) {
    while (true) {
        cout << "\n������� ����������� ������� ���������� �������� ������� �����.";
        minim = getNum();
        cout << "������� ������������ ������� ���������� �������� ������� �����.";
        maxim = getNum();
        if (maxim > minim) break;
        else if (minim > maxim) cerr << "����������� ������� ������ ���� ������ ������������.";
        else cerr << "������� �� ����� ���������.";
    }
    return;
}
// ���� � �������� ������ ���������� ������ ������ (���������� ��������� = ��������)
void getBordersStepIter(int& minim, int& maxim, int& step) {
    while (true) {
        cout << "\n������� ����������� ���������� �������� � �������.";
        minim = getNum();
        cout << "������� ������������ ���������� �������� � �������.";
        maxim = getNum();
        if (maxim > minim) break;
        else if (minim > maxim) cerr << "����������� ������� ������ ���� ������ ������������.";
        else cerr << "������� �� ����� ���������.";
    }
    while (true) {
        cout << "\n������� ��� ��������� ���������� ��������: ";
        step = getNum();
        if (maxim - minim < step) cerr << "��� �� ����� ���� ������ ���������� ��������.";
        else break;
    }
}

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
        answer = getNum();
    } while (answer <= START || answer > SPECEQUAL);
    return answer;
}
// ���� ������ ������ �� ���������� ������
int getMenuMallocMethod() {
    int answer;
    do {
        cout << endl << "�������� ���� �� ������ ��������� � �������� ������:" << endl
            << MALLOC << " � ����������� ������� malloc" << endl
            << SPECMALLOC << " � �������������� ������" << endl;
        cout << "������� ����� ������ �� ��������: ";
        answer = getNum();
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
        answer = getNum();
    } while (answer <= START || answer > WORST_FIT);
    return answer;
};
// ������� ����
int getMenuItem() {
    int answer;
    do {
        cout << endl << "�������� ���� �� ������� ����:" << endl
            << STATISTICS_MODE1 << " � ���������� ������ ��� ��������� �� ����������� �������� malloc" << endl
            << STATISTICS_MODE2 << " � ���������� ������ ��� ��������� ������� ���������� ����� � ������" << endl
            << EXIT << " - ����� �� ���������." << endl << endl;
        cout << "������� ����� ������ ����: ";
        answer = getNum();
    } while (answer <= START || answer > EXIT);
    return answer;
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    cout << "-- ������������ �������������� ������ --" << endl;
    int answerMenu, answer;
    do {
        answerMenu = getMenuItem();
        cout << endl;
        switch (answerMenu)
        {
        // ������ ����� ����� ������ ��� �������������� ������� ������������ malloc � �������������� ����������
        case STATISTICS_MODE1: {
            answer = getMenuMallocMethod();

            // ���� � �������� ������ ���������� ������� �����
            int minMem, maxMem;
            getBordersRandom(minMem, maxMem);
            cout << endl;

            // ���� � �������� ���� � ������ ���������� ��������� � �������� ������ �� ������
            int minIt, maxIt, stepIt;
            getBordersStepIter(minIt, maxIt, stepIt);
            cout << endl;

            // ���������� ����� ������. ��� malloc � �������������� �������������� ������ ������ �������.
            if (answer == 1) statistics1MallocFunctionDefault("malloc", minIt, maxIt, stepIt, minMem, maxMem);
            else {
                answer = getMenuFit();
                if (answer == 1) statistics1MallocFunctionMethods(firstFitMalloc, "firstFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
                else if (answer == 2) statistics1MallocFunctionMethods(bestFitMalloc, "bestFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
                else statistics1MallocFunctionMethods(worstFitMalloc, "worstFitMalloc", minIt, maxIt, stepIt, minMem, maxMem);
            }
            std::string command1 = argv[0]; // �������� ��� ������� ���������
            std::string command = "\"" + command1 + "\"";
            int result = system(command.c_str());
            exit(0); // ����� �� �������� ����������
            break;
        }
        // ������ ����� ����� ������ ��� �������������� ������� ��������������� ������ � ����������� �� ������ �����
        case STATISTICS_MODE2:{
            string generated; // �������� ������� ��������� 
            int fit = getMenuFit(); // ����� ������� ������ ����� ������
            int generate = getMenuGenerateBytes(); // ����� ������� ���������
            int minMem = 0, maxMem = 0; // ���������� ��� ��������� ���������
            int size = 0; // �������� ��� ���������� ������

            // � ����������� �� ��������� ���� �������������� ����������
            if (generate == 1) {
                generated = "random";
                getBordersRandom(minMem, maxMem);
                cout << endl;
            }
            else if (generate == 2) generated = "ordered";
            else if (generate == 3) generated = "reverse";
            else if(generate == 4) {
                generated = "equal";
                cout << "������� ������ ����� ������.";
                size = getNum();
            }

            // ���� � �������� ���� � ������ ���������� ��������� � �������� ������ �� ������
            int minIt, maxIt, stepIt;
            getBordersStepIter(minIt, maxIt, stepIt);
            cout << endl;

            // ���������� ����� ������
            if (fit == 1) statistics2MallocFunctionMethods(firstFitMalloc, "firstFitMalloc", generated, minIt, maxIt, stepIt, minMem, maxMem, size);
            else if (fit == 2) statistics2MallocFunctionMethods(bestFitMalloc, "bestFitMalloc", generated, minIt, maxIt, stepIt, minMem, maxMem, size);
            else if (fit == 3) statistics2MallocFunctionMethods(firstFitMalloc, "worstFitMalloc", generated, minIt, maxIt, stepIt, minMem, maxMem, size);

            std::string command1 = argv[0]; // �������� ��� ������� ���������
            std::string command = "\"" + command1 + "\"";
            int result = system(command.c_str());
            exit(0); // ����� �� �������� ����������

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

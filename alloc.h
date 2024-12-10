#pragma once
#define NALLOC 1024 // ����������� ����� ������ ������ ��� �������
typedef long Align; // ��� ������������ �� ������� long

// ��������� ��������� ����� ������
struct Header {
    Header* ptr; // ��������� ���� � ������ ���������
    size_t size; // ������ �������� �����
    Align x; // ������������ ����� �� �������

    ~Header() {
        delete[] ptr; // ������������ ������
    }
};

typedef Header* HeaderPtr;

static Header base; // ������ ������ ��� ���������� �������
static HeaderPtr freep = nullptr; // ������ � ������ ��������� ������

void cleanUpMemory();

void freeAlloc(void* ap); // ������������ ����� � ������� � ��������
HeaderPtr morecore(size_t nu); // ������ ������ �����

void* firstFitMalloc(size_t nbytes); // ���������� � ������ ���������� ����
void* bestFitMalloc(size_t nbytes); // ���������� � ������ ���������� ����
void* worstFitMalloc(size_t nbytes); // ����������� � ������ ���������� ����
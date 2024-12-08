#pragma once
//#define NALLOC 1024 // ����������� ����� ������ ������ ��� �������
typedef long Align; // ��� ������������ �� ������� long

// ��������� ��������� ����� ������
struct Header {
    Header* ptr; // ��������� ���� � ������ ���������
    size_t size; // ������ �������� �����
    Align x; // ������������ ����� �� �������
};

typedef Header* HeaderPtr;
static size_t osMemoryRequests=0;

static Header base; // ������ ������ ��� ���������� �������
static HeaderPtr freep = nullptr; // ������ � ������ ��������� ������

void freeAlloc(void* ap); // ������������ ����� � ������� � ��������
HeaderPtr morecore(size_t nu); // ������ ������ �����

void* firstFitMalloc(size_t nbytes); // ���������� � ������ ���������� ����
void* bestFitMalloc(size_t nbytes); // ���������� � ������ ���������� ����
void* worstFitMalloc(size_t nbytes); // ����������� � ������ ���������� ����
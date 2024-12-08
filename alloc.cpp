#include <cstddef>
#include <iostream>
#include <windows.h>
#include "alloc.h"

using namespace std;


// ����� ������� ��� ������������� ������
void freeAlloc(void* ap) {
    //������� ����� ����� ��������� ��� ����� �������, ��� �
    //������ ������, ��� � ��� �����. � ����� ������, ���� ���������� ������������ ���� ��������� �
    //��������� �����, �� ������������ � ��� � ���� ����.� ��� ��� �������� ������������, � ��� ��� � ���,
    //����� ��������� ��������� � ������ ����� � ������� ������ ���� �����������.
    if (ap == nullptr) return;

    Header* bp = (Header*)ap - 1; // ��������� �� ��������� �����
    Header* p;

    // ����� ��� ���������� ����������� ����� � ������ ��������� ������
    for (p = freep; !(bp > p && bp < p->ptr); p = p->ptr) { // ���� �� ��� ���, ���� ��������� �� ��������� � �������� �������� � ���������� �����
        if (p >= p->ptr && (bp > p || bp < p->ptr)) // �������� �� ����� ������, ���� ������ ���������
            break; // ������������ ����� � ������ ��� � �����
    }

    // ������� �� ��������� �������
    if (bp + bp->size == p->ptr) { // ���� ����� �������������� ����� ��������� ����� ������� ����������
        bp->size += p->ptr->size; // ����������� � ������� (������� ������� ����)
        bp->ptr = p->ptr->ptr; // ���������� ���������
    }
    else {
        bp->ptr = p->ptr; // ��������� ��������� ���������� ��� ���������� �� ��������� ����
    }

    // ������� � ���������� �������
    if (p + p->size == bp) { // ���� ����� ����������� ��������� �� ������ ������������� �����
        p->size += bp->size; // ����������� � ���������� �������
        p->ptr = bp->ptr; // ���������� ��������� �� ��������� ����
    }
    else {
        p->ptr = bp; // ��������� ��������� ���������� ��� ����������� �� ���������
    }

    freep = p; // ���������� ��������� �� ������ ���������� ������
}
HeaderPtr morecore(size_t nu) {
    Header* up = (Header*)VirtualAlloc(nullptr, nu * sizeof(Header), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    osMemoryRequests++;
    // ������ ������ � ������������ ������� (����� ���������� �����, ������������� ������, �������������� � ��������� �����, ������ ��� ������ � ������)
    if (up == nullptr) // �������� �� ��������� ������
        return nullptr;
    up->size = nu;
    freeAlloc((void*)(up + 1)); // ������������ �����, ������� ������ ��� �������
    cout << "��������!" << endl ;
    return freep; 
}

// �������������� ������, ������������ ������ �������
void* firstFitMalloc(size_t nbytes) {
    HeaderPtr p, prevp;
    size_t nunits;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; // ���������� ������

    // ���� ������ ��������� ������ ������
    if ((prevp = freep) == nullptr) {
        base.ptr = freep = prevp = &base; // ������������� ���������� �������; ���������, ��������� � ���������� = ������� ������ 
        base.size = 0;
    }

    // ��������� ����� ����������
    for (p = prevp->ptr; ; prevp = p, p = p->ptr) { // ����������� ���� ������ �����
        if (p->size >= nunits) {
            if (p->size == nunits) // ���� ������ ����� ����� ��������
                prevp->ptr = p->ptr;
            else {
                p->size -= nunits; // ���������� �������
                p += p->size; // ����������� ��������� �� ����������� ����
                p->size = nunits; // ��������� ������ �������
            }
            freep = prevp; // ����������� ������ ������ ��������� ������
            return (void*)(p + 1); // ����������� ��������� �� ���������� ������ ����� Header
        }
        if (p == freep) // ���� ��� ����� ��������, �� ���������� ����� ������
            if ((p = morecore(nunits)) == nullptr) {
                return nullptr;
            }
    }
}
void* bestFitMalloc(size_t nbytes) {
    HeaderPtr p, prevp, bestp = nullptr, bestprevp = nullptr;
    size_t nunits, minexcess = SIZE_MAX; // Minexcess � ����������� �������� ����� � �����

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == nullptr) {
        base.ptr = freep = prevp = &base;
        base.size = 0;
    }

    for (p = prevp->ptr; ; prevp = p, p = p->ptr) {
        if (p->size >= nunits) {
            size_t excess = p->size - nunits;
            if (excess < minexcess) { // ���� � ����� ����� ������ ��������� �����, �� �� �����������
                minexcess = excess;
                bestp = p;
                bestprevp = prevp;
                if (excess == 0) break; // ���� ��������� �� ����� � �����, �� �� �������
            }
        }
        if (p == freep) { // ���� ��� ����� ��������, �� ���������� ����� ������
            osMemoryRequests++;
            if ((p = morecore(nunits)) == nullptr) {
                return nullptr;
            }
        }
    }

    if (bestp != nullptr) { // ���������� ���������� ���������� ������ "������ ����������"
        if (bestp->size == nunits)
            bestprevp->ptr = bestp->ptr;
        else {
            bestp->size -= nunits;
            bestp += bestp->size;
            bestp->size = nunits;
        }
        freep = bestprevp;
        return (void*)(bestp + 1);
    }
}
void* worstFitMalloc(size_t nbytes) {
    HeaderPtr p, prevp, worstp = nullptr, worstprevp = nullptr;
    size_t nunits, maxexcess = 0;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == nullptr) {
        base.ptr = freep = prevp = &base;
        base.size = 0;
    }

    for (p = prevp->ptr; ; prevp = p, p = p->ptr) {
        if (p->size >= nunits) {
            size_t excess = p->size - nunits;
            cout << excess << endl;
            if (excess >= maxexcess) { // ���� ���������� ����� � ����� ������, �� �� �����������
                maxexcess = excess;
                worstp = p;
                worstprevp = prevp;
            }
        }
        if (p == freep && worstp == nullptr) { // ���� ��� ����� ��������, �� ���������� ����� ������, ���� ���� ����� ���������
            p = morecore(nunits);
            worstp = p;
            worstprevp = prevp;
            osMemoryRequests++;
            break;
        }
        if (p == freep) {
            break;
        }
    }
  

    if (worstp != nullptr) {// ���������� ���������� ���������� ������ "������ ����������"
        if (worstp->size == nunits)
            worstprevp->ptr = worstp->ptr;
        else {
            worstp->size -= nunits;
            worstp += worstp->size;
            worstp->size = nunits;
        }
        freep = worstprevp;
        return (void*)(worstp + 1);
    }
    else {
        cout << "problems" << endl;
        return 0;
    }
}
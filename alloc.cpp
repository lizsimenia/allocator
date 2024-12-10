#include <cstddef>
#include <iostream>
#include <windows.h>
#include "alloc.h"
#include "statistics.h"


using namespace std;

// Общие функции для распределитей памяти
void cleanUpMemory() {
    // Освобождение всех выделенных страниц
    if (freep != nullptr) {
        Header* p = freep;
        while (p != nullptr) {
            Header* next = p->ptr;
            if (p != nullptr) {
                if (!VirtualFree(p, 0, MEM_RELEASE)) {
                    break;
                }
            }
            p = next;
        }
        freep = nullptr; // Обнуление указателя на свободный список
    }
}
void freeAlloc(void* ap) {
    if (ap == nullptr) return;

    Header* bp = (Header*)ap - 1; // Указатель на заголовок блока
    Header* p;

    // Поиск для нахождения подходящего места в списке свободных блоков
    for (p = freep; !(bp > p && bp < p->ptr); p = p->ptr) { // Цикл до тех пор, пока указатель не находится в пределах текущего и следующего блока
        if (p >= p->ptr && (bp > p || bp < p->ptr)) // Проверка на конец списка, если список замкнулся
            break; // Освобождение блока в начале или в конце
    }

    // Слияние со следующим соседом
    if (bp + bp->size == p->ptr) { // Если конец освобождаемого блока находится перед началом следующего
        bp->size += p->ptr->size; // Объединение с соседом (создает большой блок)
        bp->ptr = p->ptr->ptr; // Обновление указателя
    }
    else {
        bp->ptr = p->ptr; // Установка указателя следующего для свободного на следующий блок
    }

    // Слияние с предыдущим соседом
    if (p + p->size == bp) { // Если конец предыдущего находится на начале особожденного блока
        p->size += bp->size; // Объединение с предыдущим соседом
        p->ptr = bp->ptr; // Обновление указатель на следующий блок
    }
    else {
        p->ptr = bp; // Установка указателя следующего для предыдущего на свободный
    }

    freep = p; // Обновление указателя на начало свободного списка
}
HeaderPtr morecore(size_t nu) {
    Header* up = (Header*)VirtualAlloc(nullptr, nu * sizeof(Header), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    osMemoryRequests++;
    // Запрос памяти у операционной системы (любой подходящий адрес, запрашиваемый размер, резервирование и выделение сразу, доступ для чтения и записи)
    if (up == nullptr) // Проверка на выделение памяти
        return nullptr;
    up->size = nu;
    freeAlloc((void*)(up + 1)); // освобождение блока, который только что выделен
    return freep; 
}

// Распределители памяти, использующие разные подходы
void* firstFitMalloc(size_t nbytes) {
    HeaderPtr p, prevp, firstp = nullptr, firstprevp = nullptr;
    size_t nunits;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; // Вычисление памяти

    // Если список свободных блоков пустой
    if ((prevp = freep) == nullptr) {
        base.ptr = freep = prevp = &base; // Инициализация начального запуска; Следующий, начальный и предыдущий = пустому списку 
        base.size = 0;
    }

    // Свободные блоки существуют
    for (p = prevp->ptr; ; prevp = p, p = p->ptr) { // Бесконечный цикл поиска места
        if (p == freep && firstp != nullptr) break;
        if (p == freep && firstp == nullptr) {
            if ((p = morecore(nunits)) == nullptr) {
                return nullptr;
            }
            continue;
        }
        if (p->size >= nunits) {
            if (p->size == nunits) { // Если размер блока равен искомому
                firstp = p;
                firstprevp = prevp;
                prevp->ptr = p->ptr;
            }
            else {
                firstp = p;
                firstprevp = prevp;
                p->size -= nunits; // Уменьшение размера
                p += p->size; // Перемещение указателя на уменьшенный блок
                p->size = nunits; // Установка нового размера
            }
            freep = prevp; // Перемещение начала списка свободных блоков
            return (void*)(p + 1); // Возвращение указателя на выделенную память после Header
        }
    }
}
void* bestFitMalloc(size_t nbytes) {
    HeaderPtr p, prevp, bestp = nullptr, bestprevp = nullptr;
    size_t nunits, minexcess = SIZE_MAX; // Minexcess – минимальное ненужное место в блоке

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == nullptr) {
        base.ptr = freep = prevp = &base;
        base.size = 0;
    }

    for (p = prevp->ptr; ; prevp = p, p = p->ptr) {
        if (p == freep && bestp != nullptr) break;
        if (p == freep && bestp == nullptr) {
            if ((p = morecore(nunits)) == nullptr) {
                return nullptr;
            }
            continue;
        }
        if (p->size >= nunits) {
            size_t excess = p->size - nunits;
            if (excess <= minexcess) { // Если в новом блоке меньше ненужного места, то всё обновляется
                minexcess = excess;
                bestp = p;
                bestprevp = prevp;
                if (excess == 0) break; // Если заполняет всё место в блоке, то он идеален
            }
        }
    }

    if (bestp != nullptr) { // Обновление указателей аналогично методу "Первый подходящий"
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
        if (p == freep && worstp!= nullptr) break;
        if (p == freep && worstp == nullptr) {
            if ((p = morecore(nunits)) == nullptr) {
                return nullptr; 
            }
            continue;
        }
        if (p->size >= nunits) {
            size_t excess = p->size - nunits;
            if (excess >= maxexcess) { // Если свободного места в блоке больше, то всё обновляется
                maxexcess = excess;
                worstp = p;
                worstprevp = prevp;
            }
        }
    }
  

    if (worstp != nullptr) {// Обновление указателей аналогично методу "Первый подходящий"
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
}
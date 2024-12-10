#pragma once
#define NALLOC 1024 // Минимальное число единиц памяти для запроса
typedef long Align; // Для выравнивания по границе long

// Структура заголовка блока памяти
struct Header {
    Header* ptr; // Следующий блок в списке свободных
    size_t size; // Размер текущего блока
    Align x; // Выравнивание блока по границе

    ~Header() {
        delete[] ptr; // Освобождение памяти
    }
};

typedef Header* HeaderPtr;

static Header base; // Пустой список для начального запуска
static HeaderPtr freep = nullptr; // Начало в списке свободных блоков

void cleanUpMemory();

void freeAlloc(void* ap); // Освобождение блока и слияние с соседним
HeaderPtr morecore(size_t nu); // Запрос нового блока

void* firstFitMalloc(size_t nbytes); // Размещение в первый подходящий блок
void* bestFitMalloc(size_t nbytes); // Размещение в лучший подходящий блок
void* worstFitMalloc(size_t nbytes); // Размещениее в худший подходящий блок
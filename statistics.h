#pragma once
#include <chrono>
// Cбор статистических данных первого режима
void statistics1MallocFunctionMethods(void* (*malloc_func)(size_t), std::string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize);
void statistics1MallocFunctionDefault(std::string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize);
void statistics2MallocFunctionMethods(void* (*malloc_func)(size_t), std::string nameMalloc, std::string generate, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize, size_t size);
// Запись в файл результата итерации
void writeToFile(std::string nameFile,std:: string result);

// Методы генерации количества байт
std::chrono::duration<double> randomGenerate(void* (*malloc_func)(size_t), size_t current, size_t minSize, size_t maxSize);
std::chrono::duration<double> orderedGenerate(void* (*malloc_func)(size_t), size_t current);
std::chrono::duration<double> reverseGenerate(void* (*malloc_func)(size_t), size_t current);
std::chrono::duration<double> equalGenerate(void* (*malloc_func)(size_t), size_t current, size_t size);

extern size_t osMemoryRequests;

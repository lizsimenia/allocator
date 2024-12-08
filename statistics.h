#pragma once

// Cбор статистических данных первого режима
void statistics1MallocFunctionMethods(void* (*malloc_func)(size_t), std::string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize);
void statistics1MallocFunctionDefault(std::string nameMalloc, size_t minIterations, size_t maxIterations, size_t stepIterations, size_t minSize, size_t maxSize);
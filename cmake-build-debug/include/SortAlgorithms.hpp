#pragma once
#include <vector>

// Sortowanie przez wstawianie
void insertionSort(std::vector<int>& arr);

// Sortowanie przez kopcowanie (heap sort)
void heapSort(std::vector<int>& arr);

// Sortowanie Shella
void shellSort(std::vector<int>& arr);

// Sortowanie szybkie (quick sort)
// pivotStrategy określa strategię wyboru pivota:
// 1 - lewy, 2 - prawy, 3 - srodkowy, 4 - losowy
void quickSort(std::vector<int>& arr, int pivotStrategy);

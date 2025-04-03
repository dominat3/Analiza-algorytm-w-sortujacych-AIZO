#pragma once
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Insertion Sort – szablonowa implementacja
template <typename T>
void insertionSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Heap Sort – funkcja pomocnicza heapify
template <typename T>
void heapify(std::vector<T>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Heap Sort – główna funkcja
template <typename T>
void heapSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i >= 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Helper: Generacja sekwencji Franka i Lazarusa
// W literaturze jedna z propozycji tej sekwencji opiera się na klasycznym
// podejściu: zaczynamy od h = n/2, a następnie h = floor(h / 2)
inline std::vector<int> generateFrankLazarusGaps(int n) {
    std::vector<int> gaps;
    int gap = n / 2;
    while (gap > 0) {
        gaps.push_back(gap);
        gap /= 2;
    }
    return gaps;
}

// Shell Sort – implementacja z dwoma strategiami odstępów:
// strategy = 1 -> klasyczna sekwencja: gap = n/2, gap /= 2
// strategy = 2 -> sekwencja Franka i Lazarusa (zgodnie z propozycją opisaną w literaturze)
template <typename T>
void shellSort(std::vector<T>& arr, int strategy = 1) {
    int n = arr.size();
    if (n < 2) return;

    if (strategy == 1) {
        // Klasyczna sekwencja: początkowy gap = n/2, a następnie dzielenie przez 2
        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                T temp = arr[i];
                int j = i;
                while (j >= gap && arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                arr[j] = temp;
            }
        }
    } else if (strategy == 2) {
        // Sekwencja Franka i Lazarusa – generujemy odstępy według klasycznego wzoru
        // h = n/2, h /= 2, ... (to jedna z propozycji omawianych przez Franka i Lazarusa)
        std::vector<int> gaps = generateFrankLazarusGaps(n);
        // Przetwarzamy odstępy w kolejności malejącej (od największego do najmniejszego)
        for (int gap : gaps) {
            for (int i = gap; i < n; i++) {
                T temp = arr[i];
                int j = i;
                while (j >= gap && arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                arr[j] = temp;
            }
        }
    }
}

// Quick Sort – wybór pivota
template <typename T>
int choosePivot(std::vector<T>& arr, int low, int high, int pivotStrategy) {
    if (pivotStrategy == 1) {
        return low; // lewy
    } else if (pivotStrategy == 2) {
        return high; // prawy
    } else if (pivotStrategy == 3) {
        return low + (high - low) / 2; // środkowy
    } else if (pivotStrategy == 4) {
        return low + std::rand() % (high - low + 1); // losowy
    }
    return high; // domyślnie
}

// Quick Sort – funkcja partycjonująca (Lomuto)
template <typename T>
int partition(std::vector<T>& arr, int low, int high, int pivotStrategy) {
    int pivotIndex = choosePivot(arr, low, high, pivotStrategy);
    std::swap(arr[pivotIndex], arr[high]);
    T pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quick Sort – rekurencyjna funkcja
template <typename T>
void quickSortRecursive(std::vector<T>& arr, int low, int high, int pivotStrategy) {
    if (low < high) {
        int pi = partition(arr, low, high, pivotStrategy);
        quickSortRecursive(arr, low, pi - 1, pivotStrategy);
        quickSortRecursive(arr, pi + 1, high, pivotStrategy);
    }
}

// Quick Sort – główna funkcja
template <typename T>
void quickSort(std::vector<T>& arr, int pivotStrategy) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    quickSortRecursive(arr, 0, arr.size() - 1, pivotStrategy);
}

#include "../include/DataGenerator.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Insertion Sort
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Heap Sort - funkcja pomocnicza do budowania kopca
static void heapify(std::vector<int>& arr, int n, int i) {
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

void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Budowanie kopca
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Wyciąganie elementów z kopca
    for (int i = n - 1; i >= 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Shell Sort
void shellSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

// Quick Sort - pomocnicze funkcje

// Wybór pivota w zależności od strategii
static int choosePivot(std::vector<int>& arr, int low, int high, int pivotStrategy) {
    if (pivotStrategy == 1) {
        // Lewy element
        return low;
    } else if (pivotStrategy == 2) {
        // Prawy element
        return high;
    } else if (pivotStrategy == 3) {
        // Srodkowy element
        return low + (high - low) / 2;
    } else if (pivotStrategy == 4) {
        // Losowy element
        return low + rand() % (high - low + 1);
    }
    // Domyslnie zwracamy prawy element
    return high;
}

static int partition(std::vector<int>& arr, int low, int high, int pivotStrategy) {
    int pivotIndex = choosePivot(arr, low, high, pivotStrategy);
    std::swap(arr[pivotIndex], arr[high]);
    int pivot = arr[high];
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

static void quickSortRecursive(std::vector<int>& arr, int low, int high, int pivotStrategy) {
    if (low < high) {
        int pi = partition(arr, low, high, pivotStrategy);
        quickSortRecursive(arr, low, pi - 1, pivotStrategy);
        quickSortRecursive(arr, pi + 1, high, pivotStrategy);
    }
}

void quickSort(std::vector<int>& arr, int pivotStrategy) {
    srand(static_cast<unsigned int>(time(nullptr)));
    quickSortRecursive(arr, 0, arr.size() - 1, pivotStrategy);
}

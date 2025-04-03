#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <limits>
#include <type_traits>

// Enum określający zakres generowanych danych
enum class RangeOption {
    Limited, // ograniczony zakres: [0, 100]
    Full     // pełny zakres: np. dla int: [-1000000, 1000000], dla float: [-1e6, 1e6]
};

// Szablon funkcji wczytującej tablicę z pliku.
// Zakładamy, że pierwsza liczba w pliku to rozmiar tablicy, a kolejne elementy to dane typu T.
template <typename T>
std::vector<T> loadArray(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<T> arr;
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return arr;
    }
    int size;
    file >> size;
    if (size <= 0) {
        std::cerr << "Nieprawidłowy rozmiar tablicy: " << size << std::endl;
        return arr;
    }
    arr.resize(size);
    for (int i = 0; i < size; ++i) {
        T value;
        file >> value;
        arr[i] = value;
    }
    return arr;
}

// Szablon funkcji generującej losową tablicę typu T.
// Parametr rangeOption decyduje o zakresie generowanych liczb.
template <typename T>
std::vector<T> generateRandomArray(int size, RangeOption rangeOption) {
    std::vector<T> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());

    if (rangeOption == RangeOption::Limited) {
        // Ograniczony zakres: 0 - 100
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<int> dist(0, 100);
            for (int i = 0; i < size; ++i)
                arr[i] = static_cast<T>(dist(gen));
        } else {
            std::uniform_real_distribution<float> dist(0.0f, 100.0f);
            for (int i = 0; i < size; ++i)
                arr[i] = static_cast<T>(dist(gen));
        }
    } else {
        // Pełny zakres
        if constexpr (std::is_same_v<T, int>) {
            std::uniform_int_distribution<int> dist(-1000000, 1000000);
            for (int i = 0; i < size; ++i)
                arr[i] = dist(gen);
        } else {
            std::uniform_real_distribution<double> dist(-1e6, 1e6);
            for (int i = 0; i < size; ++i)
                arr[i] = static_cast<T>(dist(gen));
        }
    }

    return arr;
}

// Szablon funkcji wypisującej zawartość tablicy na standardowe wyjście.
template <typename T>
void printArray(const std::vector<T>& arr) {
    for (const auto& elem : arr)
        std::cout << elem << " ";
    std::cout << std::endl;
}

#include "../include/SortAlgorithms.hpp"
#include <iostream>
#include <fstream>
#include <random>

// Funkcja wczytująca dane z pliku tekstowego.
// Format: pierwsza liczba to rozmiar tablicy, a kolejne to elementy.
std::vector<int> loadArray(const std::string& filename) {
    std::vector<int> arr;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return arr;
    }

    int size;
    file >> size;

    // Upewniamy się, że rozmiar jest dodatni
    if (size <= 0) {
        std::cerr << "Nieprawidłowy rozmiar tablicy: " << size << std::endl;
        return arr;
    }

    arr.resize(size);
    for (int i = 0; i < size; ++i) {
        file >> arr[i];
    }

    file.close();
    return arr;
}

// Funkcja generująca losową tablicę o zadanym rozmiarze.
// Na potrzeby testów generujemy małe zbiory danych.
std::vector<int> generateRandomArray(int size) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    // Ustawiamy zakres liczb, np. od 0 do 100 (dla małych zbiorów)
    std::uniform_int_distribution<> dist(0, 100);

    for (int i = 0; i < size; ++i) {
        arr[i] = dist(gen);
    }

    return arr;
}

// Funkcja wyświetlająca zawartość tablicy.
void printArray(const std::vector<int>& arr) {
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

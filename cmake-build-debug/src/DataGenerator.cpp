#include "../include/SortAlgorithms.hpp"
#include <iostream>
#include <fstream>
#include <random>

std::vector<int> loadArray(const std::string& filename) {
    std::vector<int> arr;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku: " << filename << std::endl;
        return arr;
    }

    int size;
    file >> size;
    if (size <= 0) {
        std::cerr << "Nieprawidlowy rozmiar tablicy: " << size << std::endl;
        return arr;
    }

    arr.resize(size);
    for (int i = 0; i < size; ++i) {
        file >> arr[i];
    }

    file.close();
    return arr;
}

std::vector<int> generateRandomArray(int size) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    // Zakres liczb od 0 do 100 – wystarczający przy testach na małych zbiorach
    std::uniform_int_distribution<> dist(0, 100);

    for (int i = 0; i < size; ++i) {
        arr[i] = dist(gen);
    }

    return arr;
}

void printArray(const std::vector<int>& arr) {
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

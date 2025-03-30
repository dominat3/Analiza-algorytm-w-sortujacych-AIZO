#pragma once

#include <vector>
#include <string>

// Wczytuje tablicę danych z pliku tekstowego.
// Pierwsza liczba w pliku określa rozmiar tablicy, kolejne elementy to dane.
std::vector<int> loadArray(const std::string& filename);

// Generuje losową tablicę o zadanym rozmiarze.
// Przydatne przy testach – na początek małe zbiory danych.
std::vector<int> generateRandomArray(int size);

// Wyświetla zawartość tablicy na standardowym wyjściu.
void printArray(const std::vector<int>& arr);

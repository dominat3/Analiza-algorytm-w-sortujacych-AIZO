#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "../include/DataGenerator.hpp"
#include "../include/SortAlgorithms.hpp"

using namespace std;

// Funkcja wyświetlająca menu główne
void displayMenu() {
    cout << "----------------------------------------" << endl;
    cout << "         MENU GLOWNE - SORTOWANIE       " << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Wczytaj tablice z pliku" << endl;
    cout << "2. Wygeneruj losowa tablice" << endl;
    cout << "3. Wyswietl aktualna tablice" << endl;
    cout << "4. Sortuj tablice" << endl;
    cout << "5. Wyjdz z programu" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Wybor: ";
}

// Funkcja wyświetlająca menu sortowania
void displaySortMenu() {
    cout << "\nWybierz algorytm sortowania:" << endl;
    cout << "1. Insertion Sort" << endl;
    cout << "2. Heap Sort" << endl;
    cout << "3. Shell Sort" << endl;
    cout << "4. Quick Sort" << endl;
    cout << "Wybor: ";
}

// Funkcja szablonowa do pomiaru czasu sortowania
template <typename Func>
void measureSortTime(Func sortFunction, vector<int>& arr, const string& sortName) {
    auto start = chrono::high_resolution_clock::now();
    sortFunction(arr);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << sortName << " zajal: " << duration.count() << " mikrosekund" << endl;
}

int main() {
    bool runProgram = true;
    int choice = 0;
    vector<int> currentArray;  // Aktualna tablica, na której będziemy pracować

    while (runProgram) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string filename;
                cout << "Podaj nazwe pliku: ";
                cin >> filename;
                currentArray = loadArray(filename);
                if (!currentArray.empty())
                    cout << "Tablica wczytana z pliku." << endl;
                else
                    cout << "Wystapil blad podczas wczytywania tablicy." << endl;
                break;
            }
            case 2: {
                int size;
                cout << "Podaj rozmiar tablicy: ";
                cin >> size;
                currentArray = generateRandomArray(size);
                cout << "Losowa tablica utworzona." << endl;
                break;
            }
            case 3: {
                if (!currentArray.empty()) {
                    cout << "Aktualna tablica:" << endl;
                    printArray(currentArray);
                } else {
                    cout << "Brak tablicy do wyswietlenia. Wczytaj lub wygeneruj tablice." << endl;
                }
                break;
            }
            case 4: {
                if (currentArray.empty()) {
                    cout << "Brak tablicy do sortowania. Wczytaj lub wygeneruj tablice." << endl;
                    break;
                }
                displaySortMenu();
                int sortChoice;
                cin >> sortChoice;
                // Tworzymy kopię tablicy, by oryginał pozostał bez zmian
                vector<int> arrayToSort = currentArray;

                switch (sortChoice) {
                    case 1:
                        measureSortTime(insertionSort, arrayToSort, "Insertion Sort");
                        break;
                    case 2:
                        measureSortTime(heapSort, arrayToSort, "Heap Sort");
                        break;
                    case 3:
                        measureSortTime(shellSort, arrayToSort, "Shell Sort");
                        break;
                    case 4: {
                        int pivotStrategy;
                        cout << "\nWybierz strategie pivota dla Quick Sort:" << endl;
                        cout << "1. Lewy" << endl;
                        cout << "2. Prawy" << endl;
                        cout << "3. Srodkowy" << endl;
                        cout << "4. Losowy" << endl;
                        cout << "Wybor: ";
                        cin >> pivotStrategy;
                        // Używamy funkcji lambda, by dopasować interfejs funkcji do measureSortTime
                        auto quickSortWrapper = [pivotStrategy](vector<int>& arr) {
                            quickSort(arr, pivotStrategy);
                        };
                        measureSortTime(quickSortWrapper, arrayToSort, "Quick Sort");
                        break;
                    }
                    default:
                        cout << "Nieprawidlowy wybor algorytmu sortowania." << endl;
                        continue;
                }
                cout << "Posortowana tablica:" << endl;
                printArray(arrayToSort);
                break;
            }
            case 5:
                cout << "Koniec programu." << endl;
                runProgram = false;
                break;
            default:
                cout << "Nieprawidlowy wybor, sprobuj ponownie." << endl;
                break;
        }
        cout << endl;
    }

    return 0;
}

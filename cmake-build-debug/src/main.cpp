#include <iostream>
#include <vector>
#include <string>

// Dołączamy nagłówki z katalogu include, dzięki target_include_directories
#include "../include/SortAlgorithms.hpp"
#include "../include/DataGenerator.hpp"

using namespace std;

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

void displaySortMenu() {
    cout << "\nWybierz algorytm sortowania:" << endl;
    cout << "1. Insertion Sort" << endl;
    cout << "2. Heap Sort" << endl;
    cout << "3. Shell Sort" << endl;
    cout << "4. Quick Sort" << endl;
    cout << "Wybor: ";
}

int main() {
    bool runProgram = true;
    int choice = 0;
    vector<int> currentArray;  // Przechowuje aktualną tablicę

    while (runProgram) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string filename;
                cout << "Podaj nazwę pliku: ";
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

                // Utwórz kopię, aby zachować oryginał do ewentualnych testów
                vector<int> arrayToSort = currentArray;

                switch (sortChoice) {
                    case 1:
                        insertionSort(arrayToSort);
                        cout << "Tablica posortowana metodą Insertion Sort:" << endl;
                        break;
                    case 2:
                        heapSort(arrayToSort);
                        cout << "Tablica posortowana metodą Heap Sort:" << endl;
                        break;
                    case 3:
                        shellSort(arrayToSort);
                        cout << "Tablica posortowana metodą Shell Sort:" << endl;
                        break;
                    case 4: {
                        int pivotStrategy;
                        cout << "\nWybierz strategię pivota dla Quick Sort:" << endl;
                        cout << "1. Lewy" << endl;
                        cout << "2. Prawy" << endl;
                        cout << "3. Srodkowy" << endl;
                        cout << "4. Losowy" << endl;
                        cout << "Wybor: ";
                        cin >> pivotStrategy;
                        quickSort(arrayToSort, pivotStrategy);
                        cout << "Tablica posortowana metodą Quick Sort:" << endl;
                        break;
                    }
                    default:
                        cout << "Nieprawidlowy wybor algorytmu sortowania." << endl;
                        continue;
                }
                // Wyświetl posortowaną tablicę
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

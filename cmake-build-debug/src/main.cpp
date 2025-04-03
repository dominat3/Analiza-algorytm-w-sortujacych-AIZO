#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional>  // aby używać std::function

#include "DataGenerator.hpp"   // plik z generatorami i loadArray
#include "SortAlgorithms.hpp"  // plik z algorytmami sortowania

using namespace std;

// -----------------------
// Wyświetlanie menu
// -----------------------
void displayMenu() {
    cout << "----------------------------------------" << endl;
    cout << "         MENU GLOWNE - SORTOWANIE       " << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Wczytaj tablice z pliku" << endl;
    cout << "2. Wygeneruj losowa tablice" << endl;
    cout << "3. Wyswietl aktualna tablice" << endl;
    cout << "4. Sortuj tablice" << endl;
    cout << "5. Wyjdz z programu" << endl;
    cout << "6. Uruchom automatyczne testy" << endl;
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

// -----------------------
// Sprawdzanie, czy wektor jest posortowany rosnąco
// -----------------------
template <typename T>
bool isSorted(const vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

// -----------------------
// runSortTest – sortuje WCIĄŻ TEN SAM zestaw danych w każdej iteracji
// (wykorzystywane m.in. w trybie interaktywnym)
// -----------------------
template <typename T, typename Func>
long long runSortTest(Func sortFunction, const vector<T>& arr, int iterations) {
    long long totalTime = 0;
    for (int i = 0; i < iterations; ++i) {
        vector<T> tmp = arr; // kopiujemy za każdym razem TĘ SAMĄ tablicę
        auto start = chrono::high_resolution_clock::now();
        sortFunction(tmp);
        auto end = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        totalTime += duration;

        if (!isSorted(tmp)) {
            cerr << "Blad: tablica nie jest posortowana!" << endl;
        }
    }
    return totalTime / iterations;
}

// -----------------------
// runSortTestNew – generuje NOWĄ tablicę przy KAŻDEJ iteracji
// (wykorzystywane w batch testach, aby za każdym pomiarem mieć inny zestaw danych)
// -----------------------
template <typename T, typename SortFunc, typename GenFunc>
long long runSortTestNew(SortFunc sortFunction, GenFunc generator, int iterations) {
    long long totalTime = 0;
    for (int i = 0; i < iterations; ++i) {
        vector<T> arr = generator();  // każda iteracja -> nowe dane
        auto start = chrono::high_resolution_clock::now();
        sortFunction(arr);
        auto end = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        totalTime += duration;

        if (!isSorted(arr)) {
            cerr << "Blad: tablica nie jest posortowana!" << endl;
        }
    }
    return totalTime / iterations;
}

// -----------------------
// Batch testy – zmodyfikowane tak, by za każdym pomiarem generować nowe dane
// -----------------------
template <typename T>
void runBatchTestsGeneric(RangeOption rangeOption) {
    // 7 reprezentatywnych rozmiarów tablic
    vector<int> sizes = { 10000, 20000, 30000, 40000, 50000, 60000, 70000 };
    int iterations = 10; // liczba powtórzeń

    // Wrappery dla insertion, heap
    auto insertionSortWrapper = [](vector<T>& arr){ insertionSort(arr); };
    auto heapSortWrapper      = [](vector<T>& arr){ heapSort(arr); };

    // Dla Shell Sort 2 strategie
    auto shellSortWrapper1 = [](vector<T>& arr){ shellSort(arr, 1); };
    auto shellSortWrapper2 = [](vector<T>& arr){ shellSort(arr, 2); };

    // Quick Sort – pivot=1..4
    auto quickSortForPivot = [&](int p) {
        return [=](vector<T>& arr){ quickSort(arr, p); };
    };

    for (int size : sizes) {
        cout << "\n======================================" << endl;
        cout << "Test dla tablicy o rozmiarze: " << size << endl;
        cout << "======================================" << endl;

        // Tworzymy "generatory" poszczególnych wariantów tablic
        // Każdy generator zwraca NOWĄ tablicę (wywołanie generator() -> nowy wektor)
        auto genRandom = [=]() -> vector<T> {
            return generateRandomArray<T>(size, rangeOption);
        };

        auto genSorted = [=]() -> vector<T> {
            vector<T> arr = generateRandomArray<T>(size, rangeOption);
            insertionSort(arr);
            return arr;
        };

        auto genReverse = [=]() -> vector<T> {
            vector<T> arr = generateRandomArray<T>(size, rangeOption);
            insertionSort(arr);
            reverse(arr.begin(), arr.end());
            return arr;
        };

        auto genPartial33 = [=]() -> vector<T> {
            vector<T> arr = generateRandomArray<T>(size, rangeOption);
            int index33 = size * 33 / 100;
            vector<T> temp(arr.begin(), arr.begin() + index33);
            insertionSort(temp);
            for (int i = 0; i < index33; ++i) {
                arr[i] = temp[i];
            }
            return arr;
        };

        auto genPartial66 = [=]() -> vector<T> {
            vector<T> arr = generateRandomArray<T>(size, rangeOption);
            int index66 = size * 66 / 100;
            vector<T> temp(arr.begin(), arr.begin() + index66);
            insertionSort(temp);
            for (int i = 0; i < index66; ++i) {
                arr[i] = temp[i];
            }
            return arr;
        };

        // Warianty w formie: (nazwa, generator)
        vector<pair<string, function<vector<T>()>>> variants = {
            {"Losowa",              genRandom},
            {"Posortowana rosnaco", genSorted},
            {"Posortowana malejaco", genReverse},
            {"Czesciowo (33%)",     genPartial33},
            {"Czesciowo (66%)",     genPartial66},
        };

        // Dla każdego wariantu wykonujemy 8 testów (Insertion, Heap, Shell(2), Quick(4))
        for (auto& variant : variants) {
            cout << "\n--- Tablica " << variant.first << " ---" << endl;

            // 1) Insertion Sort
            long long tIns = runSortTestNew<T>(
                insertionSortWrapper,
                variant.second,  // generator
                iterations
            );
            cout << "Insertion Sort: " << tIns << " ms" << endl;

            // 2) Heap Sort
            long long tHeap = runSortTestNew<T>(
                heapSortWrapper,
                variant.second,
                iterations
            );
            cout << "Heap Sort: " << tHeap << " ms" << endl;

            // 3) Shell Sort (gap=1 klasyczne)
            long long tShell1 = runSortTestNew<T>(
                shellSortWrapper1,
                variant.second,
                iterations
            );
            cout << "Shell Sort (gap=1 klasyczne): " << tShell1 << " ms" << endl;

            // 4) Shell Sort (gap=2 Ciura)
            long long tShell2 = runSortTestNew<T>(
                shellSortWrapper2,
                variant.second,
                iterations
            );
            cout << "Shell Sort (gap=2 Franka i Lazarusa): " << tShell2 << " ms" << endl;

            // 5..8) Quick Sort dla pivotów 1..4
            for (int pivotStrategy : {1, 2, 3, 4}) {
                auto quickLambda = quickSortForPivot(pivotStrategy);
                long long tQuick = runSortTestNew<T>(
                    quickLambda,
                    variant.second,  // generator
                    iterations
                );
                cout << "Quick Sort (pivot=" << pivotStrategy << "): " << tQuick << " ms" << endl;
            }
        }
    }
}

// ----------------------------------------------------------
// Interfejs interaktywny dla typu int
// ----------------------------------------------------------
void runInteractiveInt(RangeOption rangeOption) {
    bool runProgram = true;
    int choice = 0;
    vector<int> currentArray;

    while (runProgram) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string filename;
                cout << "Podaj nazwę pliku: ";
                cin >> filename;
                currentArray = loadArray<int>(filename);
                if (!currentArray.empty())
                    cout << "Tablica wczytana z pliku." << endl;
                else
                    cout << "Wystąpił błąd podczas wczytywania tablicy." << endl;
                break;
            }
            case 2: {
                int size;
                cout << "Podaj rozmiar tablicy: ";
                cin >> size;
                currentArray = generateRandomArray<int>(size, rangeOption);
                cout << "Losowa tablica utworzona." << endl;
                break;
            }
            case 3: {
                if (!currentArray.empty()) {
                    cout << "Aktualna tablica:" << endl;
                    printArray<int>(currentArray);
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
                vector<int> arrayToSort = currentArray;

                switch (sortChoice) {
                    case 1: {
                        long long t = runSortTest<int>(
                            [](vector<int>& arr){ insertionSort(arr); },
                            arrayToSort,
                            1
                        );
                        cout << "Insertion Sort: " << t << " ms" << endl;
                        insertionSort(arrayToSort);
                        break;
                    }
                    case 2: {
                        long long t = runSortTest<int>(
                            [](vector<int>& arr){ heapSort(arr); },
                            arrayToSort,
                            1
                        );
                        cout << "Heap Sort: " << t << " ms" << endl;
                        heapSort(arrayToSort);
                        break;
                    }
                    case 3: {
                        int shellStrategy;
                        cout << "\nWybierz sekwencje dla Shell Sort:" << endl;
                        cout << "1 - klasyczna (gap = n/2)" << endl;
                        cout << "2 - Franka i Lazarusa" << endl;
                        cout << "Wybor: ";
                        cin >> shellStrategy;

                        auto shellLambda = [shellStrategy](vector<int>& arr){ shellSort(arr, shellStrategy); };
                        long long t = runSortTest<int>(shellLambda, arrayToSort, 1);
                        cout << "Shell Sort: " << t << " ms" << endl;
                        shellSort(arrayToSort, shellStrategy);
                        break;
                    }
                    case 4: {
                        int pivotStrategy;
                        cout << "\nWybierz strategię pivota dla Quick Sort:" << endl;
                        cout << "1. Lewy" << endl;
                        cout << "2. Prawy" << endl;
                        cout << "3. Srodkowy" << endl;
                        cout << "4. Losowy" << endl;
                        cout << "Wybor: ";
                        cin >> pivotStrategy;

                        auto quickLambda = [pivotStrategy](vector<int>& arr){ quickSort(arr, pivotStrategy); };
                        long long t = runSortTest<int>(quickLambda, arrayToSort, 1);
                        cout << "Quick Sort: " << t << " ms" << endl;
                        quickSort(arrayToSort, pivotStrategy);
                        break;
                    }
                    default:
                        cout << "Nieprawidlowy wybor algorytmu." << endl;
                        continue;
                }
                cout << "Posortowana tablica:" << endl;
                printArray<int>(arrayToSort);
                break;
            }
            case 5:
                cout << "Koniec programu." << endl;
                runProgram = false;
                break;
            case 6:
                cout << "Uruchamianie automatycznych testow dla typu int..." << endl;
                runBatchTestsGeneric<int>(rangeOption);
                break;
            default:
                cout << "Nieprawidlowy wybor, sprobuj ponownie." << endl;
                break;
        }
        cout << endl;
    }
}

// ----------------------------------------------------------
// Interfejs interaktywny dla typu float
// ----------------------------------------------------------
void runInteractiveFloat(RangeOption rangeOption) {
    bool runProgram = true;
    int choice = 0;
    vector<float> currentArray;

    while (runProgram) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string filename;
                cout << "Podaj nazwę pliku: ";
                cin >> filename;
                currentArray = loadArray<float>(filename);
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
                currentArray = generateRandomArray<float>(size, rangeOption);
                cout << "Losowa tablica utworzona." << endl;
                break;
            }
            case 3: {
                if (!currentArray.empty()) {
                    cout << "Aktualna tablica:" << endl;
                    printArray<float>(currentArray);
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
                vector<float> arrayToSort = currentArray;

                switch (sortChoice) {
                    case 1: {
                        long long t = runSortTest<float>(
                            [](vector<float>& arr){ insertionSort(arr); },
                            arrayToSort,
                            1
                        );
                        cout << "Insertion Sort: " << t << " ms" << endl;
                        insertionSort(arrayToSort);
                        break;
                    }
                    case 2: {
                        long long t = runSortTest<float>(
                            [](vector<float>& arr){ heapSort(arr); },
                            arrayToSort,
                            1
                        );
                        cout << "Heap Sort: " << t << " ms" << endl;
                        heapSort(arrayToSort);
                        break;
                    }
                    case 3: {
                        int shellStrategy;
                        cout << "\nWybierz sekwencje dla Shell Sort:" << endl;
                        cout << "1 - klasyczna (gap = n/2)" << endl;
                        cout << "2 - Franka i Lazarus" << endl;
                        cout << "Wybor: ";
                        cin >> shellStrategy;

                        auto shellLambda = [shellStrategy](vector<float>& arr){ shellSort(arr, shellStrategy); };
                        long long t = runSortTest<float>(shellLambda, arrayToSort, 1);
                        cout << "Shell Sort: " << t << " ms" << endl;
                        shellSort(arrayToSort, shellStrategy);
                        break;
                    }
                    case 4: {
                        int pivotStrategy;
                        cout << "\nWybierz strategię pivota dla Quick Sort:" << endl;
                        cout << "1. Lewy" << endl;
                        cout << "2. Prawy" << endl;
                        cout << "3. Srodkowy" << endl;
                        cout << "4. Losowy" << endl;
                        cout << "Wybor: ";
                        cin >> pivotStrategy;

                        auto quickLambda = [pivotStrategy](vector<float>& arr){ quickSort(arr, pivotStrategy); };
                        long long t = runSortTest<float>(quickLambda, arrayToSort, 1);
                        cout << "Quick Sort: " << t << " ms" << endl;
                        quickSort(arrayToSort, pivotStrategy);
                        break;
                    }
                    default:
                        cout << "Nieprawidlowy wybor algorytmu." << endl;
                        continue;
                }
                cout << "Posortowana tablica:" << endl;
                printArray<float>(arrayToSort);
                break;
            }
            case 5:
                cout << "Koniec programu." << endl;
                runProgram = false;
                break;
            case 6:
                cout << "Uruchamianie automatycznych testow dla typu float..." << endl;
                runBatchTestsGeneric<float>(rangeOption);
                break;
            default:
                cout << "Nieprawidlowy wybor, sprobuj ponownie." << endl;
                break;
        }
        cout << endl;
    }
}

// ----------------------------------------------------------
// Funkcja main – wybór typu danych i zakresu
// ----------------------------------------------------------
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int typeChoice;
    cout << "Wybierz typ danych do sortowania:" << endl;
    cout << "1 - int" << endl;
    cout << "2 - float" << endl;
    cout << "Wybor: ";
    cin >> typeChoice;

    int rangeChoice;
    cout << "\nWybierz zakres generowanych danych:" << endl;
    cout << "1 - Ograniczony (0 do 100)" << endl;
    cout << "2 - Pelny zakres dla danego typu" << endl;
    cout << "Wybor: ";
    cin >> rangeChoice;

    RangeOption rangeOption = (rangeChoice == 1 ? RangeOption::Limited : RangeOption::Full);

    if (typeChoice == 1) {
        cout << "\nWybrano typ int.\n" << endl;
        runInteractiveInt(rangeOption);
    } else if (typeChoice == 2) {
        cout << "\nWybrano typ float.\n" << endl;
        runInteractiveFloat(rangeOption);
    } else {
        cout << "Nieprawidlowy wybor typu. Domyslnie ustawiono int." << endl;
        runInteractiveInt(rangeOption);
    }

    return 0;
}

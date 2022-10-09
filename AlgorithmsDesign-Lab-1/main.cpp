#include <iostream>
#include "ExternalSortEngine.h"
#include <fstream>
#include <ctime>

int main()
{
    const char* pathA{ "a.dat" }, * pathB{ "b.dat" }, * pathC{ "c.dat" };

    ExternalSortEngine se(pathA, pathB, pathC);

    se.generateRandomA(10 * 1024 * 1024 / 4);

    time_t tStart = time(0);
    se.sort();
    time_t tEnd = time(0);

    /*std::cout << "A:\t"; 
    se.print10(pathA);*/

    if (se.isSorted()) {
        std::cout << "Sorting is successful.\n";
    }
    std::cout << "Sorting time:\t" << (tEnd - tStart) << " secs\n";

    /*std::fstream fileA, fileA1;

    int a[1 * 1024 * 1024 / 8]{}, a1[1 * 1024 * 1024 / 8]{};
    fileA.open("a.dat", std::ios::binary | std::ios::in);
    fileA1.open("a1.dat", std::ios::binary);

    int curr{};

    for (long long i{ 0 }; i < 1 * 1024 * 1024 / 8; ++i) {
        fileA.read((char*)&curr, sizeof(curr));
        a[curr] += 1;
        fileA1.read((char*)&curr, sizeof(curr));
        a1[curr] += 1;
    }

    for (int i{ 0 }; i < 1 * 1024 * 1024 / 8; ++i) {
        if (a[i] != a1[i]) {
            std::cout << i << ": " << a[i] << ", FAIL!!!\n";
            break;
        }
    }

    fileA.close();
    fileA1.close();*/

    return 0;
}
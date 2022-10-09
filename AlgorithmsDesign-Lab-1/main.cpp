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

    if (se.isSorted()) {
        std::cout << "Sorting is successful.\n";
    }
    std::cout << "Sorting time:\t" << (tEnd - tStart) << " secs\n";

    return 0;
}
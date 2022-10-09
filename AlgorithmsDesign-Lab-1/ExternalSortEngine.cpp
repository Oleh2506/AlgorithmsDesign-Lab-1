#include "ExternalSortEngine.h"

#include "ExternalSortEngine.h"
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>

ExternalSortEngine::ExternalSortEngine(const char* pathA, const char* pathB, const char* pathC) {

	strcpy_s(mPathA, pathA);
	strcpy_s(mPathB, pathB);
	strcpy_s(mPathC, pathC);
}

// Перевірка на відсортованість в неспадному порядку чисел файлу A
bool ExternalSortEngine::isSorted() {

	std::fstream inA;
	inA.open(mPathA, std::ios::binary | std::ios::in);
	bool isSorted{ true };
	int next{}, curr{};

	inA.read((char*)&curr, sizeof(curr));
	while (!inA.eof()) {

		inA.read((char*)&next, sizeof(next));
		if (!inA.eof()) {

			if (next < curr) {
				isSorted = false;
				break;
			}

			curr = next;
		}
	}

	inA.close();
	return isSorted;
}

// Сортування набору чисел з файлу A в неспадному порядку
void ExternalSortEngine::sort() {

	while (!(this->split())) {

		/*std::cout << "A:\t";
		print10(mPathA);
		std::cout << "B:\t";
		print10(mPathB);
		std::cout << "C:\t";
		print10(mPathC);
		std::cout << "\n";*/

		this->merge();
	}
}

void ExternalSortEngine::print10(const char* filePath) {

	std::fstream fin;
	fin.open(filePath, std::ios::binary | std::ios::in);
	int curr{};

	fin.read((char*)&curr, sizeof(curr));
	for (int i = 0; i < 10 && !fin.eof(); ++i) {

		std::cout << curr << "\t";
		fin.read((char*)&curr, sizeof(curr));
	}
	std::cout << "\n";

	fin.close();
}

// Розбиття файлу A на файли B і C, в які будуть послідовно записані "серії" з вихідного файлу;
// "Серія" -- неспадна послідовність чисел з вихідного файлу
bool ExternalSortEngine::split() {

	std::fstream inA, outB, outC;
	inA.open(mPathA, std::ios::binary | std::ios::in);
	outB.open(mPathB, std::ios::binary | std::ios::out | std::ios::trunc);
	outC.open(mPathC, std::ios::binary | std::ios::out | std::ios::trunc);

	bool writeToB{ true };
	int next{}, curr{};
	bool isSorted{ true };

	inA.read((char*)&curr, sizeof(curr));
	while (!inA.eof()) {

		if (writeToB) {
			outB.write((char*)&curr, sizeof(curr));
		}
		else {
			outC.write((char*)&curr, sizeof(curr));
		}

		inA.read((char*)&next, sizeof(next));
		if (!inA.eof()) {

			if (next < curr) {
				writeToB = !writeToB;
				isSorted = false;
			}

			curr = next;
		}
	}

	curr = INT_MIN;
	outB.write((char*)&curr, sizeof(curr));
	outC.write((char*)&curr, sizeof(curr));

	inA.close();
	outB.close();
	outC.close();

	return isSorted;
}

// Злиття відповідних серій з файлів B і C в файл A
void ExternalSortEngine::merge() {

	std::fstream outA, inB, inC;
	outA.open(mPathA, std::ios::binary | std::ios::out | std::ios::trunc);
	inB.open(mPathB, std::ios::binary | std::ios::in);
	inC.open(mPathC, std::ios::binary | std::ios::in);
	int currB{}, currC{}, nextB{}, nextC{};

	inB.read((char*)&currB, sizeof(currB));
	inC.read((char*)&currC, sizeof(currC));
	inB.read((char*)&nextB, sizeof(nextB));
	inC.read((char*)&nextC, sizeof(nextC));

	do {
		// Якщо поточні елементи файлів B і C не є останніми в своїх серіях,
		// то записуємо менший з них до файлу A і зчитуємо наступний елемент відповідної серії
		if ((currB <= nextB) && (currC <= nextC)) {

			if (currB <= currC) {
				outA.write((char*)&currB, sizeof(currB));
				currB = nextB;
				inB.read((char*)&nextB, sizeof(nextB));
			}
			else {
				outA.write((char*)&currC, sizeof(currC));
				currC = nextC;
				inC.read((char*)&nextC, sizeof(nextC));
			}
		}
		else {
			// Якщо поточний елемент з файлу B є останнім в своїй серії, 
			// а поточний елемент з файлу C не є останнім в своїй серії,
			// то якщо елемент файлу B < елемент файлу C, то спершу записуємо його
			// до файлу A і зчитуємо наступний елемент файлу B, 
			// а після записуємо всі елементи серії файлу C до файлу A
			// і зчитуємо наступний елемент файлу C,
			// інакше, записуємо поточний елемент файлу C в файл A
			// і зчитуємо наступний елемент файлу C
			if ((currB > nextB) && (currC <= nextC)) {
				if (currB <= currC) {

					outA.write((char*)&currB, sizeof(currB));
					currB = nextB;
					inB.read((char*)&nextB, sizeof(nextB));

					while ((currC <= nextC) && !inC.eof()) {
						outA.write((char*)&currC, sizeof(currC));
						currC = nextC;
						inC.read((char*)&nextC, sizeof(nextC));
					}
				}
				else {
					outA.write((char*)&currC, sizeof(currC));
					currC = nextC;
					inC.read((char*)&nextC, sizeof(nextC));
				}
			}
			else {
				// Якщо поточний елемент з файлу C є останнім в своїй серії, 
				// а поточний елемент з файлу B не є останнім в своїй серії,
				// то якщо елемент файлу C < елемент файлу B, то спершу записуємо його
				// до файлу A і зчитуємо наступний елемент файлу C, 
				// а після записуємо всі елементи серії файлу B до файлу A
				// і зчитуємо наступний елемент файлу B,
				// інакше, записуємо поточний елемент файлу B в файл A
				// і зчитуємо наступний елемент файлу B
				if ((currB <= nextB) && (currC > nextC)) {
					if (currC <= currB) {

						outA.write((char*)&currC, sizeof(currC));
						currC = nextC;
						inC.read((char*)&nextC, sizeof(nextC));

						while ((currB <= nextB) && !inB.eof()) {
							outA.write((char*)&currB, sizeof(currB));
							currB = nextB;
							inB.read((char*)&nextB, sizeof(nextB));
						}
					}
					else {
						outA.write((char*)&currB, sizeof(currB));
						currB = nextB;
						inB.read((char*)&nextB, sizeof(nextB));
					}
				}
				else {
					// Якщо поточні елементи файлів A та B є останніми в своїх серіях, 
					// то спершу записуємо менший з них до файлу A, а після більший;
					// далі зчитуємо наступні елементи обох файлів
					if (currB <= currC) {
						outA.write((char*)&currB, sizeof(currB));
						outA.write((char*)&currC, sizeof(currC));
					}
					else {
						outA.write((char*)&currC, sizeof(currC));
						outA.write((char*)&currB, sizeof(currB));
					}

					currC = nextC;
					currB = nextB;
					inB.read((char*)&nextB, sizeof(nextB));
					inC.read((char*)&nextC, sizeof(nextC));
				}
			}
		}
	} while (!inB.eof() && !inC.eof());

	// Якщо в файлі B залишилися елементи, записуємо їх до файлу A
	if (!inB.eof()) {

		while (!inB.eof()) {
			outA.write((char*)&currB, sizeof(currB));
			currB = nextB;
			inB.read((char*)&nextB, sizeof(nextB));
		}
	}
	// Якщо в файлі C залишилися елементи, записуємо їх до файлу A
	if (!inC.eof()) {

		while (!inC.eof()) {
			outA.write((char*)&currC, sizeof(currC));
			currC = nextC;
			inC.read((char*)&nextC, sizeof(nextC));
		}
	}

	outA.close();
	inB.close();
	inC.close();
}

// Генерація набору випадкових aLength випадкових чисел, що будуть записані до файлу A
void ExternalSortEngine::generateRandomA(long long aLength) {

	std::fstream fileA, fileA1;

	std::random_device rd;
	std::default_random_engine defEngine(time(NULL));
	std::uniform_int_distribution<int> dist(INT_MIN + 1, INT_MAX - 1);

	fileA.open(mPathA, std::ios::binary | std::ios::out | std::ios::trunc);
	//fileA1.open("a1.dat", std::ios::binary | std::ios::out | std::ios::trunc);

	for (int i{ 0 }; i < aLength; ++i) {

		int curr = dist(defEngine);
		fileA.write((char*)&curr, sizeof(curr));
		fileA1.write((char*)&curr, sizeof(curr));
	}

	fileA.close();
	fileA1.close();
}
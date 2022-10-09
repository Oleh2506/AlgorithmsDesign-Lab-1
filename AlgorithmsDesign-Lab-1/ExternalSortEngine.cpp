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

	if (!inB.eof()) {

		while (!inB.eof()) {
			outA.write((char*)&currB, sizeof(currB));
			currB = nextB;
			inB.read((char*)&nextB, sizeof(nextB));
		}
	}
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
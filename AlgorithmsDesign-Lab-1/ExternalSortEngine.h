#pragma once

const int MAX_PATH_SIZE{ 50 };

class ExternalSortEngine
{
private:
	char mPathA[MAX_PATH_SIZE], mPathB[MAX_PATH_SIZE], mPathC[MAX_PATH_SIZE];

	// Розбиття файлу A на файли B і C, в які будуть послідовно записані "серії" з вихідного файлу;
	// "Серія" -- неспадна послідовність чисел з вихідного файлу
	bool split();
	// Злиття відповідних серій з файлів B і C в файл A
	void merge();

public:
	ExternalSortEngine(const char* pathA, const char* pathB, const char* pathC);
	// Сортування набору чисел з файлу A в неспадному порядку
	void sort();
	// Генерація набору випадкових aLength випадкових чисел, що будуть записані до файлу A
	void generateRandomA(long long aLength);
	// Перевірка на відсортованість в неспадному порядку чисел файлу A
	bool isSorted();
	void print10(const char* filePath);
};



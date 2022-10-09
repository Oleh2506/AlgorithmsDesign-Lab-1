#pragma once

const int MAX_PATH_SIZE{ 50 };

class ExternalSortEngine
{
private:
	char mPathA[MAX_PATH_SIZE], mPathB[MAX_PATH_SIZE], mPathC[MAX_PATH_SIZE];

	bool split();
	void merge();
	void print10(const char* filePath);

public:
	ExternalSortEngine(const char* pathA, const char* pathB, const char* pathC);
	void sort();
	void generateRandomA(long long aLength);
	bool isSorted();
};



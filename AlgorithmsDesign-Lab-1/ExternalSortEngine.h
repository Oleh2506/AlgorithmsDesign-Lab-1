#pragma once

const int MAX_PATH_SIZE{ 50 };

class ExternalSortEngine
{
private:
	char mPathA[MAX_PATH_SIZE], mPathB[MAX_PATH_SIZE], mPathC[MAX_PATH_SIZE];

	// �������� ����� A �� ����� B � C, � �� ������ ��������� ������� "���" � ��������� �����;
	// "����" -- �������� ����������� ����� � ��������� �����
	bool split();
	// ������ ��������� ���� � ����� B � C � ���� A
	void merge();

public:
	ExternalSortEngine(const char* pathA, const char* pathB, const char* pathC);
	// ���������� ������ ����� � ����� A � ���������� �������
	void sort();
	// ��������� ������ ���������� aLength ���������� �����, �� ������ ������� �� ����� A
	void generateRandomA(long long aLength);
	// �������� �� ������������� � ���������� ������� ����� ����� A
	bool isSorted();
	void print10(const char* filePath);
};



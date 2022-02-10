#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class CSV {

private:
	unsigned int rowMax = 0;
	unsigned int columnMax = 0;

	std::vector<std::vector<std::string>> csvString;
public:
	void LoadCSV(std::string _filepath);

	std::string GetString(unsigned int row=0, unsigned int column=0) {
		std::string errorStr;
		if (row >= rowMax) {
			errorStr = "�w��̗�T�C�Y�����߂��Ă��܂�";
			return errorStr;
		}		
		if (column >= columnMax) {
			errorStr = "�w��̍s�T�C�Y�����߂��Ă��܂�";
			return errorStr;
		}
		return csvString[row][column];
	}
	unsigned int GetRowMax() { return rowMax; }
	unsigned int GetColumnMax() { return columnMax; }

};


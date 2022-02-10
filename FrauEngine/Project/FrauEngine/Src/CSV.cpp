#include "CSV.h"

using namespace std;
void CSV::LoadCSV(string _filepath) {

	ifstream csvFile(_filepath);
	
	unsigned int rowNum = 0;
	unsigned int columnNum = 0;
	//配列サイズの確認
	string tmp;
	while (getline(csvFile, tmp)) {	
		istringstream i_stream(tmp);
		
		while (rowNum == 0 && getline(i_stream, tmp, ',')) {
			columnNum++;
		}
		rowNum++;
	}
	rowMax = rowNum;
	columnMax = columnNum;
	//配列のサイズ確定(PushBackをやりすぎると重くなるので先にサイズを確定させる)
	csvString.resize(rowMax);
	for (int i = 0; i < rowMax; i++) {
		csvString[i].resize(columnNum);
	}
	//文字列を読み込む
	rowNum = 0;
	columnNum = 0;
	csvFile.clear();
	csvFile.seekg(0);
	while (getline(csvFile, tmp)) {
		istringstream i_stream(tmp);

		columnNum = 0;
		while (getline(i_stream, tmp, ',')) {
			csvString[rowNum][columnNum] = tmp;
			columnNum++;
		}
		rowNum++;
	}
}

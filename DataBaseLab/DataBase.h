#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <iomanip>
#include <direct.h>

using namespace std;

class DataBase {
private:
	string tableName;
	string tableFileName;
	string tableConfigFileName;
	vector< pair<string, int> > tableFields;
	vector<bool> tableFieldsIndexExist;
	int tupleLength;

private:
	void createIndexFile(string fieldName, int fieldLength, int indexFileSize);
	int calculateIndexHash(string fieldName, int tableSize);
public:
	bool createTable(string table);
	bool openTable(string table);
	int selectAll();
	bool insert(vector<string> data);
	string select(int line);
};
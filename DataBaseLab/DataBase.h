#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <iomanip>
#include <experimental/filesystem>
#include <direct.h>

using namespace std;

class DataBase {
private:
	int tableCapacity = 100000;
	int tableCurrentSize;
	string tableName;
	string tableFileName;
	string tableConfigFileName;
	vector< pair<string, int> > tableFields;
	vector<bool> tableFieldsIndexExist;
	int tupleLength;

private:
	void createIndexFile(string fieldName, int fieldLength, int indexFileSize);
	int calculateIndexHash(string fieldName, int tableCapacity);
	void changeTableCurrentSizeInFile(int curSize);
public:
	bool createTable(string table);
	bool openTable(string table);
	bool insert(vector<string> data);
	int selectAll();
	vector<string> select(int line);
	vector<int> selectWhere(string field, string value);
	void updateWhere(string field, string value, string newValue);
	void deleteWhere(string field, string value);
	void dropTable(string table);
};
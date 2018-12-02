#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <iomanip>

using namespace std;

class DataBase {
private:
	string tableName;
	string tableFileName;
	string tableConfigFileName;
	vector< pair<string, int> > tableFields;
	int tupleLength;
public:
	bool openTable(string table);
	bool insert(vector<string> data);
	string select(int line);
};
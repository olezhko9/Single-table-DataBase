#include "DataBase.h"
std::vector<std::string> split(const std::string& s, char delimiter);

bool DataBase::openTable(string table) {
	tableName = table;
	tableFileName = table + "\\" + table + ".db";
	tableConfigFileName = table + "\\" + table + ".cfg";
	fstream tabeConfigFile(tableConfigFileName, std::fstream::in);

	tupleLength = 0;

	if (tabeConfigFile.is_open()) {
		string fieldName;
		int fieldLength;
		while (tabeConfigFile >> fieldName >> fieldLength) {
			pair<string, int> p(fieldName, fieldLength);
			tableFields.push_back(p);

			// вычисляем длину в байтах каждой записи в таблице
			tupleLength += fieldLength + 1;
		}
		tupleLength += 2;
	}
	else {
		return false;
	}

	/*for (int i = 0; i < tableFields.size(); i++) {
		cout << tableFields[i].first << tableFields[i].second << endl;
	}*/
	return true;
}

bool DataBase::insert(vector<string> data) {
	fstream tableFile(tableFileName, std::fstream::app);

	if (tableFile.is_open()) {
		for (int i = 0; i < data.size(); i++) {
			//cout << tableFields[i].second << endl;
			tableFile << setw(tableFields[i].second) << data[i] << ",";
		}
		tableFile << "\n";
	}
	else {
		return false;
	}
	return true;
}

string DataBase::select(int line) {
	fstream tableFile(tableFileName, std::fstream::in);
	int k = 2;
	char *tuple = new char[tupleLength - k];

	if (tableFile.is_open()) {
		tableFile.seekg((line - 1) * tupleLength, 0);
		//tableFile.read(tuple, tupleLength - k);
		tableFile.getline(tuple, tupleLength - k);
	}

	string toreturn(tuple);
	/*vector<string> vs = split(toreturn, ',');
	for (auto v : vs) {
		cout << v << endl;
	}*/
	return toreturn;
}



std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}
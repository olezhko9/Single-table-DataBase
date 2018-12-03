#include "DataBase.h"
std::vector<std::string> split(const std::string& s, char delimiter);

bool DataBase::createTable(string table) {
	tableName = table;
	_mkdir(table.c_str());
	
	tableConfigFileName = tableName + "\\" + tableName + ".cfg";
	fstream tabeConfigFile(tableConfigFileName, std::fstream::out);
	cout << "Введите имена полей, их длину и укажите true, если создавать индекс для этого поля, иначе false.\n"
		<< "Для выхода введите нулевую длину." << endl;

	if (tabeConfigFile.is_open()) {
		string fieldName, fieldIndex;
		int fieldLength;
		while (cin >> fieldName >> fieldLength >> fieldIndex) {
			if (fieldLength == 0) break;
			else {
				tabeConfigFile << fieldName << " " << fieldLength << " " << fieldIndex << endl;
				if (fieldIndex.compare("true") == 0) {
					createIndexFile(fieldName, fieldLength, 100000);
				}
			}
		}
	} 
	else {
		return false;
	}

	tableConfigFileName = "";
	tableName = "";
	return true;
}

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
	return true;
}


int DataBase::selectAll() {
	fstream tableFile(tableFileName, std::fstream::in);
	int tupleCount = 0;

	if (tableFile.is_open()) {
		for (int i = 0; i < tableFields.size(); i++) {
			cout << setw(tableFields[i].second) << tableFields[i].first << " ";
		}
		cout << endl;
		string tuple;
		while (getline(tableFile, tuple)) {
			tupleCount++;
			cout << tuple << endl;
		}
	}
	return tupleCount;
}


bool DataBase::insert(vector<string> data) {
	fstream tableFile(tableFileName, std::fstream::app);

	if (tableFile.is_open()) {
		for (int i = 0; i < data.size(); i++) {
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
		tableFile.getline(tuple, tupleLength - k);
	}

	string toreturn(tuple);
	/*vector<string> vs = split(toreturn, ',');
	for (auto v : vs) {
		cout << v << endl;
	}*/
	return toreturn;
}


void DataBase::createIndexFile(string fieldName, int fieldLength, int indexFileSize) {
	string indexFileName = tableName + "\\" + fieldName + ".idx";
	fstream indexFile(indexFileName, std::fstream::out);

	int recordLength = fieldLength + 1 + to_string(indexFileSize).size();
	string emptyRecord;
	for (int i = 0; i < recordLength; i++) {
		emptyRecord.append(" ");
	}
	emptyRecord.append("\n");
	
	for (int i = 0; i < indexFileSize; i++) {
		indexFile << emptyRecord;
	}
	indexFile.close();
}


// Алгоритм хэширования строк, которое используется для быстрого доступа к данным по ключевым полям.
// Полное описание алгоритма http://e-maxx.ru/algo/string_hashes
int DataBase::calculateIndexHash(string fieldName, int tableSize) {
	int p = 73; // простое число, примерно равное количеству символов во входном алфавите (прописные и строчные буквы и цифры)
	unsigned long long hash = 0, power = 1;
	for (int i = 0; i < fieldName.length(); i++) {
		hash += (fieldName[i] - 'a' + 1) * power;
	}
	return hash % tableSize;
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
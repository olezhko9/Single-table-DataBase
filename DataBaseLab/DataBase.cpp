#include "DataBase.h"


bool DataBase::createTable(string table) {
	tableName = table;
	_mkdir(table.c_str());
	
	tableConfigFileName = tableName + "\\" + tableName + ".cfg";
	fstream tableConfigFile(tableConfigFileName, std::fstream::out);

	if (tableConfigFile.is_open()) {
		tableCurrentSize = 0;
		tableConfigFile << tableCurrentSize << endl;

		string fieldName, fieldIndex;
		int fieldLength;
		// записываем информацию о колонках таблицы в конфиг файл
		while (cin >> fieldName >> fieldLength >> fieldIndex) {
			if (fieldLength == 0) break;
			else {
				tableConfigFile << fieldName << " " << fieldLength << " " << fieldIndex << endl;
				// создаем таблицу индексов дл€ пол€
				if (fieldIndex.compare("true") == 0) {
					createIndexFile(fieldName, fieldLength, tableCapacity);
				}
			}
		}
	} 
	else {
		return false;
	}

	tableFileName = tableName + "\\" + tableName + ".db";
	fstream tableFile(tableFileName, std::fstream::out);
	tableFile.close();

	tableConfigFileName = "";
	tableName = "";
	return true;
}

bool DataBase::openTable(string table) {
	tableName = table;
	tableFileName = table + "\\" + table + ".db";
	tableConfigFileName = table + "\\" + table + ".cfg";
	fstream tableConfigFile(tableConfigFileName, std::fstream::in);

	tupleLength = 0;

	if (tableConfigFile.is_open()) {
		tableConfigFile >> tableCurrentSize;
		string fieldName;
		int fieldLength;
		string filedIndexExists;
		while (tableConfigFile >> fieldName >> fieldLength >> filedIndexExists) {
			pair<string, int> p(fieldName, fieldLength);
			tableFields.push_back(p);
			if (filedIndexExists.compare("true") == 0)
				tableFieldsIndexExist.push_back(true);
			else tableFieldsIndexExist.push_back(false);
			// вычисл€ем длину в байтах каждой записи в таблице
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
	fstream tableFile(tableFileName, std::fstream::in);

	if (tableFile.is_open()) {
		// провер€ем на уникальность по ключевому полю
		if (selectWhere(tableFields[0].first, data[0]).size() != 0) {
			return false;
		}
		else {
			tableCurrentSize += 1;
			// записываем данные в каждое поле
			for (int i = 0; i < data.size(); i++) {
				// если по этому полю есть таблица индексов, то записываем в нее изменени€
				if (tableFieldsIndexExist[i] == true) {
					string indexFileName = tableName + "\\" + tableFields[i].first + ".idx";
					fstream indexFile(indexFileName, std::fstream::in | std::fstream::out);

					if (indexFile.is_open()) {
						// ќпредел€ем размер записи в таблице индексов
						string record;
						getline(indexFile, record);
						int recordSize = record.size() + 2;

						// ”знаем, на какой примерно строке находитс€ запись в индексе
						int fieldHash = calculateIndexHash(data[i], tableCapacity);

						// ƒелаем запись в таблице индексов в первом пустом месте
						indexFile.seekp((fieldHash - 1) * recordSize, 0);
						while (true) {
							getline(indexFile, record);
							string fieldValue;
							int idx;
							stringstream ss_in;
							ss_in << record;
							ss_in >> fieldValue >> idx;
							// если строка с записью пуста€, то записываем
							if (fieldValue.compare("") == 0) {
								indexFile.seekp(long(indexFile.tellp()) - recordSize, 0);
								stringstream ss_out;
								ss_out << setw(tableFields[i].second) << data[i] << " "
									<< setw(to_string(tableCapacity).size()) << tableCurrentSize << endl;

								indexFile.write(ss_out.str().c_str(), recordSize - 2);
								break;
							}
						}
					}
				}
			}
		}
		tableFile.close();
	}
	else {
		return false;
	}

	// вставл€ем в файл таблицы в последнюю строку
	fstream tableFileDB(tableFileName, std::fstream::app);
	if (tableFileDB.is_open()) {
		for (int i = 0; i < data.size(); i++) {
			tableFileDB << setw(tableFields[i].second) << data[i] << ",";
		}
		tableFileDB << "\n";
	}
	else {
		return false;
	}
	
	changeTableCurrentSizeInFile(tableCurrentSize);

	return true;
}


vector<string> DataBase::select(int line) {
	fstream tableFile(tableFileName, std::fstream::in);
	string tuple;

	if (tableFile.is_open()) {
		tableFile.seekg((line - 1) * tupleLength, 0);
		getline(tableFile, tuple);
	}

	vector<string> arrayFromTuple = tupleToArray(tuple, ',');
	//if (vs.size() < tableFields.size())
	if (arrayFromTuple.size() < 2) {
		vector<string> emptyArray;
		return emptyArray;
	}
	

	return arrayFromTuple;
}


//void DataBase::deleteRecord(int line)

vector< pair<int, vector<string> > > DataBase::selectWhere(string field, string value) {
	vector< pair<int, vector<string> > > lines;
	// ѕровер€ем, существует ли такое поле в таблице
	int k = -1;
	for (int i = 0; i < tableFields.size(); i++) {
		if (tableFields[i].first.compare(field) == 0)
			k = i;
	}
	if (k == -1) {
		cout << "“акого пол€ не существует." << endl;
	}
	else {
		// —мотрим, есть ли дл€ данного пол€ таблица индекса
		if (tableFieldsIndexExist[k] == true) {
			string indexFileName = tableName + "\\" + field + ".idx";
			fstream indexFile(indexFileName, std::fstream::in);

			// ќпредел€ем размер записи в таблице индексов
			string record;
			getline(indexFile, record);
			int recordSize = record.size() + 2;

			
			int fieldHash = calculateIndexHash(value, tableCapacity);
			indexFile.seekp((fieldHash - 1) * recordSize, 0);

			// ѕровер€ем наличие записи в индексном файле
			// ѕока не встретим пустую запись, пытаемс€ найти еще записи с этим значением
			while (true) {
				getline(indexFile, record);
				string fieldValue;
				int idx;
				stringstream ss;
				ss << record;
				ss >> fieldValue >> idx;
				if (fieldValue.compare(value) == 0) {
					//lines.push_back(idx);
					vector<string> tupleVector = select(idx);
					if (tupleVector.size() > 0)
						lines.push_back(make_pair(idx, tupleVector));
				}
				if (fieldValue.compare("") == 0) {
					break;
				}
			}
		}
		// ≈сли таблицы индексов нет, то ищем простым перебором
		else {
			string tuple;
			fstream tableFile(tableFileName, std::fstream::in);
			// ѕроходим по всем запис€м в таблице
			for (int i = 0; i < tableCurrentSize; i++) {
				vector<string> tupleVector = select(i + 1);
				if (tupleVector[k].compare(value) == 0) {
					lines.push_back(make_pair(i + 1, tupleVector));
				}
			}
		}
	}

	for (int i = 0; i < lines.size(); i++) {
		cout << lines[i].first << " ";
		for (int j = 0; j < lines[i].second.size(); j++) {
			cout << lines[i].second[j] << " ";
		}
		cout << endl;
	}

	return lines;
}


void DataBase::updateWhere(string field, string value, string newValue) {
	// ищем все записи по полю
	// с помощью select получаем вектор значений
	// мен€ем значение пол€ по номеру пол€
	// преобразуем в строку
	// вставл€ем в Ѕƒ на прежнее место

	vector< pair<int, vector<string> > > lines = selectWhere(field, value);

}


int DataBase::deleteWhere(string field, string value) {
	vector< pair<int, vector<string> > > lines = selectWhere(field, value);

	fstream tableFile(tableFileName, std::fstream::in | std::fstream::out);
	int k = 2;
	string tuple;

	// пуста€ запись, котора€ заменить удал€емую
	string emptyRecord;
	for (int i = 0; i < tupleLength - k; i++) {
		emptyRecord.append(" ");
	}
	emptyRecord.append("\n");


	if (tableFile.is_open()) {
		for (int i = 0; i < lines.size(); i++) {
			tableFile.seekg((lines[i].first - 1) * tupleLength, 0);
			tableFile << emptyRecord;
		}
	}

	return lines.size();
}


void DataBase::dropTable(string table) {

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


// јлгоритм хэшировани€ строк, которое используетс€ дл€ быстрого доступа к данным по ключевым пол€м.
// ѕолное описание алгоритма http://e-maxx.ru/algo/string_hashes
int DataBase::calculateIndexHash(string fieldName, int tableCapacity) {
	int p = 73; // простое число, примерно равное количеству символов во входном алфавите (прописные и строчные буквы и цифры)
	unsigned long long hash = 0, power = 1;
	for (int i = 0; i < fieldName.length(); i++) {
		hash += (fieldName[i] - 'a' + 1) * power;
	}
	return hash % tableCapacity;
}


void DataBase::changeTableCurrentSizeInFile(int curSize) {
	fstream tableConfigFile(tableConfigFileName, std::fstream::in | std::fstream::out);
	tableConfigFile.seekp(0);
	tableConfigFile << to_string(tableCurrentSize);
	tableConfigFile.close();
}


std::vector<std::string> DataBase::tupleToArray(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		while (token[0] == ' ')
			token.erase(0, 1);
		tokens.push_back(token);
	}
	return tokens;
}
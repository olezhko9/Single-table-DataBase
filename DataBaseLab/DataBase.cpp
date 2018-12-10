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
		// ���������� ���������� � �������� ������� � ������ ����
		while (cin >> fieldName >> fieldLength >> fieldIndex) {
			if (fieldLength == 0) break;
			else {
				tableConfigFile << fieldName << " " << fieldLength << " " << fieldIndex << endl;
				// ������� ������� �������� ��� ����
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
			// ��������� ����� � ������ ������ ������ � �������
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
		// ��������� �� ������������ �� ��������� ����
		if (selectWhere(tableFields[0].first, data[0]).size() != 0) {
			return false;
		}
		else {
			tableCurrentSize += 1;
			// ���������� ������ � ������ ����
			for (int i = 0; i < data.size(); i++) {
				// ���� �� ����� ���� ���� ������� ��������, �� ���������� � ��� ���������
				if (tableFieldsIndexExist[i] == true) {
					string indexFileName = tableName + "\\" + tableFields[i].first + ".idx";
					fstream indexFile(indexFileName, std::fstream::in | std::fstream::out);

					if (indexFile.is_open()) {
						// ���������� ������ ������ � ������� ��������
						string record;
						getline(indexFile, record);
						int recordSize = record.size() + 2;

						// ������, �� ����� �������� ������ ��������� ������ � �������
						int fieldHash = calculateIndexHash(data[i], tableCapacity);

						// ������ ������ � ������� �������� � ������ ������ �����
						indexFile.seekp((fieldHash - 1) * recordSize, 0);
						while (true) {
							getline(indexFile, record);
							string fieldValue;
							int idx;
							stringstream ss_in;
							ss_in << record;
							ss_in >> fieldValue >> idx;
							// ���� ������ � ������� ������, �� ����������
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

	// ��������� � ���� ������� � ��������� ������
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


vector< pair<int, vector<string> > > DataBase::selectWhere(string field, string value) {
	vector< pair<int, vector<string> > > lines;
	// ���������, ���������� �� ����� ���� � �������
	int k = -1;
	for (int i = 0; i < tableFields.size(); i++) {
		if (tableFields[i].first.compare(field) == 0)
			k = i;
	}
	if (k == -1) {
		cout << "������ ���� �� ����������" << endl;
		return lines;
	}
	else {
		// �������, ���� �� ��� ������� ���� ������� �������
		if (tableFieldsIndexExist[k] == true) {
			string indexFileName = tableName + "\\" + field + ".idx";
			fstream indexFile(indexFileName, std::fstream::in);

			// ���������� ������ ������ � ������� ��������
			string record;
			getline(indexFile, record);
			int recordSize = record.size() + 2;

			int fieldHash = calculateIndexHash(value, tableCapacity);
			indexFile.seekp((fieldHash - 1) * recordSize, 0);

			// ��������� ������� ������ � ��������� �����
			// ���� �� �������� ������ ������, �������� ����� ��� ������ � ���� ���������
			while (true) {
				getline(indexFile, record);
				string fieldValue;
				int idx;
				stringstream ss;
				ss << record;
				ss >> fieldValue >> idx;

				while (fieldValue[0] == ' ')
					fieldValue.erase(0, 1);

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
		// ���� ������� �������� ���, �� ���� ������� ���������
		else {
			string tuple;
			fstream tableFile(tableFileName, std::fstream::in);
			// �������� �� ���� ������� � �������
			for (int i = 0; i < tableCurrentSize; i++) {
				vector<string> tupleVector = select(i + 1);
				if (tupleVector[k].compare(value) == 0) {
					lines.push_back(make_pair(i + 1, tupleVector));
				}
			}
		}
	}

	/*for (int i = 0; i < lines.size(); i++) {
		cout << lines[i].first << " ";
		for (int j = 0; j < lines[i].second.size(); j++) {
			cout << lines[i].second[j] << " ";
		}
		cout << endl;
	}
	*/
	return lines;
}


int DataBase::updateWhere(string field, string value, string newValue) {
	// ��������� �� ������������ �� ��������� ����
	if (field.compare(tableFields[0].first) == 0) {
		// ���� ���� �������� � ������ �� ���������, �� �������
		if (selectWhere(tableFields[0].first, newValue).size() != 0) {
			return 0;
		}
	}

	vector< pair<int, vector<string> > > lines = selectWhere(field, value);
	// ���� ��� �������, ������� ����� ��������, �� �������
	if (lines.size() == 0) {
		cout << "������ ���� �� ����������." << endl;
		return 0;
	}
	else {
		int k = -1;
		for (int i = 0; i < tableFields.size(); i++) {
			if (tableFields[i].first.compare(field) == 0)
				k = i;
		}
		for (int i = 0; i < lines.size(); i++) {
			// ������� ������ ������ ������
			deleteRecord(lines[i].first);
			
			// �������� �������� ����
			lines[i].second[k] = newValue;

			// ��������� ����� ������
			insert(lines[i].second);
		}		
	}
	return lines.size();
}


void DataBase::deleteRecord(int line) {
	int k = 2; // ���������� �����
	// ������ ������, ������� ������� ���������
	string emptyRecord;
	for (int i = 0; i < tupleLength - k; i++) {
		emptyRecord.append(" ");
	}
	emptyRecord.append("\n");

	fstream tableFile(tableFileName, std::fstream::in | std::fstream::out);
	tableFile.seekg((line - 1) * tupleLength, 0);
	tableFile << emptyRecord;
}


int DataBase::deleteWhere(string field, string value) {
	vector< pair<int, vector<string> > > lines = selectWhere(field, value);

	// ������� ������ �� �� �������
	for (int i = 0; i < lines.size(); i++) {
		deleteRecord(lines[i].first);
	}

	return lines.size();
}


void DataBase::dropTable(string table) {
	// ����� ������ ���� ��� �������� ����� �� �� �����...
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


// ��� ������� � ����� http://qaru.site/questions/323303/hash-function-for-a-string
int DataBase::calculateIndexHash(string fieldName, int tableCapacity) {
	int seed = 131;
	unsigned long hash = 0;
	for (int i = 0; i < fieldName.length(); i++){
		hash = (hash * seed) + fieldName[i];
	}
	return hash % tableCapacity;
	return hash;
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
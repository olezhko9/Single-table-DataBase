#include <clocale>
#include "DataBase.h"
#include "BankCard.h"
using namespace std;


int main() {

	setlocale(LC_ALL, "Russian");
	int consoleCommand = -1;
	DataBase DB;
	BankCard card1("4000300022221000", "OlegN", "26.11.2018", 10500.5, 159);
	BankCard card2("1111222230004000", "DianaN", "10.12.2018", 112345.7, 234);
	BankCard card3("1000222230004444", "IlyaM", "16.08.2019", 345.48, 030);
	BankCard card4("1111222233334444", "SvetlanaB", "14.11.2020", 9999.99, 999);

	while (true) {
		cout << "\n\n(0) ����� �� ���������.\n"
			<< "(1) ������� �������\n"
			<< "(2) ������� �������\n"
			<< "����� ��������� ������� ����, ������� ����� ������� �������\n"
			<< "(3) ������� ������ �� �������\n"
			<< "(4) �������� ������ � �������\n"
			<< "(5) ����� �� �������� ���������� ����\n"
			<< "(6) ������������� ������ � �������\n"
			<< "(7) ������� ������ � �������\n"
			<< "(8) ������� �������\n"
			<< "��� �� ������ �������?\n";
		cin >> consoleCommand;

		// ����� �� ���������
		if (consoleCommand == 0) {
			cout << "����� �� ���������." << endl;
			break;
		}

		// ������� �������
		else if (consoleCommand == 1) {
			string tableName;
			cout << "������� ��� �������: ";
			cin >> tableName;
			cout << "������� ��� ����, ��� ����� � ������� true, ���� ��������� ������� �������� ������ ��� ����� ����, ����� false.\n"
				<< "������ ���� - ��������.\n"
				<< "��� ������ ������� ������� �����." << endl;
			if (DB.createTable(tableName)) cout << "������� �������" << endl;
			else cout << "������� �� �������" << endl;
		}

		// ������� �������
		else if (consoleCommand == 2) {
			string tableName;
			cout << "������� ��� �������: ";
			cin >> tableName;
			if (DB.openTable(tableName)) cout << "������� " << tableName << " ������ � ������" << endl;
			else cout << "�� ������� ������� ������� " << tableName << endl;
		}

		// ������� ������ �� �������
		else if (consoleCommand == 3) {
			int selectCount = DB.selectAll();
			cout << "����� �������� " << selectCount << " �������." << endl;
		}

		// �������� ������ � �������
		else if (consoleCommand == 4) {
			BankCard bc;
			if (DB.insert(bc.toArray())) {
				cout << "������ ���������" << endl;
			}
			else {
				cout << "������ ��� ����������" << endl;
			}
			DB.insert(card1.toArray());
			DB.insert(card2.toArray());
			DB.insert(card3.toArray());
			DB.insert(card4.toArray());
		}

		// ����� � ��������
		else if (consoleCommand == 5) {
			string field, value;
			cout << "������� ��� ����, �� �������� ������: ";
			cin >> field;
			cout << "������� �������� ����, ������� ������: ";
			cin >> value;			
			vector< pair<int, vector<string> > > selected;

			//selected = DB.selectWhere("number", "1000200030004000");
			selected = DB.selectWhere(field, value);
			for (int i = 0; i < selected.size(); i++) {
				cout << selected[i].first << " ";
				for (int j = 0; j < selected[i].second.size(); j++) {
					cout << selected[i].second[j] << " ";
				}
				cout << endl;
			}
		}

		// �������� ������
		else if (consoleCommand == 6) {
			string field, value, newValue;
			cout << "������� ��� ����, ������� ����� ��������: ";
			cin >> field;
			cout << "������� ������ ��������: ";
			cin >> value;
			cout << "������� ����� ��������: ";
			cin >> newValue;
			cout << "���� ��������� " << DB.updateWhere(field, value, newValue) << " �������.\n";
			//cout << "���� ��������� " << DB.updateWhere("number", "1111222230004000", "1000200030004000") << " �������.\n";
		}

		// �������� ������ �� �������� ����
		else if (consoleCommand == 7) {
			string field, value;
			cout << "������� ��� ����, �� �������� ����� �������: ";
			cin >> field;
			cout << "������� �������� ����: ";
			cin >> value;
			cout << "���� ������� " << DB.deleteWhere(field, value) << " �������.\n";
			//cout << "���� ������� " << DB.deleteWhere("number", "4000300022221000") << " �������.\n";
		} 

		// ������� �������
		else if (consoleCommand == 8) {
			string tableName;
			cout << "������� ��� �������, ������� ����� �������.\n";
			cin >> tableName;
			DB.dropTable(tableName);
		}

		else {
			cout << "�������� �������\n";
		}
	}
	
	system("pause");
	return 0;
}
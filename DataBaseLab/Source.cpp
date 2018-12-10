#include <clocale>
#include "DataBase.h"
#include "BankCard.h"
using namespace std;


int main() {

	setlocale(LC_ALL, "Russian");
	int consoleCommand = -1;
	DataBase DB;
	BankCard card1("4000300020001000", "Oleg Naumov", "26.11.2018", 10500.5f, 159);
	BankCard card2("1111222230004000", "Oleg Nenaumov", "26.11.2018", 12345.7f, 222);
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
			cout << "������� ��� ����, ��� ����� � ������� true, ���� ��������� ������ ��� ����� ����, ����� false.\n"
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
			//DB.openTable("card");
			int selectCount = DB.selectAll();
			cout << "����� �������� " << selectCount << " �������." << endl;
		}
		// �������� ������ � �������
		else if (consoleCommand == 4) {
			DB.openTable("card");
			if (DB.insert(card1.toArray())) {
				cout << "������ ���������" << endl;
			}
			else {
				cout << "������ ��� ����������" << endl;
			}
		}
		// ����� � ��������
		else if (consoleCommand == 5) {
			DB.openTable("card");
			DB.selectWhere("number", "1000200030004100");
			//DB.selectWhere("balance", "10500.5");
		}
		// �������� ������ �� �������� ����
		else if (consoleCommand == 7) {
			DB.openTable("card");
			cout << "���� ������� " << DB.deleteWhere("number", "1000200030004000") << " �������.\n";
			DB.selectWhere("number", "1000200030004000");
		} 
		// ������� �������
		else if (consoleCommand == 8) {
			DB.dropTable("card");
		}
		else {
			cout << "�������� �������\n";
		}
	}
	
	system("pause");
	return 0;
}
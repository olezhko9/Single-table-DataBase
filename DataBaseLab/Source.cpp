#include <clocale>
#include "DataBase.h"
#include "BankCard.h"
using namespace std;


int main() {

	setlocale(LC_ALL, "Russian");
	int consoleCommand = -1;
	DataBase DB;
	BankCard card1("4000300020001000", "Oleg Naumov", "26.11.2018", 10500.5, 159);
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
			DB.insert(card1.toArray());
		}
	}
	
	/*db.openTable("card");

	//BankCard bc("4000300020001000", "Oleg Naumov", "26.11.2018", 10500.5, 159);
	//BankCard bc("1000200030004000", "Oleg Nenaumov", "3.3.2020", 1000.0, 753);
	//cout << bc;

	//db.insert(bc.toArray());
	cout << db.select(1) << endl;
	cout << db.select(8) << endl;
	//db.select(1);*/
	system("pause");
	return 0;
}
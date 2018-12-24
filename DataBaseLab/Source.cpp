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
		cout << "\n\n(0) Выйти из программы.\n"
			<< "(1) Создать таблицу\n"
			<< "(2) Открыть таблицу\n"
			<< "Чтобы выполнить команды ниже, сначала нужно открыть таблицу\n"
			<< "(3) Вывести данные из таблицы\n"
			<< "(4) Вставить данные в таблицу\n"
			<< "(5) Поиск по значению некоторого поля\n"
			<< "(6) Редактировать запись в таблице\n"
			<< "(7) Удалить запись в таблице\n"
			<< "(8) Удалить таблицу\n"
			<< "Что Вы хотите сделать?\n";
		cin >> consoleCommand;

		// Выйти из программы
		if (consoleCommand == 0) {
			cout << "Выход из программы." << endl;
			break;
		}

		// Создать таблицу
		else if (consoleCommand == 1) {
			string tableName;
			cout << "Введите имя таблицы: ";
			cin >> tableName;
			cout << "Введите имя поля, его длину и укажите true, если создавать таблицу быстрого поиска для этого поля, иначе false.\n"
				<< "Первое поле - ключевое.\n"
				<< "Для выхода введите нулевую длину." << endl;
			if (DB.createTable(tableName)) cout << "Таблица создана" << endl;
			else cout << "Таблица не создана" << endl;
		}

		// Открыть таблицу
		else if (consoleCommand == 2) {
			string tableName;
			cout << "Введите имя таблицы: ";
			cin >> tableName;
			if (DB.openTable(tableName)) cout << "Таблица " << tableName << " готова к работе" << endl;
			else cout << "Не удалось открыть таблицу " << tableName << endl;
		}

		// Вывести данные из таблицы
		else if (consoleCommand == 3) {
			int selectCount = DB.selectAll();
			cout << "Всего показано " << selectCount << " записей." << endl;
		}

		// Вставить данные в таблицу
		else if (consoleCommand == 4) {
			BankCard bc;
			if (DB.insert(bc.toArray())) {
				cout << "Запись добавлена" << endl;
			}
			else {
				cout << "Ошибка при добавлении" << endl;
			}
			DB.insert(card1.toArray());
			DB.insert(card2.toArray());
			DB.insert(card3.toArray());
			DB.insert(card4.toArray());
		}

		// Поиск с условием
		else if (consoleCommand == 5) {
			string field, value;
			cout << "Введите имя поля, по которому искать: ";
			cin >> field;
			cout << "Введите значение поля, которое искать: ";
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

		// Обновить запись
		else if (consoleCommand == 6) {
			string field, value, newValue;
			cout << "Введите имя поля, которое нужно обновить: ";
			cin >> field;
			cout << "Введите старое значение: ";
			cin >> value;
			cout << "Введите новое значение: ";
			cin >> newValue;
			cout << "Было обновлено " << DB.updateWhere(field, value, newValue) << " записей.\n";
			//cout << "Было обновлено " << DB.updateWhere("number", "1111222230004000", "1000200030004000") << " записей.\n";
		}

		// Удаление запись по значению поля
		else if (consoleCommand == 7) {
			string field, value;
			cout << "Введите имя поля, по которому нужно удалить: ";
			cin >> field;
			cout << "Введите значение поля: ";
			cin >> value;
			cout << "Было удалено " << DB.deleteWhere(field, value) << " записей.\n";
			//cout << "Было удалено " << DB.deleteWhere("number", "4000300022221000") << " записей.\n";
		} 

		// Удалить таблицу
		else if (consoleCommand == 8) {
			string tableName;
			cout << "Введите имя таблицы, которую нужно удалить.\n";
			cin >> tableName;
			DB.dropTable(tableName);
		}

		else {
			cout << "Неверная команда\n";
		}
	}
	
	system("pause");
	return 0;
}
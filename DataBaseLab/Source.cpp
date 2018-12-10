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
			cout << "Введите имя поля, его длину и укажите true, если создавать индекс для этого поля, иначе false.\n"
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
			//DB.openTable("card");
			int selectCount = DB.selectAll();
			cout << "Всего показано " << selectCount << " записей." << endl;
		}
		// Вставить данные в таблицу
		else if (consoleCommand == 4) {
			DB.openTable("card");
			if (DB.insert(card1.toArray())) {
				cout << "Запись добавлена" << endl;
			}
			else {
				cout << "Ошибка при добавлении" << endl;
			}
		}
		// Поиск с условием
		else if (consoleCommand == 5) {
			DB.openTable("card");
			DB.selectWhere("number", "1000200030004100");
			//DB.selectWhere("balance", "10500.5");
		}
		// Удаление запись по значению поля
		else if (consoleCommand == 7) {
			DB.openTable("card");
			cout << "Было удалено " << DB.deleteWhere("number", "1000200030004000") << " записей.\n";
			DB.selectWhere("number", "1000200030004000");
		} 
		// Удалить таблицу
		else if (consoleCommand == 8) {
			DB.dropTable("card");
		}
		else {
			cout << "Неверная команда\n";
		}
	}
	
	system("pause");
	return 0;
}
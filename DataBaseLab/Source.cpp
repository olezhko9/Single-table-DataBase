#include "DataBase.h"
#include "BankCard.h"
using namespace std;


int main() {
	DataBase db;
	db.openTable("card");

	//BankCard bc("4000300020001000", "Oleg Naumov", "26.11.2018", 10500.5, 159);
	//BankCard bc("1000200030004000", "Oleg Nenaumov", "3.3.2020", 1000.0, 753);
	//cout << bc;

	//db.insert(bc.toArray());
	cout << db.select(1) << endl;
	cout << db.select(8) << endl;
	//db.select(1);
	system("pause");
	return 0;
}
#include "BankCard.h"

BankCard::BankCard(string card_number, string card_holder, string card_end_date, float card_balance, int card_cvc) {
	number = card_number;
	holder_name = card_holder;
	end_date = card_end_date;
	balance = card_balance;
	cvc = card_cvc;
}

BankCard::BankCard() {
	cout << "¬ведите номер карты: ";
	cin >> number;
	cout << "¬ведите им€ владельца карты: ";
	cin >> holder_name;
	cout << "¬ведите срок действи€ карты: ";
	cin >> end_date;
	cout << "¬ведите баланс карты: ";
	cin >> balance;
	cout << "¬ведите CVC код: ";
	cin >> cvc;
}
//5555666677778888 OlegNenaumov 11.12.2018 100500.15 302
vector<string> BankCard::toArray() {
	vector<string> data;
	data.push_back(number);
	data.push_back(holder_name);
	data.push_back(end_date);

	stringstream ss;
	ss << balance;
	string balanceString;
	ss >> balanceString;
	std::replace(balanceString.begin(), balanceString.end(), ',', '.');
	data.push_back(balanceString);

	data.push_back(to_string(cvc));

	return data;
}




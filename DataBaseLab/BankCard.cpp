#include "BankCard.h"

BankCard::BankCard(string card_number, string card_holder, string card_end_date, float card_balance, int card_cvc) {
	number = card_number;
	holder_name = card_holder;
	end_date = card_end_date;
	balance = card_balance;
	cvc = card_cvc;
}

vector<string> BankCard::toArray() {
	vector<string> data;
	data.push_back(number);
	//data.push_back(holder_name);
	//data.push_back(end_date);
	stringstream ss;
	ss << balance;
	string balanceString;
	ss >> balanceString;
	std::replace(balanceString.begin(), balanceString.end(), ',', '.');
	data.push_back(balanceString);
	//cout << balanceString << endl;
	//data.push_back(to_string(cvc));

	return data;
}


ostream& operator<<(ostream& os, const BankCard& bc) {
	os << bc.number << " " << bc.holder_name << " " << bc.end_date << " " << bc.balance << " " << bc.cvc << "\n";
	return os;
}
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

class BankCard {
private:
	string number;
	string holder_name;
	string end_date;
	float balance;
	int cvc;
public:
	BankCard(string card_number, string card_holder, string card_end_date, float card_balance, int card_cvc);

	vector<string> toArray();
	friend ostream& operator<<(ostream& os, const BankCard& bc);
};
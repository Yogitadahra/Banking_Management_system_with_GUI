#include <iostream>
#include <cstring>
#include <fstream>
#include "Account.h"
#include "AccountHolder.h"

using namespace std;

void Account::withdraw(double amount)
{
    if (balance >= amount)
    {
        balance -= amount;
        cout << "You have successfully withdrawn " << amount << " from your account and your new account balance is: " << balance << endl;
    }
    else
    {
        cout << "Insufficient Balance!\nPlease Enter Valid Amount" << endl;
    }
}

Account::Account(Account* a){
    balance=a->balance;
    strcpy(acc_number, a->acc_number);
    strcpy(password, a->password);
    card=a->card;
    cardCount=a->cardCount;
}

void Account::deposit(double amount)
{
    if (amount >= 0)
    {
        balance += amount;
    }
    else
    {
        // invalid amount
    }
}

char *Account::getAccNumber()
{
    return acc_number;
}

Transaction::Transaction() {}

Transaction::Transaction(char *acc, char *s, char *t, double a, char *d)
{
    strncpy(acc_no, acc, sizeof(acc_no));
    strncpy(source, s, sizeof(source));
    strncpy(type, t, sizeof(type));
    amount = a;
    strncpy(date, d, sizeof(date));
}

void Transaction::write()
{
    ofstream file("Transaction.dat", ios::binary | ios::app);
    if (!file.is_open())
    {
        cout << "Unable to open!" << endl;
        return;
    }
    file.write(reinterpret_cast<char *>(this), sizeof(Transaction));
    file.close();
}

void Transaction::read(char *acc, char *source)
{
    ifstream file("Transaction.dat", ios::binary | ios::in);

    if (!file.is_open())
    {
        cout << "Unable to open!" << endl;
        return;
    }

    int n = 0;
    Transaction temp;

    while (file.read(reinterpret_cast<char *>(&temp), sizeof(Transaction)))
    {

        if (!strcmp(temp.acc_no, acc) && !strcmp(temp.source, source))
        {
            cout << "Transaction Number: " << ++n << endl;
            cout << temp << endl;
        }
    }
    file.close();
}

ostream &operator<<(ostream &out, Transaction &t)
{
    return (out << "Source: " << t.source << "\tType: " << t.type << "\tAmount: " << t.amount << "\tDate: " << t.date);
}

Transaction::~Transaction() {}

void Account::generateAccNumber()
{
    ifstream file("AccountHolder.dat", ios::in | ios::binary);

    if (!file.is_open())
    {
        strcpy(acc_number, "1382409012");
        return;
    }

    file.seekg(0, ios::end);
    if (file.tellg() == 0)
    {
        strcpy(acc_number, "1382409012");
    }
    else
    {
        file.seekg(-static_cast<int>(sizeof(AccountHolder)), ios::end);
        AccountHolder temp;
        file.read(reinterpret_cast<char*>(&temp), sizeof(AccountHolder));

        if (temp.account != nullptr)
        {
            long long number = atoll(temp.account->acc_number);
            number += 100;
            std::string newAcc = std::to_string(number);
            strncpy(acc_number, newAcc.c_str(), sizeof(acc_number));
        }
        else
        {
            strcpy(acc_number, "1382409012");
        }
    }

    file.close();
}


Account::Account(char *pass){
    generateAccNumber();
    balance=0;
    strcpy(password, pass);
    cardCount=0;
    card=nullptr;
}
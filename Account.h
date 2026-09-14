#pragma once

#include <iostream>
#include <fstream>
#include "Card.h"
#include "AccountHolder.h"

using namespace std;

class Transaction{
    private:
        char acc_no[25];
        char source[25];
        char type[25];
        double amount;
        char date[25];
    
    public:

    Transaction();
    Transaction(char* acc_no, char* s, char* t, double a, char* d);
    void write();
    void read(char*, char*);
    friend ostream& operator<<(ostream& out, Transaction &t);

    ~Transaction();
};

class Account{
private:
    double balance;
    char acc_number[20];
    char password[20];
    Card *card;
    int cardCount;

    friend class AccountHolder;

public:
    Account(char *pass);
    Account(Account* a);
    void generateAccNumber();
    void withdraw(double amount);
    void transfer(Account &a, double amount);
    void deposit(double amount);

    char* getAccNumber();
};

class Current: public Account{
private:

public:
};

class Saving: public Account{
private:
    double interestRate;


public:
};
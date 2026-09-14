#pragma once

#include <iostream>

using namespace std;

class Account;

class Card{
protected:
    char pass[20];
    char card_number[20];
    
public:
    virtual void operator=(Card *c)=0;
};

class Debit_Card: public Card{
private:

public:
    void withdraw(Account &a, double amount);
    void operator=(Card *c);
};

class Credit_Card: public Card{
private:
    double borrowedMoney;
    double limit;
    char deadline[20];
    double interestRate;

public:
    void withdraw(double amount);
    void returnBorrowedAmount(double amount);
    void operator=(Card *c);
};
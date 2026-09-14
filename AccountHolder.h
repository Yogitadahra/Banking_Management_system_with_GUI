#pragma once

#include <iostream>
#include "Account.h"
#include "User.h"
using namespace std;

class AccountHolder : public User
{
private:
    Account *account;
    int accountCount;
    int age;
    double salary;
    char address[50];
    char creationDate[20];
    char contact[20];
    char accountType[20];
    bool loanApproved = false;
    bool loanRequested = false;
    float loanAmount = 0.0f;

    friend class Account;

public:
    AccountHolder();
    AccountHolder(char data[][20], int size, char *type);
    AccountHolder(User *u);

    void displayMenuScreen();
    void drawAccountInfoScreen();
    void operator=(User *u);
    char *getAccountNumber();
    bool hasPendingLoan();
    void approveLoan();
};
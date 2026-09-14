#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <ctime>
#include <chrono>
#include "Account.h"
#include "Card.h"
#include "User.h"
#include "AccountHolder.h"

using namespace std;

char* getCurrentDate();
int calculateAge(string, string);
chrono::system_clock::time_point stringToDate(string& dateStr);

class Bank
{
private:
    User *user;
    const int encryption_key=120;
    char adminName[20]="Shivam";
    char adminPassword[20]="shivam123";


public:
    void encrypt_decrypt(char* s);
void Start();

    bool log_in(char* name, char* pass);

    void createAccount(char data[][20], int size, char *type);

    void drawInputWindow();

    void drawCreateAccScreen();
};

#endif
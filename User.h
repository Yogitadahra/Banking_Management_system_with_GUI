#pragma once

#include <iostream>

using namespace std;



class User{
protected:
    char username[25];
    char password[25];

public:
    User(char* u, char* pass);
    User();
    
    virtual void displayMenuScreen()=0;

    bool verifyPass(char* pass);

    char* getUsername();

    char* getPass();


    bool checkForAdmin(char *name, char *pass);

    virtual ~User();
};
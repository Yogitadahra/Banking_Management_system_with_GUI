#include <iostream>
#include <cstring>
#include "User.h"
#include "Bank.h"

using namespace std;

User::User(char* u, char* pass){
    strcpy(username, u);
    strcpy(password, pass);
}

User::User(){}

User::~User(){}


bool User::verifyPass(char* pass)
{
    return (!strcmp(password, pass)) ? true : false;
}

char* User::getUsername()
{
    return username;
}

char* User::getPass()
{
    return password;
}

bool User::checkForAdmin(char *name, char *pass){
    char username[20]="Shivam";
    char password[20]="shivam123";

    return(!strcmp(name, username) && !strcmp(password, pass))? true: false;
}
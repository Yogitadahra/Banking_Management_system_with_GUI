#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <fstream>
#include <string>
#include "AccountHolder.h"
#include "User.h"

class Admin : public User {
public:
    Admin(char* uname,char* pass);
    Admin(User *u);

    void viewAllAccounts();
    void deleteAccount(char*);
    void approveLoan(char*);
    void displayMenuScreen();
};


#endif

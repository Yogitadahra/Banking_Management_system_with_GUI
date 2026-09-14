#include <iostream>
#include "Admin.h"
#include <cstring>
#include <fstream>

using namespace std;

void Admin::displayMenuScreen()
{
    int n;
    cout << "Choose Required Option: \n";
    cout << "1. View All Accounts\n";
    cout<<"2. Delete Account\n";
    cout<<"3. Approve Loan\n";
    cout<<"4. Exit\n";
    cout<<"Enter: ";

    cin>>n;
    switch (n)
    {
    case 1:
    {
        viewAllAccounts();
        break;
    }
    case 2:
    {
        char acc[25];
        cout<<"Enter Accout Number to delete: \n";
        cin>>acc;
        deleteAccount(acc);
        break;
    }
    case 3:
    {
        char acc[25];
        cout<<"Enter Accout Number of Loan Applicant: \n";
        cin>>acc;
        approveLoan(acc);
        break;
    }
    default:{
        return;
    }
    }
}

Admin::Admin(char* uname, char *pass)
{
    strcpy(username, uname);
    strcpy(password, pass);
}

Admin::Admin(User *u)
{
    Admin *a = dynamic_cast<Admin *>(u);
    strcpy(username, a->username);
    strcpy(password, a->password);
}

void Admin::viewAllAccounts()
{
    ifstream file("AccountHolder.dat", ios::binary);
    if (file.is_open())
    {
        return;
    }

    AccountHolder temp;
    while (file.read((char *)&temp, sizeof(AccountHolder)))
    {
        // display
    }
    file.close();
}
void Admin::deleteAccount(char *accNo)
{
    ifstream in("AccountHolder.dat", ios::binary);
    ofstream out("temp.dat", ios::binary);
    AccountHolder temp;
    bool found = false;

    while (in.read(reinterpret_cast<char *>(&temp), sizeof(AccountHolder)))
    {
        if (!strcmp(accNo, temp.getAccountNumber()))
            out.write(reinterpret_cast<char *>(&temp), sizeof(AccountHolder));
        else
            found = true;
    }

    in.close();
    out.close();
    remove("AccountHolder.dat");
    rename("temp.dat", "AccountHolder.dat");

    if (found)
        cout << "Account deleted.\n";
    else
        cout << "Account not found.\n";
}

void Admin::approveLoan(char* accNo) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    AccountHolder temp;
    bool found = false;

    while (file.read((char*)&temp, sizeof(AccountHolder))) {
        if (temp.getAccountNumber() == accNo && temp.hasPendingLoan()) {
            temp.approveLoan(); // Assume you made this
            file.seekp((int)file.tellg() - sizeof(AccountHolder));
            file.write((char*)&temp, sizeof(AccountHolder));
            found = true;
            break;
        }
    }

    file.close();
    cout << (found ? "Loan approved.\n" : "Loan not found or account not found.\n");
}
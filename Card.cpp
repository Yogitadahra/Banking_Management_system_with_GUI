#include <iostream>
#include "Account.h"
#include "Bank.h"

using namespace std;

void Credit_Card:: withdraw(double amount){
    if(amount+borrowedMoney > limit){
        cout<<"You have already reached the limit. Please clear your dues"<<endl;
        return;
    }
    else if(amount<0){
        cout<<"Invalid Amount"<<endl;
        return;
    }
    else{
        borrowedMoney+=amount;
        cout<<"Payment Successfull"<<endl;
        return;
    }
}

void Debit_Card::withdraw(Account &a, double amount){
    a.withdraw(amount);
}

void Credit_Card::returnBorrowedAmount(double amount){
    char* currentDate;
    strcpy(currentDate, getCurrentDate());
    int days= calculateAge(currentDate, deadline);
    strcpy(deadline, currentDate);

    int interest=0;

    cout<<"You have borrowed: "<<borrowedMoney<<endl;
    if(days>0){
        interest = (borrowedMoney) * (interestRate/100) * (days/30);
        borrowedMoney+=interest;
        cout<<"You are returning amount after due date:("<<endl;
        cout<<"The deadline was: "<<deadline<<" and you are "<<(double)(days/30)<<" months late so you will be charges "<<interestRate/100<<" per month and "<<interestRate/100<<" per day!"<<endl;
        cout<<"Your net borrowed amount is: "<<borrowedMoney<<" after adding interest of "<<interest<<endl;
    }
    
    if(amount>0){
        borrowedMoney-=amount;
        cout<<amount<<" has been added to your creadit card and your updated dues are: "<<borrowedMoney<<endl;
    }
    else{
        cout<<"invalid amount"<<endl;
    }

}

void Debit_Card::operator=(Card *c){
    Debit_Card* d=dynamic_cast<Debit_Card*>(c);
    strcpy(pass, d->pass);
    strcpy(card_number, d->card_number);
}

void Credit_Card::operator=(Card *c){
    Credit_Card* card=dynamic_cast<Credit_Card*>(c);
    strcpy(pass, card->pass);
    strcpy(card_number, card->card_number);
    borrowedMoney=card->borrowedMoney;
    limit=card->limit;
    strcpy(deadline, card->deadline);
    interestRate=card->limit;
}
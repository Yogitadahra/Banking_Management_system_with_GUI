#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "C:\raylib\raylib\src\raylib.h"
#include "Bank.h"
#include "Account.h"
#include "AccountHolder.h"
#include "Admin.h"
#include "User.h"
#include "Card.h"

using namespace std;

AccountHolder::AccountHolder() {}

char* AccountHolder::getAccountNumber(){
    return account->acc_number;
}

AccountHolder::AccountHolder(char data[][20], int size, char *type)
{
    strcpy(username, data[1]);
    strcpy(contact, data[2]);
    strcpy(address, data[3]);
    age = atoi(data[4]);
    salary = atof(data[5]);
    strcpy(password, data[7]);
    strcpy(creationDate, getCurrentDate());
    strcpy(accountType, type);
    accountCount = 1;
    account = new Account(data[7]);
    
}

void AccountHolder::displayMenuScreen()
{
    Texture2D background = LoadTexture("BackGround.png");
    Color title={50, 50, 50, 255};
    float btnWidth = 300;
    float btnHeight = 40;
    float spacing = 15;
    float startY = 120;
    float centerX = 800 / 2 - btnWidth / 2;

    Rectangle buttons[] = {
        {centerX, startY + 0 * (btnHeight + spacing), btnWidth, btnHeight},
        {centerX, startY + 1 * (btnHeight + spacing), btnWidth, btnHeight},
        {centerX, startY + 2 * (btnHeight + spacing), btnWidth, btnHeight},
        {centerX, startY + 3 * (btnHeight + spacing), btnWidth, btnHeight},
        {centerX, startY + 4 * (btnHeight + spacing), btnWidth, btnHeight},
        {centerX, startY + 5 * (btnHeight + spacing), btnWidth, btnHeight},
        {centerX, startY + 6 * (btnHeight + spacing), btnWidth, btnHeight},
        {centerX, startY + 7 * (btnHeight + spacing), btnWidth, btnHeight},
    };

    int index = -1;

    const char *labels[] = {
        "Display Account Information", "Deposit", "Withdraw",
        "Create Other Account", "Register For Card", "Apply For Loan",
        "Check Balance", "Back"};

    while ([&]()
           { if(WindowShouldClose()) exit(0); return true; }())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);

        DrawText("Account Menu", GetScreenWidth() / 2 - MeasureText("Account Menu", 50) / 2, 35, 50, title);
        if(index<0){
            for (int i = 0; i < 8; i++)
            {
                DrawRectangleRec(buttons[i], WHITE);
                DrawRectangleLinesEx(buttons[i], 2, BLACK);
                DrawText(labels[i], buttons[i].x + 8, buttons[i].y + 15, 20, DARKBLUE);
            }
        }
        else{
            break;
        }

        for (int i = 0; i < 8; i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), buttons[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                index = i + 1;
            }
        }
        EndDrawing();
    }

    switch (index)
    {
    case 1:
    {
        drawAccountInfoScreen();
        break;
    }
    case 2:
    {
        // drawDepositScreen();
        break;
    }
    case 3:
    {
        // drawWithdrawScreen();
        break;
    }
    case 4:
    {
        // drawCreateAccount();
        break;
    }
    case 5:
    {
        // drawRgisterForCardScreen();
        break;
    }
    case 6:
    {
        // drawApplyForLoanScreen();
        break;
    }
    case 7:
    {
        // drawShowBalanceScreen();
    }
    default:
    {
        return;
    }
    }
    UnloadTexture(background);
    ClearBackground(SKYBLUE);
}

void AccountHolder::drawAccountInfoScreen()
{
     Texture2D background = LoadTexture("BackGround.png");
    Color title={50, 50, 50, 255};

    while ([&]()
           { if(WindowShouldClose()) exit(0); return true; }())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);

        DrawText("Bank Account Details", 260, 30, 30, title);
        int x = 100, y = 100, lineSpacing = 40;
        DrawText(TextFormat("Username:      %s", username), x, y + 0 * lineSpacing, 20, BLACK);
        DrawText(TextFormat("Age:           %d", age), x, y + 1 * lineSpacing, 20, BLACK);
        DrawText(TextFormat("Salary:        %.2f", salary), x, y + 2 * lineSpacing, 20, BLACK);
        DrawText(TextFormat("Address:       %s", address), x, y + 3 * lineSpacing, 20, BLACK);
        DrawText(TextFormat("Created On:    %s", creationDate), x, y + 4 * lineSpacing, 20, BLACK);
        DrawText(TextFormat("Contact No.:   %s", contact), x, y + 5 * lineSpacing, 20, BLACK);
        DrawText(TextFormat("Account Type:  %s", accountType), x, y + 6 * lineSpacing, 20, BLACK);

        EndDrawing();
    }
    UnloadTexture(background);
    ClearBackground(RAYWHITE);
}

void AccountHolder::operator=(User *u)
{
    AccountHolder *other = dynamic_cast<AccountHolder *>(u);

    strcpy(username, other->username);
    strcpy(contact, other->contact);
    strcpy(address, other->address);
    age = other->age;
    salary = other->salary;
    strcpy(password, other->password);
    strcpy(creationDate, other->creationDate);
    strcpy(accountType, other->accountType);
    account = new Account(other->account);
}

AccountHolder::AccountHolder(User *u)
{
    AccountHolder *other = dynamic_cast<AccountHolder *>(u);

    strcpy(username, other->username);
    strcpy(contact, other->contact);
    strcpy(address, other->address);
    age = other->age;
    salary = other->salary;
    strcpy(password, other->password);
    strcpy(creationDate, other->creationDate);
    strcpy(accountType, other->accountType);
    account = new Account(other->account);
}

bool AccountHolder::hasPendingLoan(){
    return loanRequested && !loanApproved;
}

void AccountHolder::approveLoan() {
    char currentDate[10];
    strcpy(currentDate, getCurrentDate());
    int ageInDays = calculateAge(currentDate, creationDate);

    if (!loanRequested) {
        cout << "No loan requested by this user.\n";
        return;
    }

    if (loanApproved) {
        cout << "Loan already approved.\n";
        return;
    }

    if (ageInDays >= 180) {
        loanApproved = true;
        cout << "Loan approved for amount: ₹" << loanAmount << '\n';
    } else {
        cout << "Account must be at least 180 days old to approve a loan.\n";
    }
}

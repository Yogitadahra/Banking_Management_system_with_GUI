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

void Bank::encrypt_decrypt(char *s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        s[i] ^= encryption_key;
    }
}

void Bank::Start()
{
    drawInputWindow();
    user->displayMenuScreen();
}

bool Bank::log_in(char *name, char *pass)
{
    char password[20];
    char username[20];
    bool readSucces = false;
    strcpy(username, name);
    strcpy(password, pass);
    ifstream file;
    file.open("UserInfo.txt", ios::in);

    if (file.is_open() == false)
    {
        return false;
    }
    else
    {

        if (!strcmp(username, adminName) && !strcmp(password, adminPassword))
        {
            user = new Admin(username, password);
            // admin info
            readSucces=true;
        }
        else
        {
            encrypt_decrypt(password);
            string name, pass;
            while ((file>>name>>pass) && !readSucces)
            {
                if (!strcmp(username, name.c_str()) && !strcmp(password, pass.c_str()))
                {
                    ifstream file1("AccountHolder.dat", ios::binary);

                    if (file1.is_open() == false)
                    {
                        return false;
                    }

                    AccountHolder temp;
                    while (file1.read(reinterpret_cast<char *>(&temp), sizeof(AccountHolder)))
                    {
                        if (!strcmp(temp.getUsername(), username) && temp.verifyPass(password))
                        {
                            user = new AccountHolder(temp);
                            readSucces = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    file.close();
    if(readSucces) return true;
    else return false;
}

void Bank::createAccount(char data[][20], int size, char *type)
{
    encrypt_decrypt(data[7]);
    
    ofstream file("UserInfo.txt", ios::app);
    file << data[1] << " " << data[7] << endl;
    file.close();

    user=new AccountHolder(data, size, type);
    cout<<1;

    ofstream file1("AccountHolder.dat", ios::binary);
    file1.write(reinterpret_cast<char*>(user), sizeof(AccountHolder));
    file1.close();
}

char *getCurrentDate()
{
    time_t now = time(0);
    char *buf = new char[10];
    strftime(buf, sizeof(buf), "%d-%m-%Y", localtime(&now));
    return buf;
}

chrono::system_clock::time_point stringToDate(string &dateStr)
{
    istringstream ss(dateStr);
    tm tm = {};
    ss >> get_time(&tm, "%Y-%m-%d");
    return chrono::system_clock::from_time_t(mktime(&tm));
}

int calculateAge(string current, string deadline)
{
    auto time1 = stringToDate(current);
    auto time2 = stringToDate(deadline);

    auto duration = time2 - time1;
    int days = chrono::duration_cast<chrono::seconds>(duration).count() / (60 * 60 * 24);

    return days;
}

void Bank::drawInputWindow()
{
    bool should_exit = false;
    Rectangle UsernameBox = {300, 195, 200, 30};
    Rectangle PasswordBox = {300, 245, 200, 30};
    Rectangle Login_Button = {300, 300, 90, 40};
    Rectangle createAcc_Button = {410, 300, 90, 40};
    Rectangle Exit_Button = {355, 360, 90, 40};

    static int x = 0;
    bool username_clicked = false;
    bool password_clicked = false;
    char name[20] = "";
    int name_len = 0;
    char pass[20] = "";
    int pass_len = 0;
    char key;
    string status_msg = "";
    Color status_color = RED;
    float status_time = 0.0f;
    bool logInSuccess = false;

    Texture2D background = LoadTexture("BackGround.png");
    Color title={50, 50, 50, 255};

    while ([&]()
           { if(WindowShouldClose()) exit(0); return true; }())
    {
        bool mouseOnUsername = CheckCollisionPointRec(GetMousePosition(), UsernameBox);
        bool mouseOnPassword = CheckCollisionPointRec(GetMousePosition(), PasswordBox);
        bool mouseOnLogin = CheckCollisionPointRec(GetMousePosition(), Login_Button);
        should_exit = (CheckCollisionPointRec(GetMousePosition(), Exit_Button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));

        if (should_exit)
        {
            exit(0);
        }

        if (CheckCollisionPointRec(GetMousePosition(), createAcc_Button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            drawCreateAccScreen();
            break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);

        DrawText("My Bank", 300, 50, 50, title);

        if (!username_clicked || !password_clicked)
        {
            DrawText("Username:", 200, 200, 20, BLACK);
            DrawRectangleRec(UsernameBox, LIGHTGRAY);

            DrawText("Password:", 200, 250, 20, BLACK);
            DrawRectangleRec(PasswordBox, LIGHTGRAY);

            DrawRectangleRec(Login_Button, GREEN);
            DrawText("Login", 320, 310, 20, BLACK);

            DrawRectangleRec(createAcc_Button, GREEN);
            DrawText("Sign Up", 420, 310, 20, BLACK);

            DrawRectangleRec(Exit_Button, RED);
            DrawText("Exit", 385, 370, 20, BLACK);
        }

        if (mouseOnLogin && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (name[0] == '\0' || pass[0] == '\0')
            {
                status_msg = "Please enter username and password";
                status_color = RED;
                status_time = GetTime();
            }
            else if (log_in(name, pass))
            {
                status_msg = "Login successful!";
                status_color = DARKGREEN;
                status_time = GetTime();
                logInSuccess = true;
            }
            else
            {
                status_msg = "Invalid username or password";
                status_color = RED;
                status_time = GetTime();
            }
        }

        if (!status_msg.empty())
        {
            float elapsed = GetTime() - status_time;

            if (elapsed < 2.0f)
            {
                DrawText(status_msg.c_str(), (GetScreenWidth() - MeasureText(status_msg.c_str(), 20)) / 2, 420, 20, status_color);
            }
            else{

                status_msg.clear();
            }

            if (logInSuccess && elapsed >= 2.0f)
            {
                break;
            }
        }

        if (mouseOnUsername && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            username_clicked = true;
            password_clicked = false;
        }

        if (mouseOnPassword && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            password_clicked = true;
            username_clicked = false;
        }

        if (username_clicked)
        {
            char hidden[20];
            for (int i = 0; i < pass_len; i++)
            {
                hidden[i] = '*';
            }
            hidden[pass_len] = '\0';

            DrawText(hidden, PasswordBox.x + 5, PasswordBox.y + 5, 20, BLACK);

            DrawRectangleLinesEx(UsernameBox, 2, BLUE);

            DrawText(name, UsernameBox.x + 5, UsernameBox.y + 5, 20, BLACK);

            if ((x++ % 60) < 30)
            {
                DrawText("|", UsernameBox.x + 5 + MeasureText(name, 20), UsernameBox.y + 5, 20, BLACK);
            }

            key = GetCharPressed();
            while (key > 0)
            {
                if (name_len < 19 && key >= 32 && key <= 125)
                {
                    name[name_len++] = (char)key;
                    name[name_len] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && name_len > 0)
            {
                name[--name_len] = '\0';
            }
        }

        if (password_clicked)
        {
            DrawText(name, UsernameBox.x + 5, UsernameBox.y + 5, 20, BLACK);
            DrawRectangleLinesEx(PasswordBox, 2, BLUE);

            char hidden[20];
            for (int i = 0; i < pass_len; i++)
            {
                hidden[i] = '*';
            }
            hidden[pass_len] = '\0';

            DrawText(hidden, PasswordBox.x + 5, PasswordBox.y + 5, 20, BLACK);

            if ((x++ % 60) < 30)
            {
                DrawText("|", PasswordBox.x + 5 + MeasureText(hidden, 20), PasswordBox.y + 5, 20, BLACK);
            }

            key = GetCharPressed();
            while (key > 0)
            {
                if (pass_len < 19 && key >= 32 && key <= 125)
                {
                    pass[pass_len++] = (char)key;
                    pass[pass_len] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && pass_len > 0)
            {
                pass[--pass_len] = '\0';
            }
        }

        EndDrawing();
    }
    ClearBackground(SKYBLUE);
    UnloadTexture(background);
    return;
}

void Bank::drawCreateAccScreen()
{
    bool selectType = false;
    bool current = false;
    bool saving = false;
    bool first_click=true;
    bool CreationSuccess=false;
    int clicked=-1;
    int x=0;
    char key;

    string status_msg="";
    Color status_color=RED;
    float status_time=0.0f;

    char boxName[8][20];
    strcpy(boxName[0], "Name");
    strcpy(boxName[1], "Username");
    strcpy(boxName[2], "Contact");
    strcpy(boxName[3], "Address");
    strcpy(boxName[4], "Age");
    strcpy(boxName[5], "Salary");
    strcpy(boxName[6], "Password");
    strcpy(boxName[7], "Confirm Password");

    Rectangle box[8];
    float startY = 130;
    for (int i = 0; i < 8; i++)
    {
        box[i] = {(float)GetScreenWidth() / 2 - 100, startY + (i * 50), 200, 40};
    }

    Rectangle current_button = {(float)(GetScreenWidth() / 2 - 160), 80, 140, 40};
    Rectangle saving_button = {(float)(GetScreenWidth() / 2 + 20), 80, 140, 40};
    Rectangle Confirm_button = {(float)(GetScreenWidth() / 2 -160), 550, 140, 40};
    Rectangle Exit_button = {(float)(GetScreenWidth() / 2 + 20), 550, 140, 40};

    char accType[20];
    char input[8][20];
    for(int i=0; i<8; i++){
        strcpy(input[i], "");
    }

    Texture2D background = LoadTexture("BackGround.png");
    Color title={50, 50, 50, 255};

    while ([&]()
           { if(WindowShouldClose()) exit(0); return true; }())
    {

        BeginDrawing();
        ClearBackground(RAYWHITE);
    DrawTexture(background,0 ,0, WHITE);

        DrawText("My Bank", GetScreenWidth() / 2 - MeasureText("My Bank", 30) / 2, 30, 30, title);

        if (!selectType)
        {
            DrawRectangleRec(current_button, GREEN);
            DrawText("Current\nAccount", current_button.x + 20, current_button.y + 3, 20, BLACK);

            DrawRectangleRec(Exit_button, RED);
            DrawText("Exit", Exit_button.x + 50, Exit_button.y + 8, 20, BLACK);

            DrawRectangleRec(Confirm_button, GREEN);
            DrawText("Confirm", Confirm_button.x + 30, Confirm_button.y + 8, 20, BLACK);

            DrawRectangleRec(saving_button, GREEN);
            DrawText("Saving\nAccount", saving_button.x + 20, saving_button.y + 3, 20, BLACK);
        }
        if (CheckCollisionPointRec(GetMousePosition(), current_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            current = true;
            saving = false;
            selectType = true;
        }
        if (CheckCollisionPointRec(GetMousePosition(), saving_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            current = false;
            saving = true;
            selectType = true;
        }
        if (CheckCollisionPointRec(GetMousePosition(), Exit_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            exit(0);
        }

        if (CheckCollisionPointRec(GetMousePosition(), Confirm_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if(strcmp(input[7], input[6])){
                status_msg="Password Doesn't Match";
                status_time=GetTime();
                status_color=RED;
            }
            else if(!current && !saving){
                status_msg="Select Account type";
                status_time=GetTime();
                status_color=RED;
            }
            else{
                CreationSuccess=true;
                status_time=GetTime();
                status_color=GREEN;
                status_msg="Account Created Successfully";
            }
            for(int i=0; i<8; i++){
                if(input[i][0]=='\0'){
                    status_msg="Please Enter All Details";
                    status_time=GetTime();
                    status_color=RED;
                    CreationSuccess=false;
                    break;
                }
            }
        }

        if (!status_msg.empty())
        {
            float elapsed = GetTime() - status_time;

            if (elapsed < 2.0f)
            {
                DrawText(status_msg.c_str(), (GetScreenWidth() - MeasureText(status_msg.c_str(), 20)) / 2, 522, 20, status_color);
            }
            else
            {
                status_msg.clear();
            }

            if (CreationSuccess && elapsed >= 2.0f)
            {
                createAccount(input, 8, accType);
                break;
            }
        }

        if (current)
        {
            DrawRectangleRec(Exit_button, RED);
            DrawText("Exit", Exit_button.x + 50, Exit_button.y + 8, 20, BLACK);

            DrawRectangleRec(Confirm_button, GREEN);
            DrawText("Confirm", Confirm_button.x + 30, Confirm_button.y + 8, 20, BLACK);
            DrawRectangleRec(current_button, GREEN);
            DrawRectangleLinesEx(current_button, 2, BLACK);
            DrawText("Current\nAccount", current_button.x + 20, current_button.y + 3, 20, BLACK);

            DrawRectangleRec(saving_button, GREEN);
            DrawText("Saving\nAccount", saving_button.x + 20, saving_button.y + 3, 20, BLACK);

            strcpy(accType, "Current");
        }

        if (saving)
        {
            DrawRectangleRec(Exit_button, RED);
            DrawText("Exit", Exit_button.x + 50, Exit_button.y + 8, 20, BLACK);

            DrawRectangleRec(Confirm_button, GREEN);
            DrawText("Confirm", Confirm_button.x + 30, Confirm_button.y + 8, 20, BLACK);
            DrawRectangleRec(current_button, GREEN);
            DrawText("Current\nAccount", current_button.x + 20, current_button.y + 3, 20, BLACK);

            DrawRectangleRec(saving_button, GREEN);
            DrawRectangleLinesEx(saving_button, 2, BLACK);
            DrawText("Saving\nAccount", saving_button.x + 20, saving_button.y + 3, 20, BLACK);
            strcpy(accType, "Saving");
        }
        
        for(int i=0; i<8; i++){
            if(first_click){
                first_click=false;
                break;
            }
            if(CheckCollisionPointRec(GetMousePosition(), box[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                clicked=i;
            }
        }

        if(clicked<0){
            for(int i=0; i<8; i++){
                DrawRectangleRec(box[i], LIGHTGRAY);
                DrawText(boxName[i], box[i].x + 10, box[i].y + 10, 20, BLACK);
            }
        }
        else{
            for(int i=0; i<8; i++){
                if(i==clicked) continue;
                DrawRectangleRec(box[i], LIGHTGRAY);
                if(input[i][0]=='\0'){
                    DrawText(boxName[i], box[i].x + 10, box[i].y + 10, 20, BLACK);
                }
                else{
                    DrawText(input[i], box[i].x + 10, box[i].y + 10, 20, DARKBLUE);
                }
            }
            int i=clicked;
            DrawRectangleLinesEx(box[i], 2, BLACK);
            DrawRectangleRec(box[i], WHITE);

            DrawText(input[i], box[i].x + 5, box[i].y + 5, 20, DARKBLUE);

            if ((x++ % 60) < 30)
            {
                DrawText("|", box[i].x + 5 + MeasureText(input[i], 20), box[i].y + 10, 20, BLACK);
            }

            key = GetCharPressed();
            size_t len=strlen(input[i]);
            while (key > 0)
            {
                if (strlen(input[i]) < 19 && key >= 32 && key <= 125)
                {
                    input[i][len++] = (char)key;
                    input[i][len] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && strlen(input[i]) > 0)
            {
                input[i][--len] = '\0';
            }
        }
        
        EndDrawing();
    }
    UnloadTexture(background);
    ClearBackground(SKYBLUE);

}
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

void splashScreen()
{
    bool startAnimation = false;
    int titleY = 250;
    int targetY = 50;
    bool onSplashScreen = true;
    float animationSpeed = 4.0f;

    Texture2D background = LoadTexture("BackGround.png");
    Color title={50, 50, 50, 255};

    while (onSplashScreen && [&](){ if(WindowShouldClose()) exit(0); return true;}())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background,0 ,0, WHITE);
        DrawText("My Bank", 300, titleY, 50, title);
        

        if (IsKeyPressed(KEY_ENTER))
        {
            startAnimation = true;
        }

        if (startAnimation)
        {
            if (titleY > targetY)
            {
                titleY -= animationSpeed;
            }
            else
            {
                onSplashScreen = false;
            }
        }
        EndDrawing();
    }
    ClearBackground(SKYBLUE);
    UnloadTexture(background);
    
    return;
}

using namespace std;

int main()
{
    Bank b;

    InitWindow(800, 600, "Banking System");
    SetTargetFPS(60);
    bool splash = true;
    bool logInSuccess=false;
    Texture2D background = LoadTexture("BackGround.png");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(background,0 ,0, WHITE);

        if (splash)
        {
            splashScreen();
            splash = false;
        }
        if(!logInSuccess){
            b.Start();
            logInSuccess=true;
        }
        EndDrawing();
        
    }
    UnloadTexture(background);

    CloseWindow();
}
/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Ending Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include "extras/raygui.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
const char* endMessage;
const char* scoreMessage;
const char* enemiesMessage;
const char* friendliesMessage;
const char* shellsFiredMessage;
const char* bestShotMessage;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    // TODO: Initialize ENDING screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    switch (endCondition) {
        case WIN:
            endMessage = "You won!";
            break;
        case PYRRHIC_WIN:
            endMessage = "You won! But at what cost?";
            break;
        case LOSE:
            endMessage = "You lost! Try again?";
            break;
        default:
            endMessage = "End condition not set properly!";
    }

    scoreMessage = TextFormat("Score: %d", score);
    enemiesMessage = TextFormat("Enemies you killed: %d", enemiesKilled);
    friendliesMessage = TextFormat("Friendlies you killed: %d", friendliesKilled);
    shellsFiredMessage = TextFormat("Shells you fired: %d", shellsFired);
    bestShotMessage = TextFormat("Most kills in one shot: %d", bestShot);
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    // TODO: Update ENDING screen variables here!
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    // TODO: Draw ENDING screen here!
    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();

    DrawText(endMessage, screenWidth/2 - (TextLength(endMessage)*8), 100, 30, WHITE);
    DrawText(TextFormat("Score: %d", score), screenWidth/2 - 6, 200, 30, WHITE);
    DrawText(TextFormat("Enemies you killed: %d", enemiesKilled), screenWidth/2 - 186, 250, 30, WHITE);
    DrawText(TextFormat("Friendlies you killed: %d", friendliesKilled), screenWidth/2 - 219, 300, 30, WHITE);
    DrawText(TextFormat("Shells you fired: %d", shellsFired), screenWidth/2 - 159, 350, 30, WHITE);
    DrawText(TextFormat("Most kills in one shot: %d", bestShot), screenWidth/2 - 243, 400, 30, WHITE);

    if (GuiButton((Rectangle){screenWidth/2 - 50, 450, 100, 25}, "Retry"))
    {
        finishScreen = 2;
    }

    if (GuiButton((Rectangle){screenWidth/2 - 50, 480, 100, 25}, "Menu"))
    {
        finishScreen = 1;
    }
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}
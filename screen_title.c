/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include "extras/raygui.h"
#include "screens.h"
#include "decals.h"
#include "animations.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

extern ImpactDecals impactDecals;
extern CorpseDecals corpseDecals;
extern ImpactAnimations impactAnimations;
extern Animation shootAnimation;

float timer = 2.0f;
float time_max = 5.0f;

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    InitDecals();
    InitAnimations();

//    Vector2 pos1 = (Vector2){200, 200};
//    Vector2 pos1 = (Vector2){GetRandomValue(100, GetScreenWidth()-100), GetRandomValue(100, GetScreenHeight()-100)};
//    SpawnImpactAnimation(pos1);
//    SpawnDecal(IMPACT, pos1);
//    SetSoundVolume(fxImpact, 0.1f);
//    PlaySound(fxImpact);

//    Vector2 pos2 = (Vector2){700, 450};
//    SpawnImpactAnimation(pos2);
//    SpawnDecal(IMPACT, pos2);
}

// Title Screen Update logic
void UpdateTitleScreen(void)
{
    // TODO: Update TITLE screen variables here!

    // Press enter or tap to change to GAMEPLAY screen
//    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
//    {
//        finishScreen = 1;   // OPTIONS
//        finishScreen = 2;   // GAMEPLAY
//        PlaySound(fxCoin);
//    }

    UpdateAnimations(GetFrameTime());

    timer -= GetFrameTime();

    if (timer <= 0)
    {
        Vector2 randomPos = (Vector2){GetRandomValue(100, GetScreenWidth()-100), GetRandomValue(100, GetScreenHeight()-100)};
        SpawnImpactAnimation(randomPos);
        SpawnDecal(IMPACT, randomPos);
        SetSoundVolume(fxImpact, 0.1f);
        PlaySound(fxImpact);
        timer = time_max;
    }
}

// Title Screen Draw logic
void DrawTitleScreen(void)
{
    ClearBackground(BLACK);
    // TODO: Draw TITLE screen here!
//    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
//    DrawTextEx(font, "TITLE SCREEN", (Vector2){ 20, 10 }, font.baseSize*3, 4, DARKGREEN);
//    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

    DrawTexture(worldTexture, 0, 0, WHITE);
    DrawDecals();
    DrawAnimations();

    DrawText("DANGER CLOSE!", GetScreenWidth()/2 - (TextLength("DANGER CLOSE!")*15), 130, 50, BLACK);
//    // Draw base
//    DrawSprite(20, 7, (Vector2){500, 300}, (Vector2){32, 32}, 0);
//    // Draw turret
//    DrawSprite(19, 10, (Vector2){500, 300}, (Vector2){32, 38}, 0);

    if (GuiButton((Rectangle){GetScreenWidth()/2 - 50, 250, 100, 25}, "Options"))
        finishScreen = 1;
    if (GuiButton((Rectangle){GetScreenWidth()/2 - 50, 280, 100, 25}, "Play"))
        finishScreen = 2;
//    if (GuiButton((Rectangle){350, 210, 100, 25}, "Quit"))
//        finishScreen = 0;
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    // TODO: Unload TITLE screen variables here!
    ResetDecals();
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}
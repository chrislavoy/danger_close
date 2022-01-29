/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Options Screen Functions Definitions (Init, Update, Draw, Unload)
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

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Options Screen Functions Definition
//----------------------------------------------------------------------------------

// Options Screen Initialization logic
void InitOptionsScreen(void)
{
    // TODO: Initialize OPTIONS screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Options Screen Update logic
void UpdateOptionsScreen(void)
{
    // TODO: Update OPTIONS screen variables here!
}

// Options Screen Draw logic
void DrawOptionsScreen(void)
{
    // TODO: Draw OPTIONS screen here!
    ClearBackground(BLACK);
    DrawTexture(worldTexture, 0, 0, WHITE);
    DrawRectangle(250, 150, 400, 220, RAYWHITE);
    DrawText("Options", 410, 170, 20, BLACK);
    masterVolume = GuiSlider((Rectangle){350, 210, 200, 25}, "Master Volume", ((void *) 0), masterVolume, 0, 1.0f);
    musicVolume = GuiSlider((Rectangle){350, 250, 200, 25}, "Music Volume", ((void *) 0), musicVolume, 0, 1.0f);
    ezAiming = GuiCheckBox((Rectangle){350, 285, 25, 25}, "Bigger Direction Indicator", ezAiming);
    if (GuiButton((Rectangle){350, 320, 200, 25}, "Back")) finishScreen = 1;

    DrawRectangle(250, 380, 400, 200, RAYWHITE);
    DrawText("Controls", 410, 400, 20, BLACK);
    DrawText("A & D | Left & Right - Adjust Angle\nW & S | Up & Down - Adjust Range\nSpace | Left Ctrl | Right Ctrl - Fire\nEsc - Pause/Unpause", 270, 430, 20, BLACK);

    SetMasterVolume(masterVolume);
    SetMusicVolume(music, musicVolume);
}

// Options Screen Unload logic
void UnloadOptionsScreen(void)
{
    // TODO: Unload OPTIONS screen variables here!
}

// Options Screen should finish?
int FinishOptionsScreen(void)
{
    return finishScreen;
}
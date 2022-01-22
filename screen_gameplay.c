/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include "raymath.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

const float TURN_RATE = 100.0f;

Camera2D worldCamera;
Camera2D mapCamera;
RenderTexture worldRenderTexture;
RenderTexture mapRenderTexture;
Player player;
Ammo ammo;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    InitAmmo();
    InitPlayer();

    worldCamera.target = player.position;
    worldCamera.offset = Vector2Zero();
    worldCamera.rotation = 0;
    worldCamera.zoom = 1;

    worldRenderTexture = LoadRenderTexture(GetScreenWidth()-300, GetScreenHeight());

    mapCamera.target = player.position;
    mapCamera.offset = Vector2Zero();
    mapCamera.rotation = 0;
    mapCamera.zoom = 1;

    mapRenderTexture = LoadRenderTexture(GetScreenWidth()-300, GetScreenHeight());
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    float dt = GetFrameTime();

    // Press enter or tap to change to ENDING screen
//    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
//    {
//        finishScreen = 1;
//        PlaySound(fxCoin);
//    }

    if (IsKeyDown(KEY_LEFT))
    {
        player.rotation -= TURN_RATE * dt;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        player.rotation += TURN_RATE * dt;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        Shoot();
    }

    if (player.rotation > 360)
    {
        player.rotation -= 360;
    }
    else if (player.rotation < 0)
    {
        player.rotation += 360;
    }

//    player.rectangle = (Rectangle){player.position.x, player.position.y, 5, 25};

    UpdateAmmo(dt);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
//    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    DrawAmmo();
    DrawRectanglePro(player.rectangle, player.origin, player.rotation, player.color);
//    DrawTextEx(font, "GAMEPLAY SCREEN", (Vector2){ 20, 10 }, font.baseSize*3, 4, MAROON);
//    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
//    DrawText(TextFormat("Player rotation: %f", player.rotation), 20, 10, 20, DARKGRAY);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

void Shoot()
{
//    TraceLog(LOG_INFO, "Shoot");
    PlaySound(fxShoot);
//    Shell shell = ammo.shells[ammo.shellIterator];
    ammo.shells[ammo.shellIterator].rotation = player.rotation;
    ammo.shells[ammo.shellIterator].position = player.position;
    ammo.shells[ammo.shellIterator].active = true;
    ammo.shells[ammo.shellIterator].velocity = (Vector2){-sinf(ammo.shells[ammo.shellIterator].rotation * DEG2RAD), cosf(ammo.shells[ammo.shellIterator].rotation * DEG2RAD)};
//    ammo.shells[ammo.shellIterator].travelDistnace = 0;
    ammo.shellIterator++;
    if (ammo.shellIterator >= ammo.count)
    {
        ammo.shellIterator = 0;
    }
}

void InitPlayer()
{
    player.origin = (Vector2){2.5f, 0};
    player.position = (Vector2){ GetScreenWidth()/2, GetScreenHeight()/2 };
    player.rectangle = (Rectangle){player.position.x, player.position.y, 5, 25};
    player.rotation = 180;
    player.color = RED;
}

void InitAmmo()
{
    ammo.capacity = MAX_SHELLS;
    ammo.count = MAX_SHELLS;

    for (int i = 0; i < ammo.count; ++i)
    {
        ammo.shells[i].origin = (Vector2){0, 0};
        ammo.shells[i].position = (Vector2){0, 0};
        ammo.shells[i].rectangle = (Rectangle){0, 0, 5, 5};
        ammo.shells[i].rotation = 0;
        ammo.shells[i].color = BLACK;
        ammo.shells[i].active = false;
        ammo.shells[i].velocity = (Vector2){0, 0};
        ammo.shells[i].range = 1000;
//        ammo.shells[i].travelDistnace = 0;
    }
}

void Explode(int shellIndex)
{
//    TraceLog(LOG_INFO, "Explode");
    PlaySound(fxImpact);
    ammo.shells[shellIndex].active = false;
}

void UpdateAmmo(float dt)
{
    for (int i = 0; i < ammo.count; ++i)
    {
//        Shell shell = ammo.shells[i];
        if (ammo.shells[i].active)
        {
            if (Vector2Distance(player.position, ammo.shells[i].position) > ammo.shells[i].range)
            {
                Explode(i);
            }
            else
            {
                ammo.shells[i].position = Vector2Add(ammo.shells[i].position, Vector2Scale(ammo.shells[i].velocity, 600 * dt));
                ammo.shells[i].rectangle = (Rectangle){ammo.shells[i].position.x, ammo.shells[i].position.y, 5, 5};
            }
        }
    }
}

void DrawAmmo()
{
    for (int i = 0; i < ammo.count; ++i)
    {
        Shell shell = ammo.shells[i];
        if (shell.active)
        {
            DrawRectanglePro(shell.rectangle, shell.origin, shell.rotation, shell.color);
        }
    }
}
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
#include "raymath.h"
#include "extras/raygui.h"
#include "screens.h"

// IDK why I need to redefine this macro here but I do
//#define NULL (void*)0

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

const float TURN_RATE = 100.0f;

const float MAX_FIRE_RANGE = 1000;
float fireRange = MAX_FIRE_RANGE;

//Camera2D worldCamera;
Camera2D mapCamera;
//RenderTexture mainRenderTexture;
RenderTexture sideRenderTexture;
//Shader shader;
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

//    worldCamera.target = player.position;
//    worldCamera.offset = Vector2Zero();
//    worldCamera.rotation = 0;
//    worldCamera.zoom = 1;

//    mainRenderTexture = LoadRenderTexture(600, GetScreenHeight());

    mapCamera.target = (Vector2) {player.position.x - 1000, player.position.y - 1000};
    mapCamera.offset = Vector2Zero();
    mapCamera.rotation = 0;
    mapCamera.zoom = 0.3f;

    sideRenderTexture = LoadRenderTexture(600, 600);
//    sideRenderTexture = LoadRenderTexture(600, 600);
    SetTextureFilter(sideRenderTexture.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

//    shader = LoadShader(NULL, NULL);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    float dt = GetFrameTime();

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }

    if (IsKeyDown(KEY_LEFT))
    {
        player.rotation -= TURN_RATE * dt;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        player.rotation += TURN_RATE * dt;
    }

    if (IsKeyDown(KEY_UP))
    {
        fireRange += 100 * dt;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        fireRange -= 100 * dt;
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

    if (fireRange > MAX_FIRE_RANGE)
    {
        fireRange = MAX_FIRE_RANGE;
    }
    else if (fireRange < 0)
    {
        fireRange = 0;
    }

    UpdateAmmo(dt);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
//    BeginTextureMode(sideRenderTexture);
//        ClearBackground(RAYWHITE);
//        BeginMode2D(mapCamera);
//            DrawRectangle(player.position.x - 25, player.position.y - 25, 50, 50, BLUE);
//            DrawCircleLines(player.position.x, player.position.y, fireRange, RED);
//            DrawCircle(player.position.x, player.position.y, fireRange, ColorAlpha(RED, 0.25));
//            for (int i = 0; i < ammo.capacity; ++i)
//            {
//                Shell shell = ammo.shells[i];
//                if (shell.active)
//                {
//                    DrawRectangle(shell.position.x, shell.position.y, 50, 50, BLACK);
//                }
//            }
//        EndMode2D();
//    EndTextureMode();

// DEBUG DRAWS
//        DrawRectangle(player.position.x - 25, player.position.y - 25, 50, 50, BLUE);
//        DrawCircleLines(player.position.x, player.position.y, fireRange, RED);

    // TODO: Draw GAMEPLAY screen here!

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawCircleV(Vector2Zero(), 10, GREEN);
    DrawRectangle(600, 0, 300, GetScreenHeight(), LIGHTGRAY);
//    DrawTexturePro(
//        sideRenderTexture.texture,
//        (Rectangle){0, 0, sideRenderTexture.texture.width,sideRenderTexture.texture.height},
//        (Rectangle){635, 2, 230, 230},
//        (Vector2){0, 0},
//        0,
//        WHITE);
//    DrawTexturePro(sideRenderTexture.texture, (Rectangle){ 0.0f, 0.0f, (float)sideRenderTexture.texture.width, (float)-sideRenderTexture.texture.height },
//                   (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
//                                (float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    DrawAmmo();
    DrawPlayer();

    player.rotation = GuiSlider((Rectangle){670, 250, 175, 25}, "Rotation", TextFormat("%.2f", player.rotation), player.rotation, 0, 360);
    fireRange = GuiSlider((Rectangle){670, 300, 175, 25}, "Distance", TextFormat("%.2f", fireRange), fireRange, 0, MAX_FIRE_RANGE);
    if (GuiButton((Rectangle){670, 350, 175, 25}, "Fire"))
    {
        Shoot();
    }

    if (GuiButton((Rectangle){670, 400, 175, 25}, "Reload"))
    {
        Reload();
    }

    GuiLabel((Rectangle){670, 450, 175, 25}, TextFormat("%d / %d", ammo.count, ammo.capacity));
    EndDrawing();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
//    UnloadRenderTexture(mainRenderTexture);
    UnloadRenderTexture(sideRenderTexture);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

void Shoot()
{
    if (ammo.count > 0)
    {
        PlaySound(fxShoot);

        ammo.shells[ammo.shellIterator].rotation = player.rotation;
        ammo.shells[ammo.shellIterator].position = player.position;
        ammo.shells[ammo.shellIterator].active = true;
        ammo.shells[ammo.shellIterator].velocity = (Vector2){-sinf(ammo.shells[ammo.shellIterator].rotation * DEG2RAD), cosf(ammo.shells[ammo.shellIterator].rotation * DEG2RAD)};
        ammo.shells[ammo.shellIterator].range = fireRange;
        ammo.shellIterator++;

        ammo.count--;
    }
}

void InitPlayer()
{
    player.origin = (Vector2){2.5f, 0};
    player.position = (Vector2){ 300, GetScreenHeight()/2 };
    player.rectangle = (Rectangle){player.position.x, player.position.y, 5, 25};
    player.rotation = 180;
    player.color = RED;
}

void InitAmmo()
{
    ammo.capacity = MAX_SHELLS;
    ammo.count = MAX_SHELLS;

    for (int i = 0; i < ammo.capacity; ++i)
    {
        ammo.shells[i].origin = (Vector2){0, 0};
        ammo.shells[i].position = (Vector2){0, 0};
        ammo.shells[i].rectangle = (Rectangle){0, 0, 5, 5};
        ammo.shells[i].rotation = 0;
        ammo.shells[i].color = BLACK;
        ammo.shells[i].active = false;
        ammo.shells[i].velocity = (Vector2){0, 0};
        ammo.shells[i].range = 0;
    }
}

void Explode(int shellIndex)
{
    PlaySound(fxDistantImpact);
    ammo.shells[shellIndex].active = false;
}

void UpdateAmmo(float dt)
{
    for (int i = 0; i < ammo.capacity; ++i)
    {
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

void DrawPlayer()
{
    DrawRectangle(player.position.x - 10, 240, 20, 20, BLACK);
    DrawRectanglePro(player.rectangle, player.origin, player.rotation, player.color);
}

void DrawAmmo()
{
    for (int i = 0; i < ammo.capacity; ++i)
    {
        Shell shell = ammo.shells[i];
        if (shell.active)
        {
            DrawRectanglePro(shell.rectangle, shell.origin, shell.rotation, shell.color);
        }
    }
}

void Reload()
{
    ammo.count = ammo.capacity;
    ammo.shellIterator = 0;
}
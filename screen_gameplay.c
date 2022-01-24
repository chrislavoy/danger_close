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
const float MAX_FIRE_RANGE = 3000;
const float MIN_FIRE_RANGE = 200;
const int MAX_ENEMIES = 100;
const float TIME_BETWEEN_FEEDBACK = 10.0f;

float fireRange = MAX_FIRE_RANGE;
float feedbackTimer = 0;
float soundVolume = 1.0f;

//Camera2D worldCamera;
Camera2D mapCamera;
//RenderTexture mainRenderTexture;
RenderTexture2D sideRenderTexture;
//Shader shader;
Player player;
Ammo ammo;

Enemy enemies[100];

Vector2 fireTargetPos = (Vector2){0, 0};

char * feedbackMessage;
bool showMessage = false;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

void ShowFeedback();

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    InitAmmo();
    InitPlayer();
    InitEnemies();

//    worldCamera.target = player.position;
//    worldCamera.offset = Vector2Zero();
//    worldCamera.rotation = 0;
//    worldCamera.zoom = 1;

//    mainRenderTexture = LoadRenderTexture(600, GetScreenHeight());

    sideRenderTexture = LoadRenderTexture(600, 600);
    SetTextureFilter(sideRenderTexture.texture, TEXTURE_FILTER_BILINEAR);

    mapCamera.target = player.position;
    mapCamera.offset = (Vector2) {sideRenderTexture.texture.width/2, sideRenderTexture.texture.height/2};
    mapCamera.rotation = 0;
    mapCamera.zoom = 0.1f;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    float dt = GetFrameTime();

//    mapCamera.rotation += 25 * dt;

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
    else if (fireRange < MIN_FIRE_RANGE)
    {
        fireRange = MIN_FIRE_RANGE;
    }

    UpdateAmmo(dt);
    UpdateEnemies();

    if (feedbackTimer > 0)
    {
        feedbackTimer -= dt;

        if (feedbackTimer < 0)
        {
            feedbackTimer = 0;
            feedbackMessage = "";
        }
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
// DEBUG DRAWS
//        DrawCircleLines(player.position.x, player.position.y, fireRange, RED);

    // TODO: Draw GAMEPLAY screen here!

    BeginTextureMode(sideRenderTexture);
        ClearBackground(RAYWHITE);
        BeginMode2D(mapCamera);
        DrawRectangle(player.position.x - 25, player.position.y - 25, 50, 50, BLUE);
//        DrawCircleLines(player.position.x, player.position.y, fireRange, RED);
        DrawCircle(player.position.x, player.position.y, fireRange, ColorAlpha(RED, 0.25));
        for (int i = 0; i < ammo.capacity; ++i)
        {
            Shell shell = ammo.shells[i];
            if (shell.active)
            {
                DrawRectangle(shell.position.x, shell.position.y, 50, 50, BLACK);
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            if (enemies[i].active)
            {
                DrawRectangle(enemies[i].position.x, enemies[i].position.y, 50, 50, RED);
            }
        }
        EndMode2D();
    EndTextureMode();

    ClearBackground(RAYWHITE);
    DrawAmmo();
    DrawPlayer();
    DrawEnemies();
    DrawRectangle(600, 0, 300, GetScreenHeight(), LIGHTGRAY);
    DrawTexturePro(
        sideRenderTexture.texture,
        (Rectangle){0, 0, -sideRenderTexture.texture.width,sideRenderTexture.texture.height},
        (Rectangle){750, 117, 230, 230},
        (Vector2){115, 115},
        180,
        WHITE);

    // TODO: Change these GUI elements
    player.rotation = GuiSlider((Rectangle){670, 250, 175, 25}, "Rotation", TextFormat("%.2f", player.rotation), player.rotation, 0, 360);
    fireRange = GuiSlider((Rectangle){670, 300, 175, 25}, "Distance", TextFormat("%.2f", fireRange), fireRange, MIN_FIRE_RANGE, MAX_FIRE_RANGE);
//    soundVolume = GuiSlider((Rectangle){670, 325, 175, 25}, "Sound Volume", TextFormat("%.2f", soundVolume), soundVolume, 0, 1.0f);
    if (GuiButton((Rectangle){670, 350, 175, 25}, "Fire"))
    {
        Shoot();
    }

    if (GuiButton((Rectangle){670, 400, 175, 25}, "Reload"))
    {
        Reload();
    }

    GuiLabel((Rectangle){670, 450, 175, 25}, TextFormat("%d / %d", ammo.count, ammo.capacity));

    ShowMessage();
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

void InitPlayer()
{
    player.origin = (Vector2){32, 38};
    player.position = (Vector2){ 300, GetScreenHeight()/2 };
    player.rectangle = (Rectangle){player.position.x, player.position.y, 64, 64};
    player.rotation = 180;
    player.color = WHITE;
}

void InitAmmo()
{
    ammo.capacity = MAX_SHELLS;
    ammo.count = MAX_SHELLS;

    for (int i = 0; i < ammo.capacity; ++i)
    {
        ammo.shells[i].origin = (Vector2){32, 32};
        ammo.shells[i].position = (Vector2){0, 0};
        ammo.shells[i].rectangle = (Rectangle){0, 0, 64, 64};
        ammo.shells[i].rotation = 0;
        ammo.shells[i].color = WHITE;
        ammo.shells[i].active = false;
        ammo.shells[i].velocity = (Vector2){0, 0};
        ammo.shells[i].range = 0;
        ammo.shells[i].blastRadius = 100;
        ammo.shells[i].soundPlayed = false;
    }
}

void InitEnemies() {
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        enemies[i].origin = (Vector2){32, 32};
        enemies[i].position = Vector2Zero();
        enemies[i].rectangle = (Rectangle) {0, 0, 64, 64};
        enemies[i].rotation = 0;
        enemies[i].color = WHITE;
        enemies[i].movementSpeed = 1;
        enemies[i].targetPos = Vector2Zero();
        enemies[i].active = false;
    }

    enemies[0].position = (Vector2) {10, 10};
    enemies[0].targetPos = (Vector2) {800, 400};
    enemies[0].active = true;
}

void Shoot()
{
    if (ammo.count > 0)
    {
        SetSoundPitch(fxShoot, 1);
        PlaySound(fxShoot);

        ammo.shells[ammo.shellIterator].rotation = player.rotation;
        ammo.shells[ammo.shellIterator].position = player.position;
        ammo.shells[ammo.shellIterator].active = true;
        ammo.shells[ammo.shellIterator].velocity = (Vector2){-sinf(ammo.shells[ammo.shellIterator].rotation * DEG2RAD), cosf(ammo.shells[ammo.shellIterator].rotation * DEG2RAD)};
        ammo.shells[ammo.shellIterator].range = fireRange;
        ammo.shells[ammo.shellIterator].soundPlayed = false;
        ammo.shellIterator++;

        ammo.count--;
    }
}

void Explode(int shellIndex)
{
//    float distance = Vector2Distance(player.position, ammo.shells[shellIndex].position);
//    TraceLog(LOG_INFO, TextFormat("Distance: %f.2", distance));
    float volume = 1 - ammo.shells[shellIndex].range / (MAX_FIRE_RANGE + 200);
    SetSoundVolume(fxImpact, volume);
//    SetSoundPitch(fxImpact, 0.1);
    PlaySoundMulti(fxImpact);
//    PlaySound(fxImpact);
    ammo.shells[shellIndex].active = false;

    int enemiesHit = 0;

    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i].active)
        {
            if (CheckCollisionPointCircle(ammo.shells[shellIndex].position, enemies[i].position, ammo.shells[shellIndex].blastRadius))
            {
                enemies[i].active = false;
                enemiesHit++;
            }
        }
    }

    SetMessage("Good hits!");
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
//                if (Vector2Distance(player.position, ammo.shells[i].position) > ammo.shells[i].range - 200 && !ammo.shells[i].soundPlayed)
//                {
//                    // Set pitch relative to camera distance to impact
////                    SetSoundPitch(fxImpact, (2000 - Vector2Distance(player.position, ammo.shells[i].position)) / 2000);
//                    PlaySound(fxImpact);
//                    ammo.shells[i].soundPlayed = true;
//                }
                ammo.shells[i].position = Vector2Add(ammo.shells[i].position, Vector2Scale(ammo.shells[i].velocity, 400 * dt));
                ammo.shells[i].rectangle = (Rectangle){ammo.shells[i].position.x, ammo.shells[i].position.y, 64, 64};
            }
        }
    }
}

void UpdateEnemies()
{
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i].active)
        {
            if (Vector2Distance(enemies[i].position, enemies[i].targetPos) > 400)
            {
                enemies[i].position = Vector2MoveTowards(enemies[i].position, enemies[i].targetPos, enemies[i].movementSpeed);
                enemies[i].rotation = Vector2Angle(enemies[i].position, enemies[i].targetPos);
                enemies[i].rectangle = (Rectangle){enemies[i].position.x, enemies[i].position.y, 64, 64};
            }
        }
    }
}

void DrawPlayer()
{
    // Draw base
    DrawTexturePro(spriteSheet, (Rectangle){20*64, 7*64, 64, 64}, (Rectangle){player.position.x, player.position.y + 6, 64, 64}, player.origin, 0, player.color);
    // Draw turret
    DrawTexturePro(spriteSheet, (Rectangle){19*64, 10*64, 64, 64}, player.rectangle, player.origin, player.rotation - 180, player.color);
}

void DrawAmmo()
{
    for (int i = 0; i < ammo.capacity; ++i)
    {
        Shell shell = ammo.shells[i];
        if (shell.active)
        {
            DrawTexturePro(spriteSheet, (Rectangle){19*64, 11*64, 64, 64}, shell.rectangle, shell.origin, shell.rotation - 180, shell.color);
        }
    }
}

void DrawEnemies()
{
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i].active)
        {
//            DrawRectanglePro(enemies[i].rectangle, Vector2Zero(), enemies[i].rotation, enemies[i].color);
            DrawTexturePro(spriteSheet, (Rectangle){16*64, 10*64, 64, 64}, enemies[i].rectangle, enemies[i].origin, enemies[i].rotation - 180, enemies[i].color);
        }
    }
}

void Reload()
{
    ammo.count = ammo.capacity;
    ammo.shellIterator = 0;
}

void SetMessage(char* message)
{
    if (feedbackTimer < 0)
    {
        feedbackMessage = message;
        showMessage = true;
        feedbackTimer = TIME_BETWEEN_FEEDBACK;
    }
}

void ShowMessage()
{
//    DrawRectangleRounded((Rectangle){0, 300, 600, 200}, 0.01f, 1, WHITE);
//    DrawRectangleRoundedLines((Rectangle){0, 300, 600, 200}, 0.01f, 1, 5, BLACK);

    if (feedbackTimer > 0)
    {
        DrawRectangleRec((Rectangle){5, 305, 590, 190}, WHITE);
        DrawRectangleLinesEx((Rectangle){5, 305, 590, 190}, 3, BLACK);
        DrawText(feedbackMessage, 15, 315, 30, BLACK);
    }
}
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
#include "player.h"
#include "ammo.h"
#include "ground_units.h"
#include "decals.h"
#include "animations.h"

#define NULL ((void *)0)
#define RELOAD_TIME 3.5f
#define SHOT_TIME 0.5f
#define MAX_ROTATION 45.0f

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

const float TIME_BETWEEN_FEEDBACK = 5.0f;

float feedbackTimer = 0;
float feedbackReveal = 0;
float soundVolume = 1.0f;

Camera2D worldCamera;
Camera2D mapCamera;
RenderTexture mainRenderTexture;
RenderTexture2D sideRenderTexture;
Player player;
Ammo ammo;
Units enemyUnits;
Units friendlyUnits;
Vector2 fireTargetPos = (Vector2){0, 0};
int guiWidth = 250;
char feedbackMessage[50];
bool showMessage = false;
Rectangle radarRect = (Rectangle){776, 117, 230, 230};
int wave = 1;
bool paused = false;

ImpactDecals impactDecals;
CorpseDecals corpseDecals;

ImpactAnimations impactAnimations;
Animation shootAnimation;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
void DrawGui();
Vector2 RotationToVector(float);
bool MouseOverRadar();
bool MouseOverWorld();
bool PointInsideRect(Vector2, Rectangle);
Vector2 VirtualMouseToWorldPos();
int EnemiesRemaining();
int FriendliesRemaining();

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    paused = false;
    score = 0;
    enemiesKilled = 0;
    friendliesKilled = 0;
    shellsFired = 0;
    wave = 1;
    framesCounter = 0;
    finishScreen = 0;
    InitAmmo();
    InitPlayer();
	InitUnits(wave);
    InitDecals();
    InitAnimations();

	mainRenderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
//	SetTextureFilter(mainRenderTexture.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

    worldCamera.target = player.position;
    worldCamera.offset = (Vector2){GetScreenWidth()/2 - guiWidth/2, GetScreenHeight()/2};
    worldCamera.rotation = 0;
    worldCamera.zoom = 0.5f;

    sideRenderTexture = LoadRenderTexture(600, 600);
//    SetTextureFilter(sideRenderTexture.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

    mapCamera.target = Vector2Zero();
    mapCamera.offset = (Vector2) {sideRenderTexture.texture.width/2, sideRenderTexture.texture.height/2};
    mapCamera.rotation = 0;
    mapCamera.zoom = 0.1f;

    const int count_for_wave = wave * 200;

    SetMessage(TextFormat("Wave: %d\n%d Enemies incoming!", wave, count_for_wave));
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    if (!paused)
    {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_ESCAPE))
        {
            paused = true;
        }

        if (MouseOverRadar())
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                worldCamera.target = VirtualMouseToWorldPos();
            }
        }

        if (MouseOverWorld() && (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT) ))
        {
            worldCamera.target = Vector2Add(worldCamera.target, Vector2Scale(GetMouseDelta(), -1/worldCamera.zoom));
        }

#if defined(PLATFORM_WEB)
        if ((MouseOverRadar() || MouseOverWorld()) && GetMouseWheelMove() != 0)
        {
            worldCamera.zoom = Clamp(worldCamera.zoom - (GetMouseWheelMove() / 10), 0.1f, 1.0f);
        }
#else
        if ((MouseOverRadar() || MouseOverWorld()) && GetMouseWheelMove() != 0)
        {
            worldCamera.zoom = Clamp(worldCamera.zoom + (GetMouseWheelMove() / 10), 0.1f, 1.0f);
        }
#endif

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.targetRotation -= 5 * dt;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.targetRotation += 5 * dt;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) ChangeRange(500 * dt);
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) ChangeRange(-500 * dt);
        if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) Shoot();
//        if (IsKeyDown(KEY_R)) Reload();

//        if (IsGestureDetected(GESTURE_DRAG))
//        {
//            worldCamera.target = Vector2Add(worldCamera.target, Vector2Scale(GetGestureDragVector(), -1/worldCamera.zoom));
//        }
//
//        if (IsGestureDetected(GESTURE_PINCH_IN))
//        {
//            worldCamera.zoom = Clamp(Vector2Length(GetGesturePinchVector()), 0.1f, 1.0f);
//        }

        UpdatePlayer(dt);
        UpdateAmmo(dt, player.position);
        UpdateUnits();
        UpdateAnimations(dt);

        if (feedbackTimer > 0)
        {
            feedbackTimer -= dt;
            showMessage = true;

            if (feedbackTimer <= 0)
            {
                feedbackTimer = 0;
                showMessage = false;
            }
        }

        if (showMessage) feedbackReveal += 100 * dt;

        if (EnemiesRemaining() == 0)
        {
            if (wave < 5)
            {
                wave++;
                ResetEnemies(wave);
                ResetFriendlies();
                const int count_for_wave = wave * 200;
                feedbackTimer = 0;
                feedbackReveal = 0;
                SetMessage(TextFormat("Wave: %d\n%d Enemies incoming!", wave, count_for_wave));
            }
            else
            {
                if (FriendliesRemaining() < friendlyUnits.capacity/4)
                {
                    endCondition = PYRRHIC_WIN;
                }
                else
                {
                    endCondition = WIN;
                }

                EndGameResetUnits();
                ResetDecals();
                feedbackTimer = 0;
                feedbackReveal = 0;
                finishScreen = 1;
            }
        }

        if (!player.alive)
        {
            SpawnImpactAnimation(player.position);
            EndGameResetUnits();
            endCondition = LOSE;
            finishScreen = 1;
            feedbackTimer = 0;
            feedbackReveal = 0;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            paused = false;
        }
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    if (paused)
    {
        float screenWidth = GetScreenWidth();
        DrawRectangle(0, 0, screenWidth, GetScreenHeight(), BLACK);
        DrawText("Pause", screenWidth/2 - (TextLength("Pause")*8), 100, 40, WHITE);
        masterVolume = GuiSlider((Rectangle){screenWidth/2-100, 200, 200, 25}, "Master Volume", ((void *) 0), masterVolume, 0, 1.0f);
        musicVolume = GuiSlider((Rectangle){screenWidth/2-100, 240, 200, 25}, "Music Volume", ((void *) 0), musicVolume, 0, 1.0f);
        ezAiming = GuiCheckBox((Rectangle){screenWidth/2-100, 275, 25, 25}, "Bigger Direction Indicator", ezAiming);

        DrawText("Controls", 410, 320, 20, WHITE);
        DrawText("A & D | Left & Right - Adjust Angle\nW & S | Up & Down - Adjust Range\nSpace | Left Ctrl | Right Ctrl - Fire\nEsc - Pause/Unpause", 270, 360, 20, WHITE);

        if (GuiButton((Rectangle){screenWidth/2-50, 500, 100, 25}, "Unpause")) paused = false;
        if (GuiButton((Rectangle){screenWidth/2-50, 530, 100, 25}, "Quit"))
        {
            finishScreen = 1;
            endCondition = LOSE;
        }

        SetMasterVolume(masterVolume);
        SetMusicVolume(music, musicVolume);
    }
    else
    {
        // Draw "radar" view
        BeginTextureMode(sideRenderTexture);
            BeginMode2D(mapCamera);
                ClearBackground(RAYWHITE);

                // Draw world
                DrawTextureEx(worldTexture, (Vector2){-3200, -3200}, 0, 1, WHITE);

                // Draw player
                DrawRectangle(player.position.x - 50, player.position.y - 50, 100, 100, BLUE);

                // Draw range
                DrawCircleSector(Vector2SubtractValue(player.position, 50), player.fireRange, 180-MAX_ROTATION, 180+MAX_ROTATION, 0, Fade(MAROON, 0.4f));

                // Draw enemyUnits
                for (int i = 0; i < enemyUnits.capacity; ++i)
                {
                    if (enemyUnits.units[i].active)
                    {
                        DrawRectangle(enemyUnits.units[i].position.x, enemyUnits.units[i].position.y, 100, 100, RED);
                    }
                }

                // Draw Friendlies
                for (int i = 0; i < friendlyUnits.capacity; ++i)
                {
                    if (friendlyUnits.units[i].active)
                    {
                        DrawRectangle(friendlyUnits.units[i].position.x, friendlyUnits.units[i].position.y, 100, 100, BLUE);
                    }
                }

                // Draw player rotation
                int rangeLength = (ezAiming) ? player.fireRange : 1000;
                Vector2 rotVec = Vector2Add(Vector2Scale(RotationToVector(player.rotation), rangeLength), player.position);
                DrawLineEx(player.position, rotVec, 75, BLUE);



                // Draw shells
                for (int i = 0; i < ammo.capacity; ++i)
                {
                    Shell shell = ammo.shells[i];
                    if (shell.active)
                    {
                        DrawRectangle(shell.position.x, shell.position.y, 100, 100, GOLD);
                    }
                }

                // Draw camera borders
                float offset = ((1 - worldCamera.zoom) * 3200);
                DrawRectangleLinesEx((Rectangle){worldCamera.target.x - offset/2, worldCamera.target.y - offset/2, offset, offset}, 50, DARKGRAY);
            EndMode2D();
        EndTextureMode();

        BeginTextureMode(mainRenderTexture);
            BeginMode2D(worldCamera);
                ClearBackground(BLACK);
                DrawTextureEx(worldTexture, (Vector2){-3200, -3200}, 0, 1, WHITE);
                DrawDecals();
                DrawUnits();
                DrawPlayer();
                DrawAnimations();
                DrawAmmo();
            EndMode2D();
        EndTextureMode();

        ClearBackground(RAYWHITE);
        DrawTexturePro(
                mainRenderTexture.texture,
                (Rectangle){0, 0, mainRenderTexture.texture.width, -mainRenderTexture.texture.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0},
                0,
                WHITE);
        DrawGui();
        DrawMessage();
//    DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    UnloadRenderTexture(mainRenderTexture);
    UnloadRenderTexture(sideRenderTexture);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

void Shoot()
{
    if (player.reloadTimer == 0 && player.shotTimer == 0 && ammo.count > 0)
    {
        SetSoundPitch(fxShoot, 1);
        PlaySound(fxShoot);
        StartShootAnimation(player.position, player.rotation);
        FireShot(player.position, player.rotation, player.fireRange);
        shellsFired++;

        player.shotTimer = SHOT_TIME;

        if (ShotsRemaining() == 0)
        {
            player.reloadTimer = RELOAD_TIME;
            ReloadAmmo();
        }
    }
}

void Explode(int shellIndex)
{
    Shell* shell = &ammo.shells[shellIndex];
    float volume = 1 - Vector2Distance(shell->position, worldCamera.target) / 3000;//1 - ammo.shells[shellIndex].range / (MAX_FIRE_RANGE + 200);
    SetSoundVolume(fxImpact, volume);
    PlaySoundMulti(fxImpact);
    SpawnDecal(IMPACT, shell->position);
    SpawnImpactAnimation(shell->position);
    shell->active = false;

    int enemiesHit =    DamageUnitsInsideArea(shell->position, shell->blastRadius, ENEMY_TEAM);
    int friendliesHit = DamageUnitsInsideArea(shell->position, shell->blastRadius, FRIENDLY_TEAM);

    if (friendliesHit > 0)
        SetMessage("Friendly fire!");
    else if (enemiesHit > 5)
		SetMessage("Good hit!");

    enemiesKilled += enemiesHit;
    friendliesKilled += friendliesHit;

    score += (enemiesHit * 100) - (friendliesHit * 200);
}

void SetMessage(const char* message)
{
    if (feedbackTimer <= 0)
    {
	    TextCopy(feedbackMessage, message);
        showMessage = true;
        feedbackTimer = TIME_BETWEEN_FEEDBACK;
    }
}

void DrawMessage()
{
    if (showMessage)
    {
		int startY = 550;
		int endY = GetScreenHeight() - startY - 5;
        DrawRectangleRec    ((Rectangle){5, startY, 640, endY}, WHITE);
        DrawRectangleLinesEx((Rectangle){5, startY, 640, endY}, 3, BLACK);
//        DrawText(feedbackMessage, 15, startY + 15, 30, BLACK);
        DrawText(TextSubtext(feedbackMessage, 0, feedbackReveal/10), 15, startY + 15, 30, BLACK);
    }
}

void DrawGui()
{
    // Background
    DrawRectangle(650, 0, 250, GetScreenHeight(), LIGHTGRAY);

    // Radar
    DrawTexturePro(
        sideRenderTexture.texture,
        (Rectangle){0, 0, -sideRenderTexture.texture.width, sideRenderTexture.texture.height},
        radarRect,
        (Vector2){115, 115},
        180,
        WHITE);

    int yOffset = 240;

    GuiLabel((Rectangle){655, yOffset, 150, 25}, TextFormat("Zoom: %.2f", worldCamera.zoom));
    yOffset += 20;
    worldCamera.zoom = GuiSlider((Rectangle){655, yOffset, 240, 15}, NULL, NULL, worldCamera.zoom, 0.1f, 1.0f);
    yOffset += 30;

    // Range controls
    GuiLabel((Rectangle){655, yOffset, 150, 25}, TextFormat("Range: %.2f", player.fireRange));
    yOffset += 20;
    player.fireRange = GuiSlider((Rectangle){655, yOffset, 240, 25}, NULL, NULL, player.fireRange, MIN_FIRE_RANGE, MAX_FIRE_RANGE);
    yOffset += 30;

    // Rotation controls
    GuiLabel((Rectangle){655, yOffset, 150, 25}, TextFormat("Rotation: %.2f", player.targetRotation));
    yOffset += 20;
    player.targetRotation = GuiSlider((Rectangle){655, yOffset, 240, 25}, NULL, NULL, player.targetRotation, -MAX_ROTATION, MAX_ROTATION);
    yOffset += 30;

    // Player health bar
    GuiLabel((Rectangle){655, yOffset, 150, 25}, "Health:");
    yOffset += 20;
    GuiProgressBar((Rectangle){655, yOffset, 240, 15}, NULL, NULL, player.health, 0, 100);
    yOffset += 40;

    // Reload timer bar
    GuiProgressBar((Rectangle){655, yOffset, 240, 15}, NULL, NULL, RELOAD_TIME - player.reloadTimer, 0, RELOAD_TIME);
    yOffset += 17;

    // Shot timer bar
    GuiProgressBar((Rectangle){655, yOffset, 240, 15}, NULL, NULL, SHOT_TIME - player.shotTimer, 0, SHOT_TIME);
    yOffset += 17;

    // Fire button
    if (GuiButton((Rectangle){655, yOffset, 240, 50}, "Fire"))
    {
        Shoot();
    }
    yOffset += 50;

    GuiLabel((Rectangle){655, yOffset, 240, 25}, TextFormat("Ammo: %d/5", ammo.count));
    yOffset += 20;
    GuiLabel((Rectangle){655, yOffset, 240, 25}, TextFormat("Score: %d", score));
    yOffset += 20;
    GuiLabel((Rectangle){655, yOffset, 240, 25}, TextFormat("Friendlies Remaining: %d", FriendliesRemaining()));
    yOffset += 20;
    GuiLabel((Rectangle){655, yOffset, 240, 25}, TextFormat("Enemies Remaining: %d", EnemiesRemaining()));
    yOffset += 20;
    GuiLabel((Rectangle){655, yOffset, 240, 25}, TextFormat("Wave: %d/5", wave));
}

void DrawSprite(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation)
{
    Rectangle source = (Rectangle){offsetX*64+1, offsetY*64+1, 63, 63}; // Offsetting by 1px for each side because of lines when drawing
    Rectangle dest = (Rectangle){position.x, position.y, 64, 64};
    DrawTexturePro(spriteSheet, source, dest, origin, rotation, WHITE);
}

void DrawSpriteColor(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation, Color color)
{
    Rectangle source = (Rectangle){offsetX*64, offsetY*64, 64, 64};
    Rectangle dest = (Rectangle){position.x, position.y, 64, 64};
    DrawTexturePro(spriteSheet, source, dest, origin, rotation, color);
}

Vector2 RotationToVector(float rotation)
{
    return (Vector2){sinf(rotation * DEG2RAD), -cosf(rotation * DEG2RAD)};
}

bool MouseOverRadar()
{
    return PointInsideRect(GetMousePosition(), (Rectangle){radarRect.x - 115, radarRect.y - 115, radarRect.width, radarRect.height});
}

bool MouseOverWorld()
{
    return PointInsideRect(GetMousePosition(), (Rectangle){0, 0, 600, 600});
}

bool PointInsideRect(Vector2 point, Rectangle rect)
{
    return (point.x >= rect.x) && (point.x <= rect.x + rect.width) &&
            (point.y >= rect.y) && (point.y <= rect.y + rect.height);
}

Vector2 VirtualMouseToWorldPos()
{
    Vector2 mousePos = GetMousePosition();
    return (Vector2)
            {
                    Clamp(((mousePos.x - radarRect.x) / radarRect.width  * 6000), -2600, 2600),
                    Clamp(((mousePos.y - radarRect.y) / radarRect.height * 6000), -2600, 2600)
            };
}

int EnemiesRemaining()
{
    int count = 0;

    for (int i = 0; i < enemyUnits.capacity; ++i)
    {
        if (enemyUnits.units[i].active)
            count++;
    }

    return count;
}

int FriendliesRemaining()
{
    int count = 0;

    for (int i = 0; i < friendlyUnits.capacity; ++i)
    {
        if (friendlyUnits.units[i].active)
        {
            count++;
        }
    }

    return count;
}
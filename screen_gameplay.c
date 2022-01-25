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

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

const float TIME_BETWEEN_FEEDBACK = 5.0f;

float feedbackTimer = 5;
float soundVolume = 1.0f;

Camera2D worldCamera;
Camera2D mapCamera;
RenderTexture mainRenderTexture;
RenderTexture2D sideRenderTexture;
//Shader shader;
Player player;
Ammo ammo;
Units enemyUnits;
Units friendlyUnits;

Vector2 fireTargetPos = (Vector2){0, 0};
Vector2 variance = (Vector2){0, 0};

int guiWidth = 250;

char feedbackMessage[50];

bool showMessage = false;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
void DrawGui();
float Wrap(float, float, float);
Vector2 RotationToVector(float);

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    InitAmmo();
    InitPlayer();
	InitUnits();

	mainRenderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	SetTextureFilter(mainRenderTexture.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

    worldCamera.target = player.position;
    worldCamera.offset = (Vector2){GetScreenWidth()/2 - guiWidth/2, GetScreenHeight()/2};
    worldCamera.rotation = 0;
    worldCamera.zoom = 1;

    sideRenderTexture = LoadRenderTexture(600, 600);
    SetTextureFilter(sideRenderTexture.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

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

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        finishScreen = 1;
    }

    UpdatePlayer(dt);

    UpdateAmmo(dt, player.position);
	UpdateUnits();

    if (feedbackTimer > 0)
    {
        feedbackTimer -= dt;
		showMessage = true;

        if (feedbackTimer < 0)
        {
            feedbackTimer = 0;
            showMessage = false;
        }
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // Draw "radar" view
    BeginTextureMode(sideRenderTexture);
        BeginMode2D(mapCamera);
			ClearBackground(RAYWHITE);
	        // Draw player
	        DrawRectangle(player.position.x - 25, player.position.y - 25, 50, 50, BLUE);
	        // Draw range
	        DrawCircle(player.position.x, player.position.y, player.fireRange, ColorAlpha(RED, 0.25f));
	        // Draw player rotation
	        Vector2 rotVec = Vector2Add(Vector2Scale(RotationToVector(player.rotation), 250), player.position);
	        DrawLineEx(player.position, rotVec, 25, BLUE);
	        // Draw shells
	        for (int i = 0; i < ammo.capacity; ++i)
	        {
	            Shell shell = ammo.shells[i];
	            if (shell.active)
	            {
	                DrawRectangle(shell.position.x, shell.position.y, 50, 50, BLACK);
	            }
	        }
	        // Draw enemyUnits
	        for (int i = 0; i < enemyUnits.capacity; ++i)
	        {
	            if (enemyUnits.units[i].active)
	            {
	                DrawRectangle(enemyUnits.units[i].position.x, enemyUnits.units[i].position.y, 50, 50, RED);
	            }
	        }
        EndMode2D();
    EndTextureMode();

	BeginTextureMode(mainRenderTexture);
		BeginMode2D(worldCamera);
			ClearBackground(RAYWHITE);
			DrawAmmo();
			DrawPlayer();
	DrawUnits();
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
    if (ammo.count > 0)
    {
        variance = (Vector2){(float)GetRandomValue(-5, 5)/100, (float)GetRandomValue(-5, 5)/100};

        SetSoundPitch(fxShoot, 1);
        PlaySound(fxShoot);

        ammo.shells[ammo.shellIterator].rotation = player.rotation;
        ammo.shells[ammo.shellIterator].position = player.position;
        ammo.shells[ammo.shellIterator].active = true;
        ammo.shells[ammo.shellIterator].range = player.fireRange - (float)GetRandomValue(-100, 100);
        ammo.shells[ammo.shellIterator].velocity = RotationToVector(ammo.shells[ammo.shellIterator].rotation);
        ammo.shells[ammo.shellIterator].velocity = Vector2Add(ammo.shells[ammo.shellIterator].velocity, variance);

        ammo.shellIterator++;
        ammo.count--;
    }
}

void Explode(int shellIndex)
{
    float volume = 1 - ammo.shells[shellIndex].range / (MAX_FIRE_RANGE + 200);
    SetSoundVolume(fxImpact, volume);
    PlaySoundMulti(fxImpact);
    ammo.shells[shellIndex].active = false;

    int enemiesHit = DamageUnitsInsideArea(ammo.shells[shellIndex].position, ammo.shells[shellIndex].blastRadius, ENEMY_TEAM);

	if (enemiesHit > 0)
	{
		SetMessage("Good hits!");
	}
}

void Reload()
{
    ammo.count = ammo.capacity;
    ammo.shellIterator = 0;
}

void SetMessage(char* message)
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
		int startY = 500;
		int endY = GetScreenHeight() - startY - 5;
        DrawRectangleRec    ((Rectangle){5, startY, 640, endY}, WHITE);
        DrawRectangleLinesEx((Rectangle){5, startY, 640, endY}, 3, BLACK);
        DrawText(feedbackMessage, 15, startY + 15, 30, BLACK);
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
        (Rectangle){776, 117, 230, 230},
        (Vector2){115, 115},
        180,
        WHITE);

    // Rotation controls
    if (GuiButton((Rectangle){670, 250, 25, 25}, "<<"))
    {
        ChangeRotation(-60);
    }
    if (GuiButton((Rectangle){700, 250, 25, 25}, "<"))
    {
        ChangeRotation(-10);
    }
    DrawText(TextFormat("%d", (int)player.targetRotation), 740, 253, 20, BLACK);
    if (GuiButton((Rectangle){790, 250, 25, 25}, ">"))
    {
        ChangeRotation(10);
    }
    if (GuiButton((Rectangle){820, 250, 25, 25}, ">>"))
    {
        ChangeRotation(60);
    }

    // Range controls
    player.fireRange = GuiSlider((Rectangle){670, 300, 175, 25}, "Distance", TextFormat("%.2f", player.fireRange), player.fireRange, MIN_FIRE_RANGE, MAX_FIRE_RANGE);

    // Fire button
    if (GuiButton((Rectangle){670, 350, 175, 25}, "Fire"))
    {
        Shoot();
    }

    // Reload button
    if (GuiButton((Rectangle){670, 400, 175, 25}, "Reload"))
    {
        Reload();
    }

    // Ammo count
    GuiLabel((Rectangle){670, 450, 175, 25}, TextFormat("%d / %d", ammo.count, ammo.capacity));
}

void DrawSprite(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation)
{
    Rectangle source = (Rectangle){offsetX*64, offsetY*64, 64, 64};
    Rectangle dest = (Rectangle){position.x, position.y, 64, 64};
    DrawTexturePro(spriteSheet, source, dest, origin, rotation, WHITE);
}

Vector2 RotationToVector(float rotation){
    return (Vector2){sinf(rotation * DEG2RAD), -cosf(rotation * DEG2RAD)};
}
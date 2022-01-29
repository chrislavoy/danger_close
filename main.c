/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "extras/raygui.h"
#include "screens.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define DEBUG_MODE false

GameScreen currentScreen = 0;
Music music = { 0 };
Sound fxShoot = { 0 };
Sound fxImpact = { 0 };
Sound fxUnitShoot = { 0 };
Sound fxRadio = { 0 };
Sound fxReload = { 0 };
Texture2D spriteSheet = { 0 };
Texture2D worldTexture = { 0 };
Texture2D corpseTexture = { 0 };
Texture2D smokeTexture = { 0 };
EndCondition endCondition = { 0 };

// Global Variables Definition
int screenWidth = 900;
int screenHeight = 675;
float musicVolume = 0.1f;
float masterVolume = 0.5f;
int score = 0;
int enemiesKilled = 0;
int friendliesKilled = 0;
int shellsFired = 0;
int bestShot = 0;
bool ezAiming = 0;

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static int transToScreen = -1;

// Local Functions Declaration
static void ChangeToScreen(int screen);     // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

// Module Functions Declaration
static void UpdateDrawFrame(void);     // Update and Draw one frame

// Main Enry Point
int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);

    // Initialization
    InitWindow(screenWidth, screenHeight, "Danger Close");
    InitAudioDevice();

    SetExitKey(KEY_BACKSPACE);

    music = LoadMusicStream("assets/backgroundMusic.mp3");
    fxShoot = LoadSound("assets/gun.wav");
    fxImpact = LoadSound("assets/impact.wav");
    fxUnitShoot = LoadSound("assets/unitShoot.mp3");
    fxRadio = LoadSound("assets/radio.mp3");
    fxReload = LoadSound("assets/reload.mp3");

    spriteSheet = LoadTexture("assets/towerDefense_tilesheet.png");
//    SetTextureFilter(spriteSheet, TEXTURE_FILTER_ANISOTROPIC_16X);

	worldTexture = LoadTexture("assets/DangerClose.png");
//	SetTextureFilter(worldTexture, TEXTURE_FILTER_ANISOTROPIC_16X);

    corpseTexture = LoadTexture("assets/dirt_01.png");
//    SetTextureFilter(corpseTexture, TEXTURE_FILTER_ANISOTROPIC_16X);

    smokeTexture = LoadTexture("assets/smoke_07.png");
//    SetTextureFilter(smokeTexture, TEXTURE_FILTER_ANISOTROPIC_16X);

    SetMusicVolume(music, musicVolume);
    PlayMusicStream(music);

    currentScreen = ENDING;

    switch (currentScreen)
    {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif

    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Unload global data loaded
    UnloadMusicStream(music);
    UnloadSound(fxShoot);
    UnloadSound(fxImpact);
    UnloadSound(fxUnitShoot);
    UnloadSound(fxRadio);
    UnloadSound(fxReload);
	UnloadTexture(spriteSheet);
	UnloadTexture(worldTexture);
    UnloadTexture(corpseTexture);
    UnloadTexture(smokeTexture);

    // De-Initialization
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(int screen)
{
    // Unload current screen
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen)
    {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(int screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
                case LOGO: UnloadLogoScreen(); break;
                case TITLE: UnloadTitleScreen(); break;
                case OPTIONS: UnloadOptionsScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                case ENDING: UnloadEndingScreen(); break;
                default: break;
            }

            // Load next screen
            switch (transToScreen)
            {
                case LOGO: InitLogoScreen(); break;
                case TITLE: InitTitleScreen(); break;
                case OPTIONS: InitOptionsScreen(); break;
                case GAMEPLAY: InitGameplayScreen(); break;
                case ENDING: InitEndingScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = -1;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                UpdateLogoScreen();

                if (FinishLogoScreen()) TransitionToScreen(TITLE);

            } break;
            case TITLE:
            {
                UpdateTitleScreen();

                if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
                else if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);

            } break;
            case OPTIONS:
            {
                UpdateOptionsScreen();

                if (FinishOptionsScreen()) TransitionToScreen(TITLE);

            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen();

                if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
                //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

            } break;
            case ENDING:
            {
                UpdateEndingScreen();

                if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);
                else if (FinishEndingScreen() == 2) TransitionToScreen(GAMEPLAY);

            } break;
            default: break;
        }
    }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch(currentScreen)
    {
        case LOGO: DrawLogoScreen(); break;
        case TITLE: DrawTitleScreen(); break;
        case OPTIONS: DrawOptionsScreen(); break;
        case GAMEPLAY: DrawGameplayScreen(); break;
        case ENDING: DrawEndingScreen(); break;
        default: break;
    }

    // Draw full screen rectangle in front of everything
    if (onTransition) DrawTransition();

    if (DEBUG_MODE) DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
//
// Created by chris on 1/23/2022.
//

#include "player.h"
#include "raymath.h"

// Also declared in screens.h, implemented in screen_gameplay.c
void Shoot(void);
void DrawSprite(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation);

// Private function declaration
void UpdateRotation(float);
float Wrap(float, float, float);

extern Sound fxReload;

// Private variable declaration
bool CCW = false;

void InitPlayer(void)
{
    player.origin = (Vector2){32, 38};
    player.position = (Vector2){0, 2750};
    player.rectangle = (Rectangle){player.position.x, player.position.y, 64, 64};
    player.rotation = 0;
    player.targetRotation = 0;
    player.color = WHITE;
    player.fireRange = MAX_FIRE_RANGE;
    player.reloadTimer = 0;
    player.shotTimer = 0;
    player.health = 100.0f;
    player.alive = true;
}

void UpdatePlayer(float dt)
{
//    if (IsKeyDown(KEY_LEFT))
//    {
//        ChangeRotation(-TURN_RATE * dt);
//    }
//
//    if (IsKeyDown(KEY_RIGHT))
//    {
//        ChangeRotation(TURN_RATE * dt);
//    }

    UpdateRotation(dt);

//    if (IsKeyDown(KEY_UP))
//    {
//        ChangeRange(200 * dt);
//    }
//
//    if (IsKeyDown(KEY_DOWN))
//    {
//        ChangeRange(-200 * dt);
//    }
//
//    if (IsKeyPressed(KEY_SPACE))
//    {
//        Shoot();
//    }

    if (player.reloadTimer > 0) player.reloadTimer -= dt;
    if (player.reloadTimer < 0)
    {
        player.reloadTimer = 0;
        StopSound(fxReload);
    }

    if (player.shotTimer > 0) player.shotTimer -= dt;
    if (player.shotTimer < 0) player.shotTimer = 0;

    if (player.health <= 0)
        player.alive = false;

}

void DrawPlayer()
{
    if (player.alive)
    {
        // Draw base
        DrawSprite(20, 7, Vector2Add(player.position, (Vector2){0, 6}), player.origin, 0);
        // Draw turret
        DrawSprite(19, 10, player.position, player.origin, player.rotation);
    }
}

//void ChangeRotation(float amount)
//{
//    player.targetRotation += amount;
//
//    player.targetRotation = Wrap(player.targetRotation, 0, 360);
//
//    // I don't love this, but this sets the rotation direction
//    // based on how far the player needs to rotate
//    float delta = player.rotation - player.targetRotation;
//    float absDelta = fabsf(delta);
//
//    if (delta < 0) CCW = false;
//    else CCW = true;
//    if (absDelta > 180) CCW = !CCW;
//}

void ChangeRange(float amount)
{
    player.fireRange += amount;

    player.fireRange = Clamp(player.fireRange, MIN_FIRE_RANGE, MAX_FIRE_RANGE);
}

void UpdateRotation(float dt)
{
//    player.rotation -= 270;
//    player.targetRotation -= 270;

    float delta = player.rotation - player.targetRotation;

    if (fabsf(delta) > 0.5f)
    {
        if (delta < 0) CCW = false;
        else CCW = true;
//        if (absDelta > 180) CCW = !CCW;

        if (CCW)
            player.rotation -= TURN_RATE * dt;
        else
            player.rotation += TURN_RATE * dt;

//        player.rotation = Wrap(player.rotation, 0, 360);
    }
    else
    {
        player.rotation = player.targetRotation;
//        player.rotation = Wrap(player.rotation, 0, 360);
    }
}

float Wrap(float value, float min, float max)
{
    float result = (value < min) ? value + max : value;
    if (result > max) result -= max;
    if (result == max) result = min;
    return result;
}
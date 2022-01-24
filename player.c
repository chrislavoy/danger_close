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

// Private variable declaration
bool CCW = false;

void InitPlayer(void)
{
    player.origin = (Vector2){32, 38};
    player.position = (Vector2){ 300, GetScreenHeight()/2 };
    player.rectangle = (Rectangle){player.position.x, player.position.y, 64, 64};
    player.rotation = 180;
    player.targetRotation = 180;
    player.color = WHITE;
    player.fireRange = MAX_FIRE_RANGE;
}

void UpdatePlayer(float dt)
{
    if (IsKeyDown(KEY_LEFT))
    {
        ChangeRotation(-TURN_RATE * dt);
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        ChangeRotation(TURN_RATE * dt);
    }

    UpdateRotation(dt);

    if (IsKeyDown(KEY_UP))
    {
        ChangeRange(200 * dt);
    }

    if (IsKeyDown(KEY_DOWN))
    {
        ChangeRange(-200 * dt);
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        Shoot();
    }
}

void DrawPlayer()
{
    // Draw base
    DrawSprite(20, 7, Vector2Add(player.position, (Vector2){0, 6}), player.origin, 0);
    // Draw turret
    DrawSprite(19, 10, player.position, player.origin, player.rotation - 180);
}

void ChangeRotation(float amount)
{
    player.targetRotation += amount;

    player.targetRotation = Wrap(player.targetRotation, 0, 360);

    // I don't love this, but this sets the rotation direction
    // based on how far the player needs to rotate
    float delta = player.rotation - player.targetRotation;
    float absDelta = fabsf(delta);

    if (delta < 0) CCW = false;
    else CCW = true;
    if (absDelta > 180) CCW = !CCW;
}

void ChangeRange(float amount)
{
    player.fireRange += amount;

    player.fireRange = Clamp(player.fireRange, MIN_FIRE_RANGE, MAX_FIRE_RANGE);
}

void UpdateRotation(float dt)
{
    float delta = fabsf(player.rotation - player.targetRotation);

    if (delta > 0.5f)
    {
        if (CCW)
            player.rotation -= TURN_RATE * dt;
        else
            player.rotation += TURN_RATE * dt;

        player.rotation = Wrap(player.rotation, 0, 360);
    }
    else
    {
        player.rotation = player.targetRotation;
        player.rotation = Wrap(player.rotation, 0, 360);
    }
}

float Wrap(float value, float min, float max)
{
    float result = (value < min) ? value + max : value;
    if (result > max) result -= max;
    if (result == max) result = min;
    return result;
}
//
// Created by chris on 1/23/2022.
//

#include "player.h"

void Shoot(void); // Declared in screens.h, implemented in screen_gameplay.c

void InitPlayer(void)
{
    player.origin = (Vector2){32, 38};
    player.position = (Vector2){ 300, GetScreenHeight()/2 };
    player.rectangle = (Rectangle){player.position.x, player.position.y, 64, 64};
    player.rotation = 180;
    player.color = WHITE;
    player.fireRange = MAX_FIRE_RANGE;
}

void UpdatePlayer(float dt)
{
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
        player.fireRange += 100 * dt;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        player.fireRange -= 100 * dt;
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

    if (player.fireRange > MAX_FIRE_RANGE)
    {
        player.fireRange = MAX_FIRE_RANGE;
    }
    else if (player.fireRange < MIN_FIRE_RANGE)
    {
        player.fireRange = MIN_FIRE_RANGE;
    }
}

void DrawPlayer(Texture2D* spriteSheet)
{
    // Draw base
    DrawTexturePro(*spriteSheet, (Rectangle){20*64, 7*64, 64, 64}, (Rectangle){player.position.x, player.position.y + 6, 64, 64}, player.origin, 0, player.color);
    // Draw turret
    DrawTexturePro(*spriteSheet, (Rectangle){19*64, 10*64, 64, 64}, player.rectangle, player.origin, player.rotation - 180, player.color);
}
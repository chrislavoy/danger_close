//
// Created by chris on 1/23/2022.
//

#include "ammo.h"
#include "raymath.h"

// Also declared in screens.h, implemented in screen_gameplay.c
void Explode(int);
void DrawSprite(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation);

void InitAmmo(void)
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
    }
}

void UpdateAmmo(float dt, Vector2 playerPosition)
{
    for (int i = 0; i < ammo.capacity; ++i)
    {
        if (ammo.shells[i].active)
        {
            if (Vector2Distance(playerPosition, ammo.shells[i].position) > ammo.shells[i].range)
            {
                Explode(i);
            }
            else
            {
                ammo.shells[i].position = Vector2Add(ammo.shells[i].position, Vector2Scale(ammo.shells[i].velocity, 400 * dt));
                ammo.shells[i].rectangle = (Rectangle){ammo.shells[i].position.x, ammo.shells[i].position.y, 64, 64};
            }
        }
    }
}

void DrawAmmo()
{
    for (int i = 0; i < ammo.capacity; ++i)
    {
        Shell shell = ammo.shells[i];
        if (shell.active)
        {
            DrawSprite(19, 11, shell.position, shell.origin, shell.rotation);
        }
    }
}
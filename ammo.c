//
// Created by chris on 1/23/2022.
//

#include "ammo.h"
#include "raymath.h"

// Also declared in screens.h, implemented in screen_gameplay.c
void Explode(int);
void DrawSprite(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation);
Vector2 RotationToVector(float);

void InitAmmo(void)
{
    ammo.capacity = MAX_SHELLS;
    ammo.count = 5;

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
        ammo.shells[i].blastRadius = 200;
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
                ammo.shells[i].position = Vector2Add(ammo.shells[i].position, Vector2Scale(ammo.shells[i].velocity, AMMO_SPEED * dt));
                ammo.shells[i].rectangle = (Rectangle){ammo.shells[i].position.x, ammo.shells[i].position.y, 64, 64};
            }
        }
    }
}

void FireShot(Vector2 playerPosition, float playerRotation, float playerFireRange)
{
    ammo.count--;

    ammo.shells[ammo.shellIterator].rotation = playerRotation;
    ammo.shells[ammo.shellIterator].position = playerPosition;
    ammo.shells[ammo.shellIterator].active = true;
    ammo.shells[ammo.shellIterator].range = playerFireRange - (float)GetRandomValue(-100, 100);
    ammo.shells[ammo.shellIterator].velocity = RotationToVector(ammo.shells[ammo.shellIterator].rotation);
    Vector2 variance = (Vector2){(float)GetRandomValue(-5, 5)/100, (float)GetRandomValue(-5, 5)/100};
    ammo.shells[ammo.shellIterator].velocity = Vector2Add(ammo.shells[ammo.shellIterator].velocity, variance);

    ammo.shellIterator++;
    if (ammo.shellIterator == ammo.capacity)
        ammo.shellIterator = 0;
}

void ReloadAmmo()
{
    ammo.count = 5;
//    ammo.shellIterator = 0;
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

int ShotsRemaining() {
    return ammo.count;
}

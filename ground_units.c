//
// Created by chris on 1/23/2022.
//

#include "ground_units.h"
#include "raymath.h"

// Also declared in screens.h, implemented in screen_gameplay.c
void DrawSprite(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation);

void InitEnemies(void) {
    enemies.capacity = MAX_ENEMIES;

    for (int i = 0; i < enemies.capacity; ++i)
    {
        enemies.units[i].origin = (Vector2){32, 32};
        enemies.units[i].position = (Vector2){GetRandomValue(-2999, 2999), GetRandomValue(-3000, -2000)};
        enemies.units[i].rectangle = (Rectangle) {0, 0, 64, 64};
        enemies.units[i].rotation = 0;
        enemies.units[i].color = WHITE;
        enemies.units[i].movementSpeed = 1;
        enemies.units[i].targetPos = Vector2Add(enemies.units[i].position, (Vector2){0, 1000});
        enemies.units[i].active = true;
    }
}

void UpdateEnemies()
{
    for (int i = 0; i < enemies.capacity; ++i)
    {
        if (enemies.units[i].active)
        {
            if (Vector2Distance(enemies.units[i].position, enemies.units[i].targetPos) > 400)
            {
                enemies.units[i].position = Vector2MoveTowards(enemies.units[i].position, enemies.units[i].targetPos, enemies.units[i].movementSpeed);
                enemies.units[i].rotation = Vector2Angle(enemies.units[i].position, enemies.units[i].targetPos);
                enemies.units[i].rectangle = (Rectangle){enemies.units[i].position.x, enemies.units[i].position.y, 64, 64};
            }
        }
    }
}

void DrawEnemies()
{
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies.units[i].active)
        {
            DrawSprite(16, 10, enemies.units[i].position, enemies.units[i].origin, enemies.units[i].rotation);
        }
    }
}
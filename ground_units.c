//
// Created by chris on 1/23/2022.
//

#include "ground_units.h"
#include "raymath.h"

void InitEnemies(void) {
    enemies.capacity = MAX_ENEMIES;

    for (int i = 0; i < enemies.capacity; ++i)
    {
        enemies.units[i].origin = (Vector2){32, 32};
        enemies.units[i].position = Vector2Zero();
        enemies.units[i].rectangle = (Rectangle) {0, 0, 64, 64};
        enemies.units[i].rotation = 0;
        enemies.units[i].color = WHITE;
        enemies.units[i].movementSpeed = 1;
        enemies.units[i].targetPos = Vector2Zero();
        enemies.units[i].active = false;
    }

    enemies.units[0].position = (Vector2) {10, 10};
    enemies.units[0].targetPos = (Vector2) {800, 400};
    enemies.units[0].active = true;
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

void DrawEnemies(Texture2D* spriteSheet)
{
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies.units[i].active)
        {
            DrawTexturePro(*spriteSheet, (Rectangle){16*64, 10*64, 64, 64}, enemies.units[i].rectangle, enemies.units[i].origin, enemies.units[i].rotation - 180, enemies.units[i].color);
        }
    }
}
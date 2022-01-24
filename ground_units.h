//
// Created by chris on 1/23/2022.
//

#ifndef DANGER_CLOSE_GROUND_UNITS_H
#define DANGER_CLOSE_GROUND_UNITS_H

#include "raylib.h"

#define MAX_ENEMIES 100

typedef struct Enemy {
    Vector2 origin;
    Vector2 position;
    Rectangle rectangle;
    float rotation;
    Color color;
    bool active;
    Vector2 targetPos;
    float movementSpeed;
} Enemy;

typedef struct Enemies {
    int capacity;
    Enemy units[MAX_ENEMIES];
} Enemies;

extern Enemies enemies;

void InitEnemies(void);
void UpdateEnemies();
void DrawEnemies();

#endif //DANGER_CLOSE_GROUND_UNITS_H

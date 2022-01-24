//
// Created by chris on 1/23/2022.
//

#ifndef DANGER_CLOSE_PLAYER_H
#define DANGER_CLOSE_PLAYER_H

#include "raylib.h"

#define TURN_RATE 30.0f
#define MAX_FIRE_RANGE 3000
#define MIN_FIRE_RANGE 200

typedef struct Player {
    Vector2 origin;
    Vector2 position;
    Rectangle rectangle;
    float rotation;
    float targetRotation;
    Color color;
    float fireRange;
} Player;

extern Player player;

void InitPlayer(void);
void UpdatePlayer(float);
void DrawPlayer();
void ChangeRotation(float);
void ChangeRange(float);

#endif //DANGER_CLOSE_PLAYER_H

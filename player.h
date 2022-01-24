//
// Created by chris on 1/23/2022.
//

#ifndef DANGER_CLOSE_PLAYER_H
#define DANGER_CLOSE_PLAYER_H

#include "raylib.h"

#define TURN_RATE 100.0f
#define MAX_FIRE_RANGE 3000
#define MIN_FIRE_RANGE 200

typedef struct Player {
    Vector2 origin;
    Vector2 position;
    Rectangle rectangle;
    float rotation;
    Color color;
    float fireRange;
} Player;

extern Player player;

void InitPlayer(void);
void UpdatePlayer(float dt);
void DrawPlayer(Texture2D*);

#endif //DANGER_CLOSE_PLAYER_H

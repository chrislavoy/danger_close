//
// Created by chris on 1/23/2022.
//

#ifndef DANGER_CLOSE_AMMO_H
#define DANGER_CLOSE_AMMO_H

#include "raylib.h"

#define MAX_SHELLS 20
#define AMMO_SPEED 500

typedef struct Shell {
    Vector2 origin;
    Vector2 position;
    Rectangle rectangle;
    float rotation;
    Color color;
    bool active;
    Vector2 velocity;
    float range;
    int blastRadius;
} Shell;

typedef struct Ammo {
    int count;
    int capacity;
    int shellIterator;
    Shell shells[MAX_SHELLS];
} Ammo;

extern Ammo ammo;

void InitAmmo(void);
void UpdateAmmo(float, Vector2);
void FireShot(Vector2, float, float);
int ShotsRemaining();
void ReloadAmmo();
void DrawAmmo();

#endif //DANGER_CLOSE_AMMO_H

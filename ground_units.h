//
// Created by chris on 1/23/2022.
//

#ifndef DANGER_CLOSE_GROUND_UNITS_H
#define DANGER_CLOSE_GROUND_UNITS_H

#include "raylib.h"

#define MAX_ENEMIES 100
#define MAX_FRIENDLIES 100
#define FRIENDLY_TEAM 0
#define ENEMY_TEAM 1
#define HIT_CHANCE 0.15f
#define DETECTION_RANGE 400
#define SHOT_TIMER_MAX 3.0f

typedef struct Unit {
    Vector2 origin;
    Vector2 position;
    Rectangle rectangle;
    float rotation;
    Color color;
    bool active;
    Vector2 moveTo;
    float movementSpeed;
	short team;
    int target;
    float shotTimer;
} Unit;

typedef struct Units {
    int capacity;
    Unit units[MAX_ENEMIES];
} Units;

extern Units enemyUnits;
extern Units friendlyUnits;
extern Sound fxUnitShoot;

void InitUnits(void);
void UpdateUnits();
void DrawUnits();
int DamageUnitsInsideArea(Vector2, float, short);

#endif //DANGER_CLOSE_GROUND_UNITS_H

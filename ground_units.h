//
// Created by chris on 1/23/2022.
//

#ifndef DANGER_CLOSE_GROUND_UNITS_H
#define DANGER_CLOSE_GROUND_UNITS_H

#include "raylib.h"
#include "player.h"

#define MAX_ENEMIES 1000
#define MAX_FRIENDLIES 300
#define FRIENDLY_TEAM 0
#define ENEMY_TEAM 1
#define FRIENDLY_HIT_CHANCE 0.1f
#define ENEMY_HIT_CHANCE 0.25f
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
extern Player player;

void InitUnits(int);
void ResetEnemies(int);
void ResetFriendlies(void);
void UpdateUnits();
void DrawUnits();
int DamageUnitsInsideArea(Vector2, float, short);
void EndGameResetUnits();

#endif //DANGER_CLOSE_GROUND_UNITS_H

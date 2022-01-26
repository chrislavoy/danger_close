//
// Created by chris on 1/23/2022.
//

#include "ground_units.h"
#include "raymath.h"

void InitFriendlies(void);
void InitEnemies(void);
void AttackUnit(int, int);
int EnemyUnitInRange(int, Vector2, float);

// Also declared in screens.h, implemented in screen_gameplay.c
void DrawSprite(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation);

void InitUnits(void)
{
    InitEnemies();

	InitFriendlies();
}

void InitEnemies()
{
	enemyUnits.capacity = MAX_ENEMIES;

	for (int i = 0; i < enemyUnits.capacity; ++i)
	{
        Unit* unit = &enemyUnits.units[i];
        unit->origin = (Vector2){32, 32};
        unit->position = (Vector2){GetRandomValue(-2999, 2999), GetRandomValue(-3000, -2000)};
        unit->rectangle = (Rectangle) {0, 0, 64, 64};
        unit->rotation = 0;
        unit->color = WHITE;
        unit->movementSpeed = 3;
        unit->moveTo = (Vector2){unit->position.x, 3000};//Vector2Add(enemyUnits.units[i].position, (Vector2){0, 1000});
        unit->active = true;
        unit->team = ENEMY_TEAM;
        unit->target = -1;
        unit->shotTimer = 0;
	}
}

void InitFriendlies()
{
	friendlyUnits.capacity = MAX_FRIENDLIES;

	for (int i = 0; i < friendlyUnits.capacity; ++i)
	{
        Unit* unit = &friendlyUnits.units[i];
        unit->origin = (Vector2){32, 32};
        unit->position = (Vector2){GetRandomValue(-2999, 2999), GetRandomValue(3000, 2000)};
        unit->rectangle = (Rectangle) {0, 0, 64, 64};
        unit->rotation = 0;
        unit->color = WHITE;
        unit->movementSpeed = 3;
        unit->moveTo = (Vector2){unit->position.x, -3000};//Vector2Add(friendlyUnits.units[i].position, (Vector2){0, -1000});
        unit->active = true;
        unit->team = FRIENDLY_TEAM;
        unit->target = -1;
        unit->shotTimer = 0;
	}
}

void UpdateUnits()
{
    for (int i = 0; i < enemyUnits.capacity; ++i)
    {
        Unit* unit = &enemyUnits.units[i];
        if (unit->active)
        {
            if (unit->shotTimer > 0)
            {
                unit->shotTimer -= GetFrameTime();
                if (unit->shotTimer < 0)
                {
                    unit->shotTimer = 0;
                }
            }

            if (unit->target == -1)
            {
                int index = EnemyUnitInRange(FRIENDLY_TEAM, unit->position, DETECTION_RANGE);
                if (index > -1)
                {
                    unit->target = index;
                }

                if (Vector2Distance(unit->position, unit->moveTo) > 400)
                {
                    unit->position = Vector2MoveTowards(unit->position, unit->moveTo, unit->movementSpeed);
                    unit->rotation = Vector2Angle(unit->position, unit->moveTo);
                    unit->rectangle = (Rectangle){unit->position.x, unit->position.y, 64, 64};
                }
            }
            else
            {
                if (friendlyUnits.units[unit->target].active)
                {
                    if (unit->shotTimer == 0)
                    {
                        AttackUnit(FRIENDLY_TEAM, unit->target);
                        unit->shotTimer = SHOT_TIMER_MAX;
                    }
                }
                else
                {
                    unit->target = -1;
                }
            }
        }
    }

	for (int i = 0; i < friendlyUnits.capacity; ++i)
	{
        Unit* unit = &friendlyUnits.units[i];
		if (unit->active)
		{
            if (unit->shotTimer > 0)
            {
                unit->shotTimer -= GetFrameTime();
                if (unit->shotTimer < 0)
                {
                    unit->shotTimer = 0;
                }
            }

            if (unit->target == -1)
            {

                int index = EnemyUnitInRange(ENEMY_TEAM, unit->position, DETECTION_RANGE);
                if (index > -1)
                {
                    unit->target = index;
                }

                if (Vector2Distance(unit->position, unit->moveTo) > 400)
                {
                    unit->position = Vector2MoveTowards(unit->position, unit->moveTo, unit->movementSpeed);
                    unit->rotation = Vector2Angle(unit->position, unit->moveTo);
                    unit->rectangle = (Rectangle){unit->position.x, unit->position.y, 64, 64};
                }
            }
            else
            {
                if (enemyUnits.units[unit->target].active)
                {
                    if (unit->shotTimer == 0)
                    {
                        AttackUnit(ENEMY_TEAM, unit->target);
                        unit->shotTimer = SHOT_TIMER_MAX;
                    }
                }
                else
                {
                    unit->target = -1;
                }
            }
		}
	}
}

void DrawUnits()
{
    for (int i = 0; i < enemyUnits.capacity; ++i)
    {
        if (enemyUnits.units[i].active)
        {
            DrawSprite(16, 10, enemyUnits.units[i].position, enemyUnits.units[i].origin, enemyUnits.units[i].rotation);
        }
    }

	for (int i = 0; i < friendlyUnits.capacity; ++i)
	{
		if (friendlyUnits.units[i].active)
		{
			DrawSprite(15, 10, friendlyUnits.units[i].position, friendlyUnits.units[i].origin, friendlyUnits.units[i].rotation);
		}
	}
}

int DamageUnitsInsideArea(Vector2 position, float radius, short team)
{
	int hits = 0;
	Units* unitList = (team == FRIENDLY_TEAM) ? &friendlyUnits : &enemyUnits;

	for (int i = 0; i < unitList->capacity; ++i)
	{
		if (unitList->units[i].active)
		{
			if (CheckCollisionPointCircle(position, unitList->units[i].position, radius))
			{
				unitList->units[i].active = false;
				hits++;
			}
		}
	}

	return hits;
}

void AttackUnit(int team, int index)
{
    SetSoundVolume(fxUnitShoot, 0.25f);
    PlaySoundMulti(fxUnitShoot);
    Units* unitList = (team == FRIENDLY_TEAM) ? &friendlyUnits : &enemyUnits;
    if((float)GetRandomValue(0, 100) / 100.0f > (1.0f - HIT_CHANCE))
    {
        unitList->units[index].active = false;
    }
}

int EnemyUnitInRange(int team, Vector2 position, float range)
{
    Units* unitList = (team == FRIENDLY_TEAM) ? &friendlyUnits : &enemyUnits;
    for (int i = 0; i < unitList->capacity; ++i)
    {
        if (unitList->units[i].active && Vector2Distance(unitList->units[i].position, position) < range)
        {
            return i;
        }
    }

    return -1;
}
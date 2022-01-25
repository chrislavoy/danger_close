//
// Created by chris on 1/23/2022.
//

#include "ground_units.h"
#include "raymath.h"

void InitFriendlies(void);
void InitEnemies(void);

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
		enemyUnits.units[i].origin = (Vector2){32, 32};
		enemyUnits.units[i].position = (Vector2){GetRandomValue(-2999, 2999), GetRandomValue(-3000, -2000)};
		enemyUnits.units[i].rectangle = (Rectangle) {0, 0, 64, 64};
		enemyUnits.units[i].rotation = 0;
		enemyUnits.units[i].color = WHITE;
		enemyUnits.units[i].movementSpeed = 1;
		enemyUnits.units[i].targetPos = Vector2Add(enemyUnits.units[i].position, (Vector2){0, 1000});
		enemyUnits.units[i].active = true;
		enemyUnits.units[i].team = ENEMY_TEAM;
	}
}

void InitFriendlies()
{
	friendlyUnits.capacity = MAX_FRIENDLIES;

	for (int i = 0; i < friendlyUnits.capacity; ++i)
	{
		friendlyUnits.units[i].origin = (Vector2){32, 32};
		friendlyUnits.units[i].position = (Vector2){GetRandomValue(-2999, 2999), GetRandomValue(3000, 2000)};
		friendlyUnits.units[i].rectangle = (Rectangle) {0, 0, 64, 64};
		friendlyUnits.units[i].rotation = 0;
		friendlyUnits.units[i].color = WHITE;
		friendlyUnits.units[i].movementSpeed = 1;
		friendlyUnits.units[i].targetPos = Vector2Add(friendlyUnits.units[i].position, (Vector2){0, 1000});
		friendlyUnits.units[i].active = true;
		friendlyUnits.units[i].team = FRIENDLY_TEAM;
	}
}

void UpdateUnits()
{
    for (int i = 0; i < enemyUnits.capacity; ++i)
    {
        if (enemyUnits.units[i].active)
        {
            if (Vector2Distance(enemyUnits.units[i].position, enemyUnits.units[i].targetPos) > 400)
            {
	            enemyUnits.units[i].position = Vector2MoveTowards(enemyUnits.units[i].position, enemyUnits.units[i].targetPos, enemyUnits.units[i].movementSpeed);
	            enemyUnits.units[i].rotation = Vector2Angle(enemyUnits.units[i].position, enemyUnits.units[i].targetPos);
	            enemyUnits.units[i].rectangle = (Rectangle){enemyUnits.units[i].position.x, enemyUnits.units[i].position.y, 64, 64};
            }
        }
    }

	for (int i = 0; i < friendlyUnits.capacity; ++i)
	{
		if (friendlyUnits.units[i].active)
		{
			if (Vector2Distance(friendlyUnits.units[i].position, friendlyUnits.units[i].targetPos) > 400)
			{
				friendlyUnits.units[i].position = Vector2MoveTowards(friendlyUnits.units[i].position, friendlyUnits.units[i].targetPos, friendlyUnits.units[i].movementSpeed);
				friendlyUnits.units[i].rotation = Vector2Angle(friendlyUnits.units[i].position, friendlyUnits.units[i].targetPos);
				friendlyUnits.units[i].rectangle = (Rectangle){friendlyUnits.units[i].position.x, friendlyUnits.units[i].position.y, 64, 64};
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

//void DestroyUnit(int index)
//{
//
//}
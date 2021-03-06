//
// Created by chris on 1/23/2022.
//

#include "ground_units.h"
#include "raymath.h"
#include "decals.h"

void InitFriendlies(void);
void InitEnemies(int);
void AttackUnit(int, int);
int EnemyUnitInRange(int, Vector2, float);
void AttackPlayer();
void GetNextObjective(Unit*);
int GetNearbyTarget(Vector2);
void UpdateFriendlyUnits();
void UpdateEnemyUnits();

// Also declared in screens.h, implemented in screen_gameplay.c
void DrawSprite(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation);
void DrawSpriteColor(int offsetX, int offsetY, Vector2 position, Vector2 origin, float rotation, Color color);
bool PointInsideRect(Vector2 point, Rectangle rect);

Vector2 enemyTargetPos1a = (Vector2){-2250, -150};
Vector2 enemyTargetPos1b = (Vector2){0, -150};
Vector2 enemyTargetPos1c = (Vector2){2250, -150};
Vector2 enemyTargetPos2  = (Vector2){0, 2800};

Vector2 targetList[3];

float soundCoolDownMax = 0.1f;
float soundCoolDown = 0;

void InitUnits(int wave)
{
    targetList[0] = enemyTargetPos1a;
    targetList[1] = enemyTargetPos1b;
    targetList[2] = enemyTargetPos1c;

    InitEnemies(wave);
	InitFriendlies();
}

void InitEnemies(int wave)
{
	enemyUnits.capacity = MAX_ENEMIES;
    const int count_for_wave = wave * 200;

    int randTarget = GetRandomValue(0, 2);
    int randTarget2 = GetRandomValue(0, 2);

	for (int i = 0; i < count_for_wave; ++i)
	{
        Unit* unit = &enemyUnits.units[i];
        unit->origin = (Vector2){32, 32};
        unit->position = (Vector2){GetRandomValue(-2999, 2999), GetRandomValue(-5000, -3500)};
        unit->rectangle = (Rectangle) {0, 0, 64, 64};
        unit->rotation = 180;
        unit->color = WHITE;
        unit->movementSpeed = (float)GetRandomValue(5,10) / 5;
        unit->active = true;
        unit->team = ENEMY_TEAM;
        unit->target = -1;
        unit->shotTimer = 0;

        int target = GetRandomValue(0, 2);
        switch (wave) {
            case 1:
                unit->moveTo = targetList[1];
                break;
            case 2:
                if (i%2 == 0)
                    unit->moveTo = targetList[0];
                else
                    unit->moveTo = targetList[2];
                break;
            case 3:
                unit->moveTo = targetList[randTarget];
                break;
            case 4:
                if (i%2 == 0)
                    unit->moveTo = targetList[randTarget];
                else
                    unit->moveTo = targetList[randTarget2];
                break;
            default:
                unit->moveTo = targetList[target];
        }
	}
}

void InitFriendlies()
{
	friendlyUnits.capacity = MAX_FRIENDLIES;

	for (int i = 0; i < friendlyUnits.capacity; ++i)
	{
        Unit* unit = &friendlyUnits.units[i];
        unit->origin = (Vector2){32, 32};
        unit->position = (Vector2){0, 600};
        unit->rectangle = (Rectangle) {0, 0, 64, 64};
        unit->rotation = 270;
        unit->color = WHITE;
        unit->movementSpeed = 2;
        unit->moveTo = Vector2Add(targetList[i%3], (Vector2){GetRandomValue(-300, 300), GetRandomValue(-290, 0)});
        unit->active = true;
        unit->team = FRIENDLY_TEAM;
        unit->target = -1;
        unit->shotTimer = 0;
	}
}

void UpdateUnits()
{
    UpdateEnemyUnits();
    UpdateFriendlyUnits();

    if (soundCoolDown > 0)
    {
        soundCoolDown -= GetFrameTime();

        if (soundCoolDown < 0)
            soundCoolDown = 0;
    }
}

void UpdateEnemyUnits()
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

                if (Vector2Distance(unit->position, player.position) < DETECTION_RANGE)
                {
                    unit->target = -2;
                }

                if (Vector2Distance(unit->position, unit->moveTo) > 100)
                {
                    unit->position = Vector2MoveTowards(unit->position, unit->moveTo, unit->movementSpeed);
                    unit->rotation = Vector2Angle(unit->position, unit->moveTo);
                    unit->rectangle = (Rectangle){unit->position.x, unit->position.y, 64, 64};
                }
                else
                {
                    GetNextObjective(unit);
                }
            }
            else if (unit->target == -2)
            {
                if (unit->shotTimer == 0)
                {
                    unit->rotation = Vector2Angle(unit->position, player.position);
                    AttackPlayer();
                    unit->shotTimer = SHOT_TIMER_MAX;
                }
            }
            else
            {
                if (friendlyUnits.units[unit->target].active)
                {
                    if (unit->shotTimer == 0)
                    {
                        unit->rotation = Vector2Angle(unit->position, friendlyUnits.units[unit->target].position);
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
}

void UpdateFriendlyUnits()
{
    for (int i = 0; i < friendlyUnits.capacity; ++i)
    {
        Unit *unit = &friendlyUnits.units[i];
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
                else
                {
                    unit->target = GetNearbyTarget(unit->position);
                }
            }
            else
            {
                if (enemyUnits.units[unit->target].active)
                {
                    if (unit->shotTimer == 0)
                    {
                        unit->rotation = Vector2Angle(unit->position, enemyUnits.units[unit->target].position);
                        AttackUnit(ENEMY_TEAM, unit->target);
                        unit->shotTimer = SHOT_TIMER_MAX;
                    }
                }
                else
                {
                    unit->target = -1;
                }
            }

            if (Vector2Distance(unit->position, unit->moveTo) > 50)
            {
                unit->position = Vector2MoveTowards(unit->position, unit->moveTo, unit->movementSpeed);
                unit->rotation = Vector2Angle(unit->position, unit->moveTo);
                unit->rectangle = (Rectangle) {unit->position.x, unit->position.y, 64, 64};
            }
        }
    }
}

void DrawUnits()
{
    Unit* unit;
    for (int i = 0; i < enemyUnits.capacity; ++i)
    {
        unit = &enemyUnits.units[i];
        if (unit->active && PointInsideRect(unit->position, (Rectangle){-3200, -3200, 6400, 6400}))
        {
            DrawSprite(16, 10, unit->position, unit->origin, unit->rotation);
            DrawSpriteColor(16, 10, unit->position, unit->origin, unit->rotation, GRAY);
            if (unit->shotTimer >= SHOT_TIMER_MAX - 0.15f)
            {
                DrawSprite(22, 12, unit->position, (Vector2){32, 64}, unit->rotation + 90);
            }
        }
    }

	for (int i = 0; i < friendlyUnits.capacity; ++i)
	{
        unit = &friendlyUnits.units[i];
		if (unit->active && PointInsideRect(unit->position, (Rectangle){-3200, -3200, 6400, 6400}))
		{
			DrawSprite(15, 10, unit->position, unit->origin, unit->rotation);
            if (unit->shotTimer >= SHOT_TIMER_MAX - 0.15f)
            {
                DrawSprite(22, 12, unit->position, (Vector2){32, 64}, unit->rotation + 90);
            }
		}
	}

    // Debug draws for moveTo targets
//    DrawCircleV(enemyTargetPos1a, 100, ColorAlpha(RED, 0.5f));
//    DrawCircleV(enemyTargetPos1b, 100, ColorAlpha(RED, 0.5f));
//    DrawCircleV(enemyTargetPos1c, 100, ColorAlpha(RED, 0.5f));
//    DrawCircleV(enemyTargetPos2, 100, ColorAlpha(RED, 0.5f));
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
                SpawnDecal(CORPSE, unitList->units[i].position);
				unitList->units[i].active = false;
				hits++;
			}
		}
	}

	return hits;
}

void AttackUnit(int team, int index)
{
    if (soundCoolDown == 0)
    {
        SetSoundVolume(fxUnitShoot, 0.15f);
        SetSoundPitch(fxUnitShoot, 1.0f - ((float)GetRandomValue(0, 10) / 100));
        PlaySoundMulti(fxUnitShoot);
        soundCoolDown = soundCoolDownMax;
    }
    Units* unitList = (team == FRIENDLY_TEAM) ? &friendlyUnits : &enemyUnits;
    float hitChance = (team == FRIENDLY_TEAM) ? FRIENDLY_HIT_CHANCE : ENEMY_HIT_CHANCE;
    if((float)GetRandomValue(0, 100) / 100.0f > (1.0f - hitChance))
    {
        SpawnDecal(CORPSE, unitList->units[index].position);
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

void AttackPlayer()
{
    SetSoundVolume(fxUnitShoot, 0.05f);
    PlaySoundMulti(fxUnitShoot);

    if((float)GetRandomValue(0, 100) / 100.0f > (1.0f - ENEMY_HIT_CHANCE))
    {
        player.health -= 1;
    }
}

void GetNextObjective(Unit* unit)
{
    unit->moveTo = enemyTargetPos2;
//    if (Vector2Distance(unit->moveTo, enemyTargetPos1a) < 100 || Vector2Distance(unit->moveTo, enemyTargetPos1c) < 100)
//        unit->moveTo = enemyTargetPos1b;
//    else if (Vector2Distance(unit->moveTo, enemyTargetPos1b) < 100)
//        unit->moveTo = enemyTargetPos2;
}

void ResetEnemies(int wave)
{
    InitEnemies(wave);
}

void ResetFriendlies()
{
    for (int i = 0; i < friendlyUnits.capacity; ++i)
    {
        Unit* unit = &friendlyUnits.units[i];
        if (!unit->active)
        {
            unit->active = true;
            unit->position = (Vector2){player.position.x + GetRandomValue(-300, 300), player.position.y + GetRandomValue(-300, 300)}; //unit->moveTo;
        }
        unit->target = -1;
    }
}

int GetNearbyTarget(Vector2 pos)
{
    for (int i = 0; i < friendlyUnits.capacity; ++i)
    {
        Unit* friend = &friendlyUnits.units[i];
        if (friend->active && Vector2Distance(pos, friend->position) < 200 && friend->target != -1)
            return friend->target;
    }

    return -1;
}

void EndGameResetUnits()
{
    for (int i = 0; i < enemyUnits.capacity; ++i)
    {
        enemyUnits.units[i].active = false;
    }

    for (int i = 0; i < friendlyUnits.capacity; ++i)
    {
        friendlyUnits.units[i].active = false;
    }
}
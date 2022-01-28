//
// Created by chris on 1/27/2022.
//

#include "decals.h"
#include "raylib.h"
#include "raymath.h"

void DrawSprite(int, int, Vector2, Vector2, float);
void DrawSpriteColor(int, int, Vector2, Vector2, float, Color);

void InitDecals(void)
{
    impactDecals.capacity = MAX_IMPACT_DECALS;
    impactDecals.iterator = 0;
    for (int i = 0; i < impactDecals.capacity; ++i)
    {
        impactDecals.decals[i].position = Vector2Zero();
        impactDecals.decals[i].origin = (Vector2){32, 32};
        impactDecals.decals[i].rotation = (float)GetRandomValue(0, 359);
        impactDecals.decals[i].active = false;
        impactDecals.decals[i].type = IMPACT;
    }

    corpseDecals.capacity = MAX_CORPSE_DECALS;
    corpseDecals.iterator = 0;
    for (int i = 0; i < corpseDecals.capacity; ++i)
    {
        corpseDecals.decals[i].position = Vector2Zero();
        corpseDecals.decals[i].origin = (Vector2){32, 32};
        corpseDecals.decals[i].rotation = (float)GetRandomValue(0, 359);
        corpseDecals.decals[i].active = false;
        corpseDecals.decals[i].type = CORPSE;
    }
}

void ResetDecals(void)
{
    InitDecals();
}

void SpawnDecal(DecalType type, Vector2 position)
{
    switch (type) {
        case IMPACT:
            impactDecals.decals[impactDecals.iterator].position = position;
            impactDecals.decals[impactDecals.iterator].active = true;
            impactDecals.iterator++;
            if (impactDecals.iterator == impactDecals.capacity) impactDecals.iterator = 0;
            break;
        case CORPSE:
            corpseDecals.decals[corpseDecals.iterator].position = position;
            corpseDecals.decals[corpseDecals.iterator].active = true;
            corpseDecals.iterator++;
            if (corpseDecals.iterator == corpseDecals.capacity) corpseDecals.iterator = 0;
        default: return;
    }
}

void DrawDecals(void)
{
    Decal* decal;
    for (int i = 0; i < impactDecals.capacity; ++i)
    {
        decal = &impactDecals.decals[i];
        if (decal->active)
        {
            decal = &impactDecals.decals[i];
            DrawSprite(21, 0, decal->position, decal->origin, decal->rotation);
        }
    }

    for (int i = 0; i < corpseDecals.capacity; ++i)
    {
        decal = &corpseDecals.decals[i];
        if (decal->active)
        {
            decal = &corpseDecals.decals[i];
            DrawTexturePro(corpseTexture, (Rectangle){0, 0, corpseTexture.width, corpseTexture.height}, (Rectangle){decal->position.x, decal->position.y, 64, 64}, (Vector2){32, 32}, decal->rotation, RED);
        }
    }
}

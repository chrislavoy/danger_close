//
// Created by chris on 1/27/2022.
//

#ifndef DANGER_CLOSE_DECALS_H
#define DANGER_CLOSE_DECALS_H

#include "raylib.h"

#define MAX_IMPACT_DECALS 200
#define MAX_CORPSE_DECALS 3000

typedef enum DecalType { IMPACT = 0, CORPSE } DecalType;

typedef struct Decal {
    Vector2 position;
    Vector2 origin;
    float rotation;
    DecalType type;
    bool active;
} Decal;

typedef struct ImpactDecals {
    int capacity;
    int iterator;
    Decal decals[MAX_IMPACT_DECALS];
} ImpactDecals;

typedef struct CorpseDecals {
    int capacity;
    int iterator;
    Decal decals[MAX_CORPSE_DECALS];
} CorpseDecals;

extern ImpactDecals impactDecals;
extern CorpseDecals corpseDecals;
extern Texture2D corpseTexture;

void InitDecals(void);
void ResetDecals(void);
void SpawnDecal(DecalType, Vector2);
void DrawDecals(void);

#endif //DANGER_CLOSE_DECALS_H

//
// Created by chris on 1/27/2022.
//

#ifndef DANGER_CLOSE_ANIMATIONS_H
#define DANGER_CLOSE_ANIMATIONS_H

#include "raylib.h"

#define MAX_ANIMATIONS 10

typedef struct Element {
    Vector2 position;
    Vector2 origin;
    Vector2 velocity;
//    Rectangle drawRect;
    float rotation;
    float scale;
} Element;

typedef struct Animation {
    Vector2 position;
    float runtime;
    float maxRuntime;
    Element elements[5];
    int elementCount;
    bool active;
} Animation;

typedef struct ImpactAnimations {
    int capacity;
    int iterator;
    Animation animations[MAX_ANIMATIONS];
} ImpactAnimations;

extern ImpactAnimations impactAnimations;
extern Animation shootAnimation;
extern Texture2D smokeTexture;

void InitAnimations(void);
void SpawnImpactAnimation(Vector2);
void StartShootAnimation(Vector2, float);
void UpdateAnimations(float);
void DrawAnimations(void);

#endif //DANGER_CLOSE_ANIMATIONS_H

//
// Created by chris on 1/27/2022.
//

#include "animations.h"
#include "raymath.h"

void InitImpactAnimations(void);
void InitShootAnimation(void);
void UpdateImpactAnimations(float);
void UpdateShootAnimation(float);
void DrawImpactAnimations(void);
void DrawShootAnimation(void);
Vector2 RotationToVector(float);

void InitAnimations(void)
{
    InitImpactAnimations();
    InitShootAnimation();
}

void InitImpactAnimations(void)
{
    impactAnimations.capacity = MAX_ANIMATIONS;
    impactAnimations.iterator = 0;
    for (int i = 0; i < impactAnimations.capacity; ++i)
    {
        Animation* animation = &impactAnimations.animations[i];
        animation->position = (Vector2){0, 0};
        animation->runtime = 0;
        animation->maxRuntime = 3.0f;
        animation->elementCount = 5;
        for (int j = 0; j < animation->elementCount; ++j)
        {
            animation->elements[j].position = animation->position;
            animation->elements[j].origin = (Vector2){32, 32};
            animation->elements[j].velocity = (Vector2){0, 0};
            animation->elements[j].rotation = (float)GetRandomValue(0, 359);
            animation->elements[j].scale = 1;
        }
        animation->elements[0].velocity = (Vector2){0, 0};
        animation->elements[1].velocity = (Vector2){-1, -1};
        animation->elements[2].velocity = (Vector2){1, -1};
        animation->elements[3].velocity = (Vector2){1, 1};
        animation->elements[4].velocity = (Vector2){-1, 1};
        animation->active = false;
    }
}

void InitShootAnimation(void)
{
    shootAnimation.position = (Vector2){0, 0};
    shootAnimation.runtime = 0;
    shootAnimation.maxRuntime = 0.5f;
    shootAnimation.elementCount = 5;
    for (int i = 0; i < shootAnimation.elementCount; ++i)
    {
        shootAnimation.elements[i].position = shootAnimation.position;
        shootAnimation.elements[i].origin = (Vector2){32, 32};
        shootAnimation.elements[i].velocity = (Vector2){0, 0};
        shootAnimation.elements[i].rotation = (float)GetRandomValue(0, 359);
        shootAnimation.elements[i].scale = 1;
    }
    shootAnimation.elements[0].velocity = (Vector2){0, -1};
    shootAnimation.elements[1].velocity = (Vector2){-2, 0};
    shootAnimation.elements[2].velocity = (Vector2){-1, 0};
    shootAnimation.elements[3].velocity = (Vector2){1, 0};
    shootAnimation.elements[4].velocity = (Vector2){2, 0};
    shootAnimation.active = false;
}

void SpawnImpactAnimation(Vector2 position)
{
    Animation* animation = &impactAnimations.animations[impactAnimations.iterator];
    animation->active = true;
    animation->position = position;
    for (int i = 0; i < animation->elementCount; ++i)
    {
        animation->elements[i].position = position;
    }

    impactAnimations.iterator++;

    if (impactAnimations.iterator == impactAnimations.capacity)
        impactAnimations.iterator = 0;
}

void StartShootAnimation(Vector2 position, float rotation)
{
    shootAnimation.active = true;
    shootAnimation.position = position;
    shootAnimation.runtime = 0;
    for (int i = 0; i < shootAnimation.elementCount; ++i)
    {
        shootAnimation.elements[i].position = position;
    }
}

void UpdateAnimations(float dt)
{
    UpdateImpactAnimations(dt);
    UpdateShootAnimation(dt);
}

void UpdateImpactAnimations(float dt)
{
    for (int i = 0; i < impactAnimations.capacity; ++i)
    {
        Animation* animation = &impactAnimations.animations[i];
        if (animation->active)
        {
            for (int j = 0; j < animation->elementCount; ++j)
            {
                Element* element = &animation->elements[j];
                element->position = Vector2Add(element->position, Vector2Scale(element->velocity, 10 * dt));
                element->rotation += 5 * dt;
            }

            animation->runtime += dt;

            if (animation->runtime > animation->maxRuntime)
            {
                animation->active = false;
                animation->runtime = 0;
            }
        }
    }
}

void UpdateShootAnimation(float dt)
{
    if (shootAnimation.active)
    {
        for (int i = 0; i < shootAnimation.elementCount; ++i)
        {
            Element* element = &shootAnimation.elements[i];
            element->position = Vector2Add(element->position, Vector2Scale(element->velocity, 50 * dt));
            element->rotation += 5 * dt;
        }

        shootAnimation.runtime += dt;

        if (shootAnimation.runtime > shootAnimation.maxRuntime)
        {
            shootAnimation.active = false;
            shootAnimation.runtime = 0;
        }
    }
}

void DrawAnimations()
{
    DrawShootAnimation();
    DrawImpactAnimations();
}

void DrawImpactAnimations(void)
{
    for (int i = 0; i < impactAnimations.capacity; ++i)
    {
        Animation* animation = &impactAnimations.animations[i];
        if (animation->active)
        {
            for (int j = 0; j < animation->elementCount; ++j)
            {
                Element* element = &animation->elements[j];
                element->origin = (Vector2)
                        {
                            (element->origin.x + (animation->runtime/animation->maxRuntime)),
                            (element->origin.y + (animation->runtime/animation->maxRuntime)),
                        };
                DrawTexturePro(
                        smokeTexture,
                        (Rectangle){0, 0, smokeTexture.width, smokeTexture.height},
                        (Rectangle){element->position.x, element->position.y, element->origin.x * 2, element->origin.y * 2},
                        element->origin,
                        element->rotation,
                        ColorAlpha(DARKGRAY, 1 - animation->runtime/animation->maxRuntime));
            }
        }
    }
}

void DrawShootAnimation(void)
{
    if (shootAnimation.active)
    {
        for (int i = 0; i < shootAnimation.elementCount; ++i)
        {
            Element* element = &shootAnimation.elements[i];
            element->origin = (Vector2)
                    {
                            (64 + (shootAnimation.runtime/shootAnimation.maxRuntime * 10))/2,
                            (64 + (shootAnimation.runtime/shootAnimation.maxRuntime * 10))/2,
                    };
            DrawTexturePro(
                    smokeTexture,
                    (Rectangle){0, 0, smokeTexture.width, smokeTexture.height},
                    (Rectangle){element->position.x, element->position.y, element->origin.x * 2, element->origin.y * 2},
                    element->origin,
                    element->rotation,
                    ColorAlpha(DARKGRAY, 1 - shootAnimation.runtime/shootAnimation.maxRuntime));
        }
    }
}
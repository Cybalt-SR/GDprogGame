#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h/Util.h"
#include "h/Entity.h"

static void punch(struct Entity *this, struct Entity *target)
{
}
static void kick(struct Entity *this, struct Entity *target)
{
}
static void throw(struct Entity * this, struct Entity *target)
{
}
static void magicAtk(struct Entity *this, struct Entity *target)
{
}
static void block(struct Entity *this, struct Entity *target)
{
}
static struct Entity new(int _hp, int _def, int _magic)
{
    return (struct Entity){
        .hp = _hp,
        .def = _def,
        .magic = _magic};
}
static void AskAllocPoints(int min, int max, int *remainingPoints, int *stat_field, char stat_label[])
{
    printf("Allocate (%i to %i) points to ", min, max);
    printf(stat_label);
    printf(" (%i remaining) : ", *remainingPoints);
    int pointsAlloc = AskInt("");

    int thereIsReason = 0;
    char *reason;
    if (pointsAlloc < min)
    {
        pointsAlloc = min;
        free(reason);
        reason = "capped at minimum";
        thereIsReason = 1;
    }
    if (pointsAlloc > max)
    {
        pointsAlloc = max;
        free(reason);
        reason = "capped at maximum";
        thereIsReason = 1;
    }
    if (pointsAlloc > *remainingPoints)
    {
        pointsAlloc = *remainingPoints;
        free(reason);
        reason = "not enough points";
        thereIsReason = 1;
    }

    *remainingPoints -= pointsAlloc;
    *stat_field = pointsAlloc;
    printf("Allocated %i points to ", pointsAlloc);
    printf(stat_label);

    if (thereIsReason)
    {
        printf(" (");
        printf(reason);
        printf(")");
    }

    printf("\n");
}
static struct Entity CreateEntity(int byPlayer)
{
    struct Entity entity = Entity.new(0, 0, 0);

    if (byPlayer)
    {
        int remainingPoints = 10;
        AskAllocPoints(1, 10, &remainingPoints, &entity.hp, "Health (1 point = 10 hp)");
        entity.hp *= 10;
        AskAllocPoints(0, 10, &remainingPoints, &entity.def, "Defense");
        AskAllocPoints(0, 10, &remainingPoints, &entity.magic, "Magic");
    }
    else
    {
        entity.hp = 50;
        entity.def = 5;
        entity.magic = 3;
    }

    return entity;
}
static void *GetAction(int which)
{
    return *Entity.AllActions[which];
}
static void DisplayActions(struct Entity *actioner)
{
    printf("Possible Actions : \n");
    for (int i = 0; i < ACTIONCOUNT; i++)
    {
        if (i != MAGICACTIONID || (actioner->magic > 0))
            printf("[%i] %s\n", i, Entity.AllActionNames[i]);
    }
}
const struct EntityClass Entity = {
    .new = &new,
    .AskAllocPoints = &AskAllocPoints,
    .CreateEntity = &CreateEntity,
    .AllActions = {&punch, &kick, &throw, &magicAtk, &block},
    .AllActionNames = {"Punch", "Kick", "Throw", "Magic Attack", "Block"},
    .DisplayActions = &DisplayActions};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h/Util.h"
#include "h/Entity.h"

static void punch(Entity *this, Entity *target)
{
}
static void kick(Entity *this, Entity *target)
{
}
static void throw(Entity * this, Entity *target)
{
}
static void magicAtk(Entity *this, Entity *target)
{
}
static void block(Entity *this, Entity *target)
{
}
static Entity new(int _hp, int _def, int _magic)
{
    return (Entity){
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
static Entity CreateEntity(int byPlayer)
{
    Entity entity = Entity.new(0, 0, 0);

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
static EntityAction GetAction(Entity *actioner, int randomAuto)
{
    int canMagicAttack = 1;

    if (randomAuto == 0)
    {
        printf("Possible Actions : \n");
        for (int i = 0; i < ACTIONCOUNT; i++)
        {
            if (Entity.AllActions[i] != &magicAtk || (actioner->magic > 0))
                printf("[%i] %s\n", i, Entity.AllActionNames[i]);
            else
                canMagicAttack = 0;
        }
    }

    int ActionId = (randomAuto == 0) ? AskInt("Action to do : ") : RandomRange(0, ACTIONCOUNT);

    if (ActionId >= 0 && ActionId < ACTIONCOUNT)
    {
        if (Entity.AllActions[ActionId] == &magicAtk && canMagicAttack == 0)
        {
            if (randomAuto == 0)
                printf("You do not have any charges for this. Please choose another.\n");
            return GetAction(actioner, randomAuto);
        }
        else
        {
            return Entity.AllActions[ActionId];
        }
    }
    else if (randomAuto == 0)
    {
        printf("Invalid choice. Please choose another.\n");
        return GetAction(actioner, randomAuto);
    }
}
const struct Entity Entity = {
    .new = &new,
    .AskAllocPoints = &AskAllocPoints,
    .CreateEntity = &CreateEntity,
    .AllActions = {&punch, &kick, &throw, &magicAtk, &block},
    .AllActionNames = {"Punch", "Kick", "Throw", "Magic Attack", "Block"},
    .GetAction = &GetAction};

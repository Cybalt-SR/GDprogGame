#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h/Util.h"
#include "h/Entity.h"

//Entity Actions
static void Punch(Entity *this, Entity *target)
{
}
static void Kick(Entity *this, Entity *target)
{
}
static void Throw(Entity * this, Entity *target)
{
}
static void MagicAtk(Entity *this, Entity *target)
{
}
static void Block(Entity *this, Entity *target)
{
}

//Entity utils
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
    Entity entity = {.hp = 0, .def = 0, .magic = 0};

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
            if (EntityActions.All[i].Action != EntityActions.Specific.MagicAtk.Action || (actioner->magic > 0))
                printf("[%i] %s\n", i, EntityActions.All[i].name);
            else
                canMagicAttack = 0;
        }
    }

    int ActionId = (randomAuto == 0) ? AskInt("Action to do : ") : RandomRange(0, ACTIONCOUNT);

    if (ActionId >= 0 && ActionId < ACTIONCOUNT)
    {
        if (EntityActions.All[ActionId].Action == EntityActions.Specific.MagicAtk.Action && canMagicAttack == 0)
        {
            if (randomAuto == 0)
                printf("You do not have any charges for this. Please choose another.\n");
            return GetAction(actioner, randomAuto);
        }
        else
        {
            return EntityActions.All[ActionId];
        }
    }
    else if (randomAuto == 0)
    {
        printf("Invalid choice. Please choose another.\n");
        return GetAction(actioner, randomAuto);
    }
}
const struct EntityUtilType EntityUtil= {
    .AskAllocPoints = &AskAllocPoints,
    .CreateEntity = &CreateEntity,
    .GetAction = &GetAction};

const union EntityActionsType EntityActions = {
    .Specific.Block = {&Block, "Block"},
    .Specific.Kick = {&Kick, "Kick"},
    .Specific.MagicAtk = {&MagicAtk, "Magic Attack"},
    .Specific.Throw = {&Throw, "Throw"},
    .Specific.Punch = {&Punch, "Punch"},
};
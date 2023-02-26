#include "h/Util.h"
#include "h/EntityList.h"
#include "h/Entity.h"

//==============================
// Entity Actions
//==============================

static void Punch(Entity *this, Entity *target)
{
    printf("%s Punched! ", this->name);

    if (RandomRange(1, 10) > target->def)
    {
        int damage = RandomRange(1, 10);
        target->hp -= damage;

        printf("It hit %s for %i damage! \n", target->name, damage);
    }
    else
    {
        printf("It missed... \n");
    }
}
static void Kick(Entity *this, Entity *target)
{
    printf("%s Kicked! ", this->name);

    if (RandomRange(2, 9) > target->def)
    {
        int damage = RandomRange(1, 10) * 2;
        target->hp -= damage;

        int defMod = RandomRange(-5, 0);
        EntityList.Add(target->DefModifiers, defMod, 2);

        printf("It hit %s for %i damage, decreasing %s def by %i until next turn! \n", target->name, damage, target->pronoun, defMod);
    }
    else
    {
        printf("It missed... \n");
    }
}
static void Throw(Entity *this, Entity *target)
{
    printf("%s Throwed! ", this->name);
    int actualDef = target->def;
    EntityList.GetTotal(target->DefModifiers, &actualDef);

    if (RandomRange(2, 9) > actualDef)
    {
        int damage = RandomRange(1, 5);
        target->hp -= damage;

        int targetDefMod = RandomRange(-10, -1);
        int thisDefMod = RandomRange(-3, -1);
        EntityList.Add(target->DefModifiers, targetDefMod, 2);
        EntityList.Add(this->DefModifiers, thisDefMod, 2);

        printf("It hit %s for %i damage, decreasing %s def by %i and %s def by %i until next turn! \n", target->name, damage, target->pronoun, targetDefMod, this->pronoun, thisDefMod);
    }
    else
    {
        printf("It missed... \n");
    }
}
static void MagicAtk(Entity *this, Entity *target)
{
    printf("%s Magic Attacked! ", this->name);

    if (RandomRange(1, 10) > target->def)
    {
        int damage = RandomRange(1, 10);
        target->hp -= damage;

        printf("It hit %s for %i damage! \n", target->name, damage);
    }
    else
    {
        printf("It missed... \n");
    }
}
static void Block(Entity *this, Entity *target)
{
    int defMod = RandomRange(1, 10);
    EntityList.Add(this->DefModifiers, defMod, 1);
    printf("%s Blocked! Increasing DEF by %i that turn!\n", this->name, defMod);
}

static EntityEvent GetActionEvent(Entity *actioner, int Automated)
{
    int canMagicAttack = 1;

    // Display possible actions if the GET is not automated
    if (Automated == 0)
    {
        printf("Possible Actions : \n");
        for (int i = 0; i < ACTIONCOUNT; i++)
        {
            if (EntityActions.All[i].Action != EntityActions.S.MagicAtk.Action || (actioner->magic > 0))
                printf("[%i] %s\n", i, EntityActions.All[i].name);
            else
                canMagicAttack = 0;
        }
    }

    // Ask manually if not automated
    int ActionId = (Automated == 0) ? AskInt("Action to do : ") : RandomRange(0, ACTIONCOUNT);

    if (ActionId >= 0 && ActionId < ACTIONCOUNT)
    {
        if (EntityActions.All[ActionId].Action == EntityActions.S.MagicAtk.Action && canMagicAttack == 0)
        {
            // Notify if not automated
            if (Automated == 0)
                printf("You do not have any charges for this. Please choose another.\n");
            return GetActionEvent(actioner, Automated);
        }
        else
        {
            EntityEvent entityEvent = {.action = &EntityActions.All[ActionId], .doer = actioner};
            return entityEvent;
        }
    }
    else if (Automated == 0) // This only happens if it is not automated
    {
        printf("Invalid choice. Please choose another.\n");
        return GetActionEvent(actioner, Automated);
    }
}

EntityEvent AskPlayerAction(Entity *actioner) { return GetActionEvent(actioner, 0); }
EntityEvent GetRandomAction(Entity *actioner) { return GetActionEvent(actioner, 1); }

const union EntityActions EntityActions = {
    .S.Block = {&Block, "Block", 0},
    .S.Kick = {&Kick, "Kick", 1},
    .S.MagicAtk = {&MagicAtk, "Magic Attack", 0},
    .S.Throw = {&Throw, "Throw", 1},
    .S.Punch = {&Punch, "Punch", 0},
};

//==============================
// Entity Constructing
//==============================

static void AskAllocPoints(int min, int max, int *remainingPoints, int *stat_field, char stat_label[])
{
    printf("Allocate (%i to %i) points to ", min, max);
    printf(stat_label);
    printf(" (%i remaining) : ", *remainingPoints);
    int pointsAlloc = AskInt("");

    int thereIsReason = 0;
    char *reason;
    reason = (char *)malloc(18);
    if (pointsAlloc < min)
    {
        pointsAlloc = min;
        strcpy(reason, "capped at minimum");
        thereIsReason = 1;
    }
    if (pointsAlloc > max)
    {
        pointsAlloc = max;
        strcpy(reason, "capped at maximum");
        thereIsReason = 1;
    }
    if (pointsAlloc > *remainingPoints)
    {
        pointsAlloc = *remainingPoints;
        strcpy(reason, "not enough points");
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

    free(reason);
}
static Entity *Create(char name[], char pronoun[], int byPlayer)
{
    Entity *entity = (Entity *)malloc(sizeof(Entity));
    entity->name = name;
    entity->pronoun = pronoun;
    entity->hp = 0;
    entity->def = 0;
    entity->magic = 0;
    entity->DefModifiers = EntityList.CreateList();

    if (byPlayer)
    {
        int remainingPoints = 10;
        AskAllocPoints(1, 10, &remainingPoints, &entity->hp, "Health (1 point = 10 hp)");
        entity->hp *= 10;
        AskAllocPoints(0, 10, &remainingPoints, &entity->def, "Defense");
        AskAllocPoints(0, 10, &remainingPoints, &entity->magic, "Magic");

        entity->GetActionEvent = &AskPlayerAction;
    }
    else
    {
        entity->hp = RandomRange(10, 50);
        entity->def = RandomRange(1, 5);
        entity->magic = RandomRange(0, 3);
        entity->GetActionEvent = &GetRandomAction;
    }

    return entity;
}

const struct EntityConstructor EntityConstructor = {.Create = &Create};

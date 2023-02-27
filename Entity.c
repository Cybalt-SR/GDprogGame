#include "h/Util.h"
#include "h/EntityList.h"
#include "h/Entity.h"

//==============================
// Entity Actions
//==============================

static void Punch(Entity *this, Entity *target)
{
    Print("%s Punched! ", Colors.Cyan, this->name);

    if (RandomRange(1, 10) > target->def)
    {
        int damage = RandomRange(1, 10);
        target->hp -= damage;

        Print("It hit %s for %i damage! \n", Colors.Cyan, target->name, damage);
    }
    else
    {
        Print("It missed... \n", Colors.Cyan);
    }
}
static void Kick(Entity *this, Entity *target)
{
    Print("%s Kicked! ", Colors.Cyan, this->name);

    if (RandomRange(2, 9) > target->def)
    {
        int damage = RandomRange(1, 10) * 2;
        target->hp -= damage;

        int defMod = RandomRange(-5, 0);
        EntityList.Add(target->DefModifiers, defMod, 2);

        Print("It hit %s for %i damage, decreasing %s def by %i until next turn! \n", Colors.Cyan, target->name, damage, target->pronoun, defMod);
    }
    else
    {
        Print("It missed... \n", Colors.Cyan);
    }
}
static void Throw(Entity *this, Entity *target)
{
    Print("%s Throwed! ", Colors.Cyan, this->name);
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

        Print("It hit %s for %i damage, decreasing %s def by %i and %s def by %i until next turn! \n", Colors.Cyan, target->name, damage, target->pronoun, targetDefMod, this->pronoun, thisDefMod);
    }
    else
    {
        Print("It missed... \n", Colors.Cyan);
    }
}
static void MagicAtk(Entity *this, Entity *target)
{
    Print("%s Magic Attacked! ", Colors.Cyan, this->name);

    if (RandomRange(1, 10) > target->def)
    {
        int damage = RandomRange(1, 10);
        target->hp -= damage;

        Print("It hit %s for %i damage! \n", Colors.Cyan, target->name, damage);
    }
    else
    {
        Print("It missed... \n", Colors.Cyan);
    }
}
static void Block(Entity *this, Entity *target)
{
    int defMod = RandomRange(1, 10);
    EntityList.Add(this->DefModifiers, defMod, 1);
    Print("%s Blocked! Increasing DEF by %i that turn!\n", Colors.Cyan, this->name, defMod);
}

static EntityEvent GetActionEvent(Entity *actioner, int Automated)
{
    int canMagicAttack = 1;

    // Display possible actions if the GET is not automated
    if (Automated == 0)
    {
        Print("Possible Actions : \n", Colors.Reset);
        for (int i = 0; i < ACTIONCOUNT; i++)
        {
            if (EntityActions.All[i].Action != EntityActions.S.MagicAtk.Action || (actioner->magic > 0))
                Print("[%i] %s\n", Colors.Reset, i, EntityActions.All[i].name);
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
                Print("You do not have any charges for this. Please choose another.\n", Colors.Reset);
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
        Print("Invalid choice. Please choose another.\n", Colors.Reset);
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
    Print("Allocate (%i to %i) points to ", Colors.Reset, min, max);
    Print(stat_label, Colors.Reset);
    Print(" (%i remaining) : ", Colors.Reset, *remainingPoints);
    int pointsAlloc = AskInt("");

    char *reason = NULL;
    if (pointsAlloc < min)
    {
        pointsAlloc = min;
        reason = "capped at minimum";
    }
    if (pointsAlloc > max)
    {
        pointsAlloc = max;
        reason = "capped at maximum";
    }
    if (pointsAlloc > *remainingPoints)
    {
        pointsAlloc = *remainingPoints;
        reason = "not enough points";
    }

    *remainingPoints -= pointsAlloc;
    *stat_field = pointsAlloc;
    Print("Allocated %i points to ", Colors.Reset, pointsAlloc);
    Print(stat_label, Colors.Reset);

    if (reason != NULL)
    {
        Print(" (", Colors.Reset);
        Print(reason, Colors.Reset);
        Print(")", Colors.Reset);
    }

    Print("\n", Colors.Reset);
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

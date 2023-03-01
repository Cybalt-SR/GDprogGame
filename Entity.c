#include "h/u_stdio.h"

//==============================
// Entity Actions
//==============================

/*
Scope: Private
Use: Automate roll checking and displaying across different actions.
Remarks: This acts as an automatic ELSE if customElse is FALSE.
*/
static int RollAgainst(int min, int max, int mult, Entity *target, int customElse)
{
    int roll = RandomRange(min, max) * mult;
    int targetDef = target->def;
    List.GetTotal(target->DefModifiers, &targetDef);

    int result = roll > targetDef;
    if (result)
    {
        Print("It hit (%i vs %i) ", Colors.Cyan, roll, targetDef);
    }
    else
    {
        Print("It missed (%i vs %i)... ", Colors.Cyan, roll, targetDef);

        if (customElse != 1)
        {
            Print("\n", Colors.Reset);
        }
    }

    return result;
}

/*
Scope: Member Function
Use: When an attacker chooses punch, generate a random number between 1 and 10. If the random 
number is higher than the victim’s defense, the punch hits. If the punch hits, generate another random 
number between 1 and 10, and subtract this from the victim’s health.
*/
static void Punch(Entity *this, Entity *target)
{
    Print("%s Punched! ", Colors.Cyan, this->name);

    if (RollAgainst(1, 10, 1, target, 0))
    {
        int damage = RandomRange(1, 10);
        target->hp -= damage;

        Print("%s for %i damage! \n", Colors.Cyan, target->name, damage);
    }
}
/*
Scope: Member Function
Use: When  an  attacker  chooses  kick,  generate  a  random  number  between  2  and  9.  If  the  random 
number  is  higher  than  the  victim’s defense, the kick hits, otherwise the kick misses. If the kick hits, 
generate another random number between 1 and 10, and multiply this by 2, and subtract  this product 
from  the victim’s health. Regardless of whether the kick hits or misses, generate a random  number 
between 0 and 5. If the kick hits, subtract that number from the attacker’s defense for this turn ONLY, but 
if the kick misses subtract that number from the attacker’s defense for this turn and the next turn ONLY.
*/
static void Kick(Entity *this, Entity *target)
{
    Print("%s Kicked! ", Colors.Cyan, this->name);

    int defMod = RandomRange(-5, 0);

    if (RollAgainst(2, 9, 1, target, 1))
    {
        int damage = RandomRange(1, 10) * 2;
        target->hp -= damage;

        List.Add(this->DefModifiers, Element.GetModifier(defMod, 1));

        Print("%s for %i damage, decreasing %s def by %i for that turn! \n", Colors.Cyan, target->name, damage, this->pronoun, defMod);
    }
    else
    {

        List.Add(this->DefModifiers, Element.GetModifier(defMod, 2));
        Print("decreasing %s def by %i until next turn! \n", Colors.Cyan, this->pronoun, defMod);
    }
}
/*
Scope: Member Function
When an attacker chooses throw, generate a random number between 0 and 10. If the random 
number is higher than the victim’s defense, the throw is successful, otherwise the throw is unsuccessful. 
If the throw  is successful, generate another random number between 1  and 5, and subtract this to the 
victim’s health, and then generate another random number between 1 and 10, and subtract this to the 
victim’s defense for this  turn  and  the  next  turn  ONLY.  If  the  throw  is  successful,  generate  a  random 
number  between  1  and  3.  Subtract  that  number  from  the  attacker’s  defense  for  this  turn  ONLY.  If  the 
throw  is  unsuccessful,  generate  a  random  number  between  1  and  10.  Subtract  that  number  from  the 
attacker’s defense for this turn ONLY. 
*/
static void Throw(Entity *this, Entity *target)
{
    Print("%s Throwed! ", Colors.Cyan, this->name);

    if (RollAgainst(0, 10, 1, target, 1))
    {
        int damage = RandomRange(1, 5);
        target->hp -= damage;

        int targetDefMod = RandomRange(-10, -1);
        int thisDefMod = RandomRange(-3, -1);
        List.Add(target->DefModifiers, Element.GetModifier(targetDefMod, 2));
        List.Add(this->DefModifiers, Element.GetModifier(thisDefMod, 1));

        Print("%s for %i damage, decreasing %s def by %i and %s def by %i until next turn! \n", Colors.Cyan, target->name, damage, target->pronoun, targetDefMod, this->pronoun, thisDefMod);
    }else{
        int thisDefMod = RandomRange(-10, -1);
        List.Add(this->DefModifiers, Element.GetModifier(thisDefMod, 1));
        Print("decreasing %s def by %i until next turn! \n", Colors.Cyan, this->pronoun, thisDefMod);
    }
}
/*
Scope: Member Function
An attacker can only choose magic attack if they have at least 1 magic. If their magic is 0, the magic 
attack option should not be an available option. When an attacker chooses magic attack, subtract 1 from 
their magic statistic. 
When an attacker chooses magic attack, generate a random number between 1 and 5, and then 
multiply  that  by  3.  If  the  product  is  higher  than the victim’s defense, the magic attack  is  successful, 
otherwise  the  magic  attack  is  unsuccessful.  If  the  magic  attack  is  successful,  generate  another  random 
number between 1 and 10, and multiply this by 5, and then subtract this product from the victim’s health.  
*/
static void MagicAtk(Entity *this, Entity *target)
{
    Print("%s Magic Attacked! ", Colors.Cyan, this->name);

    if (RollAgainst(1, 5, 3, target, 0))
    {
        int damage = RandomRange(1, 10) * 5;
        target->hp -= damage;
        this->magic -= 1;

        Print("%s for %i damage! \n", Colors.Cyan, target->name, damage);
    }
}
/*
Scope: Member Function
When the player chooses block, generate a random number between 1 and 10, and add this to 
the player’s defense for this turn ONLY. When the computer chooses block, generate a random number 
between 1 and 10, and add this to the computer’s defense for this turn ONLY.  
*/
static void Block(Entity *this, Entity *target)
{
    int defMod = RandomRange(1, 10);
    List.Add(this->DefModifiers, Element.GetModifier(defMod, 1));
    Print("%s Blocked! Increasing DEF by %i that turn!\n", Colors.Cyan, this->name, defMod);
}

/*
Scope: Partial Member Function
This function is used to automatically randomize or request an action depending on the entity and get type.
*/
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

    if (ActionId >= 0 && ActionId < ACTIONCOUNT) //Within range check
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
    else if (Automated == 0) // Recursive input validation. This only happens if it is not automated
    {
        Print("Invalid choice. Please choose another.\n", Colors.Reset);
        return GetActionEvent(actioner, Automated);
    }
}

// Wrapped function call within another function with abstracted parameters.
EntityEvent AskPlayerAction(Entity *actioner) { return GetActionEvent(actioner, 0); }
// Wrapped function call within another function with abstracted parameters.
EntityEvent GetRandomAction(Entity *actioner) { return GetActionEvent(actioner, 1); }

//The complete union (named array) of Entity Actions.
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

/*
Scope: Namespace Function
Use: Ask the player where to allocate their remaining points.
*/
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

/*
Scope: Partial Namespace Function
Use: Abstracted function that exists only to wrap the allocation and initialization of an Entity struct.
*/
static Entity *CreateEntityShell(char name[], char pronoun[])
{
    Entity *entity = (Entity *)uMemAlloc(sizeof(Entity));
    entity->name = name;
    entity->pronoun = pronoun;
    entity->hp = 0;
    entity->def = 0;
    entity->magic = 0;
    entity->DefModifiers = List.CreateList();

    return entity;
}

/*
Scope: Namespace Function
Use: Entity with user-defined point allocation.
*/
static Entity *CreatePlayer(char name[], char pronoun[])
{
    Entity *entity = CreateEntityShell(name, pronoun);

    int remainingPoints = 10;

    AskAllocPoints(1, 10, &remainingPoints, &entity->hp, "Health (1 point = 10 hp)");
    entity->hp *= 10;
    AskAllocPoints(0, 10, &remainingPoints, &entity->def, "Defense");
    AskAllocPoints(0, 10, &remainingPoints, &entity->magic, "Magic");

    entity->GetActionEvent = &AskPlayerAction;

    return entity;
}

/*
Scope: Namespace Function
Use: Entity with randomized point allocation depending on level.
*/
static Entity *CreateBot(char name[], char pronoun[], int level)
{
    Entity *entity = CreateEntityShell(name, pronoun);

    entity->hp = RandomRange(10 + (10 * level), 50 + (10 * level));
    entity->def = RandomRange(Min(1 + level, 8), Min(5 + level, 9));
    entity->magic = RandomRange(0, 3 + level);
    entity->GetActionEvent = &GetRandomAction;

    return entity;
}

/*
Scope: Namespace
Use: Contains the two constructor functions.
*/
const struct EntityConstructor EntityConstructor = {.CreateBot = &CreateBot, .CreatePlayer = &CreatePlayer};
